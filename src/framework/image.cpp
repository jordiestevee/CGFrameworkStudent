#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "GL/glew.h"
#include "../extra/picopng.h"
#include "image.h"
#include "utils.h"
#include "camera.h"
#include "mesh.h"


Image::Image() {
	width = 0; height = 0;
	pixels = NULL;
}

Image::Image(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new Color[width*height];
	memset(pixels, 0, width * height * sizeof(Color));
}

// Copy constructor
Image::Image(const Image& c)
{
	pixels = NULL;
	width = c.width;
	height = c.height;
	bytes_per_pixel = c.bytes_per_pixel;
	if(c.pixels)
	{
		pixels = new Color[width*height];
		memcpy(pixels, c.pixels, width*height*bytes_per_pixel);
	}
}

// Assign operator
Image& Image::operator = (const Image& c)
{
	if(pixels) delete pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	bytes_per_pixel = c.bytes_per_pixel;

	if(c.pixels)
	{
		pixels = new Color[width*height*bytes_per_pixel];
		memcpy(pixels, c.pixels, width*height*bytes_per_pixel);
	}
	return *this;
}

Image::~Image()
{
	if(pixels) 
		delete pixels;
}

void Image::Render()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(width, height, bytes_per_pixel == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

// Change image size (the old one will remain in the top-left corner)
void Image::Resize(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for(unsigned int x = 0; x < min_width; ++x)
		for(unsigned int y = 0; y < min_height; ++y)
			new_pixels[ y * width + x ] = GetPixel(x,y);

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

// Change image size and scale the content
void Image::Scale(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];

	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++y)
			new_pixels[ y * width + x ] = GetPixel((unsigned int)(this->width * (x / (float)width)), (unsigned int)(this->height * (y / (float)height)) );

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

Image Image::GetArea(unsigned int start_x, unsigned int start_y, unsigned int width, unsigned int height)
{
	Image result(width, height);
	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++x)
		{
			if( (x + start_x) < this->width && (y + start_y) < this->height) 
				result.SetPixel( x, y, GetPixel(x + start_x,y + start_y) );
		}
	return result;
}

void Image::FlipY()
{
	int row_size = bytes_per_pixel * width;
	Uint8* temp_row = new Uint8[row_size];
#pragma omp simd
	for (int y = 0; y < height * 0.5; y += 1)
	{
		Uint8* pos = (Uint8*)pixels + y * row_size;
		memcpy(temp_row, pos, row_size);
		Uint8* pos2 = (Uint8*)pixels + (height - y - 1) * row_size;
		memcpy(pos, pos2, row_size);
		memcpy(pos2, temp_row, row_size);
	}
	delete[] temp_row;
}

bool Image::LoadPNG(const char* filename, bool flip_y)
{
	std::string sfullPath = absResPath(filename);
	std::ifstream file(sfullPath, std::ios::in | std::ios::binary | std::ios::ate);

	// Get filesize
	std::streamsize size = 0;
	if (file.seekg(0, std::ios::end).good()) size = file.tellg();
	if (file.seekg(0, std::ios::beg).good()) size -= file.tellg();

	if (!size)
		return false;

	std::vector<unsigned char> buffer;

	// Read contents of the file into the vector
	if (size > 0)
	{
		buffer.resize((size_t)size);
		file.read((char*)(&buffer[0]), size);
	}
	else
		buffer.clear();

	std::vector<unsigned char> out_image;

	if (decodePNG(out_image, width, height, buffer.empty() ? 0 : &buffer[0], (unsigned long)buffer.size(), true) != 0)
		return false;

	size_t bufferSize = out_image.size();
	unsigned int originalBytesPerPixel = (unsigned int)bufferSize / (width * height);
	
	// Force 3 channels
	bytes_per_pixel = 3;

	if (originalBytesPerPixel == 3) {
		pixels = new Color[bufferSize];
		memcpy(pixels, &out_image[0], bufferSize);
	}
	else if (originalBytesPerPixel == 4) {

		unsigned int newBufferSize = width * height * bytes_per_pixel;
		pixels = new Color[newBufferSize];

		unsigned int k = 0;
		for (unsigned int i = 0; i < bufferSize; i += originalBytesPerPixel) {
			pixels[k] = Color(out_image[i], out_image[i + 1], out_image[i + 2]);
			k++;
		}
	}

	// Flip pixels in Y
	if (flip_y)
		FlipY();

	return true;
}

