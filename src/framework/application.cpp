#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 

Application::Application(const char* caption, int width, int height)
{
	this->window = createWindow(caption, width, height);

	int w,h;
	SDL_GetWindowSize(window,&w,&h);

	this->mouse_state = 0;
	this->time = 0.f;
	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(nullptr);

	this->framebuffer.Resize(w, h);

}

Application::~Application()
{
}

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;

	borderWidth = 1;
	Color paintColor = Color::BLUE;


	Image toolbar;
	toolbar.LoadPNG("images/toolbar.png");
	framebuffer.DrawImage(toolbar, 0, 0, false);

	Image clear;
	clear.LoadPNG("images/clear.png");
	
	clearButton.image = clear;
	clearButton.position = Vector2(15, 16);
	framebuffer.DrawImage(clear, 15, 16, false);


	Image load;
	load.LoadPNG("images/load.png");
	
	loadButton.image = load;
	loadButton.position = Vector2(65, 16);
	framebuffer.DrawImage(load, 65, 16, false);

	Image save;
	save.LoadPNG("images/save.png");
	Button saveButton;
	saveButton.image = save;
	saveButton.position = Vector2(115, 16);
	framebuffer.DrawImage(save, 115, 16, false);

	Image eraser;
	eraser.LoadPNG("images/eraser.png");
	
	eraserButton.image = eraser;
	eraserButton.position = Vector2(165, 16);
	framebuffer.DrawImage(eraser, 165, 16, false);

	Image black;
	black.LoadPNG("images/black.png");
	blackButton.image = black;
	blackButton.position = Vector2(215, 16);
	framebuffer.DrawImage(black, 215, 16, false);

	Image red;
	red.LoadPNG("images/red.png");
	redButton.image = red;
	redButton.position = Vector2(265, 16);
	framebuffer.DrawImage(red, 265, 16, false);

	Image green;
	green.LoadPNG("images/green.png");
	greenButton.image = green;
	greenButton.position = Vector2(315, 16);
	framebuffer.DrawImage(green, 315, 16, false);

	Image blue;
	blue.LoadPNG("images/blue.png");
	blueButton.image = blue;
	blueButton.position = Vector2(365, 16);
	framebuffer.DrawImage(blue, 365, 16, false);

	Image pink;
	pink.LoadPNG("images/pink.png");
	pinkButton.image = pink;
	pinkButton.position = Vector2(415, 16);
	framebuffer.DrawImage(pink, 415, 16, false);

	Image cyan;
	cyan.LoadPNG("images/cyan.png");
	cyanButton.image = cyan;
	cyanButton.position = Vector2(465, 16);
	framebuffer.DrawImage(cyan, 465, 16, false);

	Image line;
	line.LoadPNG("images/line.png");
	lineButton.image = line;
	lineButton.position = Vector2(515, 16);
	framebuffer.DrawImage(line, 515, 16, false);

	Image rectangle;
	rectangle.LoadPNG("images/rectangle.png");
	rectangleButton.image = rectangle;
	rectangleButton.position = Vector2(565, 16);
	framebuffer.DrawImage(rectangle, 565, 16, false);

	Image circle;
	circle.LoadPNG("images/circle.png");
	circleButton.image = circle;
	circleButton.position = Vector2(615, 16);
	framebuffer.DrawImage(circle, 615, 16, false);

	Image triangle;
	triangle.LoadPNG("images/triangle.png");
	triangleButton.image = triangle;
	triangleButton.position = Vector2(665, 16);
	framebuffer.DrawImage(triangle, 665, 16, false);

	ParticleSystem* particlesystem = new ParticleSystem();

	particleSystem.Init();
}

// Render one frame
void Application::Render(void)
{
	// ...
	/*if (isAnimation) {
		ParticleSystem->Init();
		ParticleSystem->Render(&framebuffer);
	}*/
	particleSystem.Render(&framebuffer);
	
framebuffer.Render();
}

// Called after render
void Application::Update(float seconds_elapsed)
{
	particleSystem.Update(0.1f);
}

//keyboard press event 
void Application::OnKeyPressed(SDL_KeyboardEvent event)
{	
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
	switch (event.keysym.sym) {

	case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
	case SDLK_PLUS:
	case SDLK_KP_PLUS:
		borderWidth++;
		break;
	case SDLK_MINUS:
	case SDLK_KP_MINUS:
		if (borderWidth > 1) {
			borderWidth--;
		}
		break;
	case SDLK_1:
	case SDLK_KP_1:
		// Draw Lines
		framebuffer.DrawLineDDA(mouse_position.x, mouse_position.y, mouse_position.x + 50, mouse_position.y + 50, paintColor);
		break;
	case SDLK_2:
		// Draw Rectangles
		framebuffer.DrawRect(mouse_position.x, mouse_position.y, 60, 40, Color::GREEN, borderWidth, fillShapes, paintColor);
		break;

	case SDLK_3:
		// Draw Circles
		framebuffer.DrawCircle(mouse_position.x, mouse_position.y, 25, Color::BLUE, borderWidth, fillShapes, paintColor);
		break;

	case SDLK_4:
		// Draw Triangles
		framebuffer.DrawTriangle(Vector2(mouse_position.x, mouse_position.y), Vector2(mouse_position.x + 50, mouse_position.y), Vector2(mouse_position.x + 25, mouse_position.y + 50)
		, Color::CYAN, fillShapes, paintColor);
		break;

	case SDLK_5:
		// Paint
		isPainting = !isPainting; //press 5 to start painting and 5 again to stop
		drawLine = false;
		drawRect = false;
		drawCircle = false;
		drawtriangle = false;
		isErasing = false;

		break;

	case SDLK_6:
		// Animation
		isAnimation = !isAnimation; 
		break;

	case SDLK_f:
		// Fill Shapes
		fillShapes = !fillShapes;
		break;
	}
}