// Loads an image from a TGA file
bool Image::LoadTGA(const char* filename, bool flip_y)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char TGAcompare[12];
	unsigned char header[6];
	unsigned int imageSize;
	unsigned int bytesPerPixel;

    std::string sfullPath = absResPath( filename );

	FILE * file = fopen( sfullPath.c_str(), "rb");
   	if ( file == NULL || fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||
		fread(header, 1, sizeof(header), file) != sizeof(header))
	{
		std::cerr << "File not found: " << sfullPath.c_str() << std::endl;
		if (file == NULL)
			return NULL;
		else
		{
			fclose(file);
			return NULL;
		}
	}

	TGAInfo* tgainfo = new TGAInfo;
    
	tgainfo->width = header[1] * 256 + header[0];
	tgainfo->height = header[3] * 256 + header[2];
    
	if (tgainfo->width <= 0 || tgainfo->height <= 0 || (header[4] != 24 && header[4] != 32))
	{
		fclose(file);
		delete tgainfo;
		return NULL;
	}
    
	tgainfo->bpp = header[4];
	bytesPerPixel = tgainfo->bpp / 8;
	imageSize = tgainfo->width * tgainfo->height * bytesPerPixel;
    
	tgainfo->data = new unsigned char[imageSize];
    
	if (tgainfo->data == NULL || fread(tgainfo->data, 1, imageSize, file) != imageSize)
	{
		if (tgainfo->data != NULL)
			delete tgainfo->data;
            
		fclose(file);
		delete tgainfo;
		return false;
	}

	fclose(file);

	// Save info in image
	if(pixels)
		delete pixels;

	width = tgainfo->width;
	height = tgainfo->height;
	pixels = new Color[width*height];

	// Convert to float all pixels
	for (unsigned int y = 0; y < height; ++y) {
		for (unsigned int x = 0; x < width; ++x) {
			unsigned int pos = y * width * bytesPerPixel + x * bytesPerPixel;
			// Make sure we don't access out of memory
			if( (pos < imageSize) && (pos + 1 < imageSize) && (pos + 2 < imageSize))
				SetPixel(x, height - y - 1, Color(tgainfo->data[pos + 2], tgainfo->data[pos + 1], tgainfo->data[pos]));
		}
	}

	// Flip pixels in Y
	if (flip_y)
		FlipY();

	delete tgainfo->data;
	delete tgainfo;

	return true;
}

// Saves the image to a TGA file
bool Image::SaveTGA(const char* filename)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	std::string fullPath = absResPath(filename);
	FILE *file = fopen(fullPath.c_str(), "wb");
	if ( file == NULL )
	{
		perror("Failed to open file: ");
		return false;
	}

	unsigned short header_short[3];
	header_short[0] = width;
	header_short[1] = height;
	unsigned char* header = (unsigned char*)header_short;
	header[4] = 24;
	header[5] = 0;

	fwrite(TGAheader, 1, sizeof(TGAheader), file);
	fwrite(header, 1, 6, file);

	// Convert pixels to unsigned char
	unsigned char* bytes = new unsigned char[width*height*3];
	for(unsigned int y = 0; y < height; ++y)
		for(unsigned int x = 0; x < width; ++x)
		{
			Color c = pixels[y*width+x];
			unsigned int pos = (y*width+x)*3;
			bytes[pos+2] = c.r;
			bytes[pos+1] = c.g;
			bytes[pos] = c.b;
		}

	fwrite(bytes, 1, width*height*3, file);
	fclose(file);

	return true;
}

void Image::DrawRect(int x, int y, int w, int h, const Color& borderColor,
	int borderWidth, bool isFilled, const Color& fillColor)
{
	for (int d = 0; d < borderWidth; ++d) {
		for (int i = x - borderWidth; i < x + w + borderWidth; ++i) {
			SetPixelSafe(i, y-d-1, borderColor);
			SetPixelSafe(i, y + h + d, borderColor);
		}

		for (int j = y; j < y + h; ++j) {
			SetPixelSafe(x - d-1, j, borderColor);
			SetPixelSafe(x + w + d, j, borderColor);
		}
	}

	if (isFilled) {
		for (int i = x; i < x + w; ++i) {
			for (int j = y; j < y + h; ++j) {
				SetPixelSafe(i, j, fillColor);
			}
		}
	}
}


#ifndef IGNORE_LAMBDAS

// You can apply and algorithm for two images and store the result in the first one
// ForEachPixel( img, img2, [](Color a, Color b) { return a + b; } );
template <typename F>
void ForEachPixel(Image& img, const Image& img2, F f) {
	for(unsigned int pos = 0; pos < img.width * img.height; ++pos)
		img.pixels[pos] = f( img.pixels[pos], img2.pixels[pos] );
}

#endif

FloatImage::FloatImage(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new float[width * height];
	memset(pixels, 0, width * height * sizeof(float));
}

// Copy constructor
FloatImage::FloatImage(const FloatImage& c) {
	pixels = NULL;

	width = c.width;
	height = c.height;
	if (c.pixels)
	{
		pixels = new float[width * height];
		memcpy(pixels, c.pixels, width * height * sizeof(float));
	}
}

// Assign operator
FloatImage& FloatImage::operator = (const FloatImage& c)
{
	if (pixels) delete pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	if (c.pixels)
	{
		pixels = new float[width * height * sizeof(float)];
		memcpy(pixels, c.pixels, width * height * sizeof(float));
	}
	return *this;
}

FloatImage::~FloatImage()
{
	if (pixels)
		delete pixels;
}