void Application::OnMouseButtonDown(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_LEFT) {

		// Save the starting position
		startX = mouse_position.x;
		startY = mouse_position.y;
		if (isPainting) {
			// Paint at the current mouse position
			framebuffer.DrawRect(mouse_position.x, mouse_position.y, 5, 5, paintColor, 1, true, paintColor);
		}

		if (isErasing) {
			framebuffer.DrawRect(mouse_position.x, mouse_position.y, 10,10, Color::BLACK, 1, true ,Color::BLACK);
		}
	}
}

void Application::OnMouseButtonUp(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_LEFT) {

		if (blackButton.IsMouseInside(Vector2(mouse_position.x, mouse_position.y))) {
			paintColor = Color::BLACK;
		}
		if (redButton.IsMouseInside(Vector2(mouse_position.x, mouse_position.y))) {
			paintColor = Color::RED;
		}
		if (greenButton.IsMouseInside(Vector2(mouse_position.x, mouse_position.y))) {
			paintColor = Color::GREEN;
		}
		if (blueButton.IsMouseInside(Vector2(mouse_position.x, mouse_position.y))) {
			paintColor = Color::BLUE;
		}
		if (pinkButton.IsMouseInside(Vector2(mouse_position.x, mouse_position.y))) {
			paintColor = Color(255, 192, 203);
		}
		if (cyanButton.IsMouseInside(Vector2(mouse_position.x, mouse_position.y))) {
			paintColor = Color::CYAN;
		}

		if (drawLine) {
			if ((startX == mouse_position.x && startY == mouse_position.y) == false) {
				framebuffer.DrawLineDDA(startX, startY, mouse_position.x, mouse_position.y, paintColor);
			}
		}
		if (lineButton.IsMouseInside(Vector2(mouse_position.x, mouse_position.y))) {
			drawLine = true;
			drawRect = false;
			drawCircle = false;
			drawtriangle = false;
			isPainting = false;
			isErasing = false;

		}

		if (drawRect) {
			if ((startX == mouse_position.x && startY == mouse_position.y) == false) {

				int width = abs(mouse_position.x - startX);
				int height = abs(mouse_position.y - startY);

				framebuffer.DrawRect(startX, startY, width, height, paintColor, borderWidth, fillShapes, Color::GREEN);
			}
		}
		if (rectangleButton.IsMouseInside(Vector2(mouse_position.x, mouse_position.y))) {
			drawRect = true;
			drawLine = false;
			drawCircle = false;
			drawtriangle = false;
			isPainting = false;
			isErasing = false;

		}
		
		if (drawCircle) {
			if ((startX == mouse_position.x && startY == mouse_position.y) == false) {
				Vector2 p0(startX, startY);
				Vector2 p1(mouse_position.x, mouse_position.y);
				float r = std::sqrt(std::pow(abs(p1.x - p0.x), 2) + std::pow(abs(p1.y - p0.y), 2)) / 2;

				framebuffer.DrawCircle(startX, startY, r, paintColor, borderWidth, fillShapes, Color::BLUE);
			}
		}
		if (circleButton.IsMouseInside(Vector2(mouse_position.x, mouse_position.y))) {
			drawCircle = true;
			drawLine = false;
			drawRect = false;
			drawtriangle = false;
			isPainting = false;
			isErasing = false;

		}


		if (drawtriangle) {
			if ((startX == mouse_position.x && startY == mouse_position.y) == false) {
				Vector2 p0(startX, startY);
				Vector2 p1(startX + 2 * (abs(mouse_position.x - startX)), startY);
				Vector2 p2(mouse_position.x, mouse_position.y);

				framebuffer.DrawTriangle(p0, p1, p2, paintColor, fillShapes, Color::CYAN);
			}
		}
		if (triangleButton.IsMouseInside(Vector2(mouse_position.x, mouse_position.y))) {
			drawtriangle = true;
			drawLine = false;
			drawRect = false;
			drawCircle = false;
			isPainting = false;
			isErasing = false;
		}
		if (eraserButton.IsMouseInside(Vector2(mouse_position.x, mouse_position.y))) {
			isErasing = true;
			drawtriangle = false;
			drawLine = false;
			drawRect = false;
			drawCircle = false;
			isPainting = false;
		}

		if (clearButton.IsMouseInside(Vector2(mouse_position.x, mouse_position.y))) {
			framebuffer.DrawRect(0, 0, window_width, window_height, Color::BLACK, 0, true, Color::BLACK);
			Init();
		}
	}
}	

void Application::OnMouseMove(SDL_MouseButtonEvent event){
	if (isPainting && (event.button == SDL_BUTTON_LEFT)) {
		// Paint at the current mouse position
		framebuffer.DrawRect(mouse_position.x, mouse_position.y, 5, 5, paintColor, 1, true, paintColor);
	}
	if (isErasing && (event.button == SDL_BUTTON_LEFT)) {
		framebuffer.DrawRect(mouse_position.x, mouse_position.y, 10, 10, Color::BLACK, 1, true, Color::BLACK);
	}
}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
	float dy = event.preciseY;

	// ...
}

void Application::OnFileChanged(const char* filename)
{ 
	Shader::ReloadSingleShader(filename);
}