// Change image size (the old one will remain in the top-left corner)
void FloatImage::Resize(unsigned int width, unsigned int height)
{
	float* new_pixels = new float[width * height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for (unsigned int x = 0; x < min_width; ++x)
		for (unsigned int y = 0; y < min_height; ++y)
			new_pixels[y * width + x] = GetPixel(x, y);

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

void Image::DrawLineDDA(int x0, int y0, int x1, int y1, const Color& c) {
	int dx = x1 - x0;
	int dy = y1 - y0;
	int d = std::max(abs(dx), abs(dy));

	float vx = static_cast<float>(dx) / d;
	float vy = static_cast<float>(dy) / d;

	float x = x0;
	float y = y0;
	for (int i = 0; i <= d; i++) {
		SetPixelSafe(static_cast<int>(floor(x)), static_cast<int>(floor(y)), c);
		x += vx;
		y += vy;
	}
}

void Image::DrawCircle(int x, int y, int r, const Color& borderColor,
	int borderWidth, bool isFilled, const Color& fillColor) {

	// Filling the circle
	if (isFilled) {
		for (int i = x - r; i <= x + r; ++i) {
			for (int j = y - r; j <= y + r; ++j) {
				if ((i - x) * (i - x) + (j - y) * (j - y) <= r * r) {
					SetPixel(i, j, fillColor);
				}
			}
		}
	}

	// Drawing the border
	for (int i = 0; i < borderWidth; ++i) {
		for (float angle = 0.0; angle <= 360.0; angle += 0.1) {
			int newX = static_cast<int>(x + r * cos(angle));
			int newY = static_cast<int>(y + r * sin(angle));
			SetPixel(newX, newY, borderColor);
		}
	}
}


void Image::DrawTriangle(const Vector2& p0, const Vector2& p1, const Vector2& p2, const Color& borderColor, bool isFilled, const Color& fillColor) {
	float topPoint = std::max(p0.y, p1.y);
	topPoint = std::max(topPoint, p2.y);
	float minPoint = std::min(p0.y, p1.y);
	minPoint = std::min(minPoint, p2.y);
	int h = static_cast<int>(topPoint - minPoint);

	std::vector<Cell> table;
	h = abs(h);
	table.resize(h);

	for (int i = 0; i < h; ++i) {
		table[i].y = static_cast<int>(minPoint) + i;
	}

	ScanLineDDA(p0.x, p0.y, p1.x, p1.y, table);
	ScanLineDDA(p1.x, p1.y, p2.x, p2.y, table);
	ScanLineDDA(p2.x, p2.y, p0.x, p0.y, table);

	for (int i = 0; i < h; ++i) {
		if (isFilled) {
			for (int x = table[i].minx + 1; x < table[i].maxx; ++x) {
				SetPixelSafe(x, minPoint + i, fillColor);
			}
		}
	}
	DrawLineDDA(p0.x, p0.y, p1.x, p1.y, borderColor);
	DrawLineDDA(p0.x, p0.y, p2.x, p2.y, borderColor);
	DrawLineDDA(p1.x, p1.y, p2.x, p2.y, borderColor);
}

void Image::ScanLineDDA(int x0, int y0, int x1, int y1, std::vector<Cell>& table) {
	int dx = x1 - x0;
	int dy = y1 - y0;
	int d = std::max(std::abs(dx), std::abs(dy));

	float vx = static_cast<float>(dx) / d;
	float vy = static_cast<float>(dy) / d;

	float x = x0;
	float y = y0;

	for (int i = 0; i <= d; ++i) {
		int currentY = static_cast<int>(floor(y));
		for (int j = 0; j < table.size(); ++j) {
			if (table[j].y == currentY) {
				table[j].minx = std::min(table[j].minx, static_cast<int>(floor(x)));
				table[j].maxx = std::max(table[j].maxx, static_cast<int>(floor(x)));
			}
		}
		x += vx;
		y += vy;
	}
}

void Image::DrawImage(const Image& image, int x, int y, bool top)
{
	if (top)
		y = height - y - image.height;

	for (unsigned int i = 0; i < image.width; ++i)
	{
		for (unsigned int j = 0; j < image.height; ++j)
		{
			Color pixelColor = image.GetPixel(i, j);
			SetPixelSafe(x + i, y + j, pixelColor);
		}
	}
}

bool Button::IsMouseInside(const Vector2& mousePosition)
{
	return (
		mousePosition.x >= position.x &&
		mousePosition.x <= position.x + image.width &&
		mousePosition.y >= position.y &&
		mousePosition.y <= position.y + image.height
		);
}

void ParticleSystem::Init() {
	for (int i = 0; i < MAX_PARTICLES; ++i) {
		particles[i].position = Vector2(std::rand() % 1400, std::rand() % 700);
		particles[i].velocity = Vector2((rand() % 5 - 2), (rand() % 5 - 2));
		particles[i].color = Color::WHITE; // White color
		particles[i].acceleration = 0.1f; //Sets the acceleration of the particle
		particles[i].ttl = 100;
		particles[i].inactive = false;
	}
}

void ParticleSystem::Render(Image* framebuffer) {
	for (int i = 0; i < MAX_PARTICLES; ++i) {
		if (!particles[i].inactive) {
			framebuffer->SetPixelSafe(static_cast<int>(particles[i].position.x), static_cast<int>(particles[i].position.y), particles[i].color);
		}
	}
}

void ParticleSystem::Update(float dt) {
	for (int i = 0; i < MAX_PARTICLES; ++i) {
		if (!particles[i].inactive) {
			particles[i].position.x += particles[i].velocity.x * dt;
			particles[i].position.y += particles[i].velocity.y * dt;
			particles[i].velocity.x += particles[i].acceleration * dt;
			particles[i].velocity.y += particles[i].acceleration * dt;
			particles[i].ttl -= dt;

			if (particles[i].ttl <= 0.0f) {
				particles[i].inactive = true;
			}
		}
	}
}

void Image::drawPixel(float x, float y, const sTriangleInfo& triangle, const Vector3& bCoords) { //auxiliar function

	// Use colors!
	if (triangle.texture == nullptr) {
		// Draw pixel in the framebuffer
		Color finalColor = bCoords.x * triangle.c0 + bCoords.y * triangle.c1 + bCoords.z * triangle.c2;
		SetPixelSafe(x, y, finalColor);

	}
	else {
		// Use texture!
		Vector2 uv0_texture = Vector2((triangle.uv0.x * triangle.texture->width - 1), (triangle.uv0.y * triangle.texture->height - 1));
		Vector2 uv1_texture = Vector2((triangle.uv1.x * triangle.texture->width - 1), (triangle.uv1.y * triangle.texture->height - 1));
		Vector2 uv2_texture = Vector2((triangle.uv2.x * triangle.texture->width - 1), (triangle.uv2.y * triangle.texture->height - 1));

		float uv_x = (uv0_texture.x * bCoords.x) + (uv1_texture.x * bCoords.y) + (uv2_texture.x * bCoords.z);
		float uv_y = (uv0_texture.y * bCoords.x) + (uv1_texture.y * bCoords.y) + (uv2_texture.y * bCoords.z);

		Color textureColor = triangle.texture->GetPixelSafe(uv_x, uv_y);

		SetPixelSafe(x, y, textureColor);
	}
}

void Image::DrawTriangleInterpolated(const sTriangleInfo& triangle, FloatImage* zBuffer) {

	float topPoint = std::max(triangle.p0.y, triangle.p1.y);
	topPoint = std::max(topPoint, triangle.p2.y);
	float minPoint = std::min(triangle.p0.y, triangle.p1.y);
	minPoint = std::min(minPoint, triangle.p2.y);
	int h = static_cast<int>(topPoint - minPoint);

	Matrix44 M;
	M.M[0][0] = triangle.p0.x; M.M[0][1] = triangle.p1.x; M.M[0][2] = triangle.p2.x;
	M.M[1][0] = triangle.p0.y; M.M[1][1] = triangle.p1.y; M.M[1][2] = triangle.p2.y;
	M.M[2][0] = 1; M.M[2][1] = 1; M.M[2][2] = 1;
	
	M.Transpose();
	M.Inverse();

	std::vector<Cell> table(h);
	for (int i = 0; i < h; ++i) {
		table[i].y = static_cast<int>(minPoint + i);
	}

	ScanLineDDA(triangle.p0.x, triangle.p0.y, triangle.p1.x, triangle.p1.y, table);
	ScanLineDDA(triangle.p1.x, triangle.p1.y, triangle.p2.x, triangle.p2.y, table);
	ScanLineDDA(triangle.p2.x, triangle.p2.y, triangle.p0.x, triangle.p0.y, table);


	for (int i = 0; i < h; ++i) {
		for (int x = table[i].minx; x <= table[i].maxx; ++x) {
			Vector2 p = Vector2(x, minPoint + i);

			// Calculate barycentric coordinates
			Vector3 bCoords = M * Vector3(p.x, p.y, 1);
			bCoords.Clamp(0,1);


			if (triangle.occlusion) {
				// Interpolate Z value using barycentric coordinates
				float interpolatedZ = bCoords.x * triangle.p0.z + bCoords.y * triangle.p1.z + bCoords.z * triangle.p2.z;

				// Check Z-Buffer for occlusion
				if (interpolatedZ < zBuffer->GetPixel(x, static_cast<unsigned int>(p.y))) {

					zBuffer->SetPixel(x, static_cast<unsigned int>(p.y), interpolatedZ);

					drawPixel(p.x, p.y, triangle, bCoords);
				}
			}
			else {
				drawPixel(p.x, p.y, triangle, bCoords);
			}
		}
	}
}
