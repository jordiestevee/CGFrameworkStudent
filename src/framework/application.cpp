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
	Button clearButton;
	clearButton.image = clear;
	clearButton.position = Vector2(15, 16);
	framebuffer.DrawImage(clear, 15, 16, false);


	Image load;
	load.LoadPNG("images/load.png");
	Button loadButton;
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
	Button eraserButton;
	eraserButton.image = eraser;
	eraserButton.position = Vector2(165, 16);
	framebuffer.DrawImage(eraser, 165, 16, false);

	Image black;
	black.LoadPNG("images/black.png");
	Button blackButton;
	blackButton.image = black;
	blackButton.position = Vector2(215, 16);
	framebuffer.DrawImage(black, 215, 16, false);

	Image red;
	red.LoadPNG("images/red.png");
	Button redButton;
	redButton.image = red;
	redButton.position = Vector2(265, 16);
	framebuffer.DrawImage(red, 265, 16, false);

	Image green;
	green.LoadPNG("images/green.png");
	Button greenButton;
	greenButton.image = green;
	greenButton.position = Vector2(315, 16);
	framebuffer.DrawImage(green, 315, 16, false);

	Image blue;
	blue.LoadPNG("images/blue.png");
	Button blueButton;
	blueButton.image = blue;
	blueButton.position = Vector2(365, 16);
	framebuffer.DrawImage(blue, 365, 16, false);

	Image pink;
	pink.LoadPNG("images/pink.png");
	Button pinkButton;
	pinkButton.image = pink;
	pinkButton.position = Vector2(415, 16);
	framebuffer.DrawImage(pink, 415, 16, false);

	Image cyan;
	cyan.LoadPNG("images/cyan.png");
	Button cyanButton;
	cyanButton.image = cyan;
	cyanButton.position = Vector2(465, 16);
	framebuffer.DrawImage(cyan, 465, 16, false);

	Image line;
	line.LoadPNG("images/line.png");
	Button lineButton;
	lineButton.image = line;
	lineButton.position = Vector2(515, 16);
	framebuffer.DrawImage(line, 515, 16, false);

	Image rectangle;
	rectangle.LoadPNG("images/rectangle.png");
	Button rectangleButton;
	rectangleButton.image = rectangle;
	rectangleButton.position = Vector2(565, 16);
	framebuffer.DrawImage(rectangle, 565, 16, false);

	Image circle;
	circle.LoadPNG("images/circle.png");
	Button circleButton;
	circleButton.image = circle;
	circleButton.position = Vector2(615, 16);
	framebuffer.DrawImage(circle, 615, 16, false);

	Image triangle;
	triangle.LoadPNG("images/triangle.png");
	triangleButton.image = triangle;
	triangleButton.position = Vector2(665, 16);
	framebuffer.DrawImage(triangle, 665, 16, false);
}

// Render one frame
void Application::Render(void)
{
	// ...
	
framebuffer.Render();
}

// Called after render
void Application::Update(float seconds_elapsed)
{
	/*if (triangleButton.IsMouseInside(Vector2(mouse_position.x, mouse_position.y))) {
		drawtriangle = true;
	}*/
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
		if (borderWidth > 0) {
			borderWidth--;
		}
		break;
	case SDLK_1:
	case SDLK_KP_1:
		framebuffer.DrawLineDDA(mouse_position.x, mouse_position.y, mouse_position.x + 50, mouse_position.y + 50, Color::RED);
		break;
	case SDLK_2:
		framebuffer.DrawRect(mouse_position.x, mouse_position.y, 60, 40, Color::RED, borderWidth, false, Color::WHITE);
		break;

	case SDLK_3:
		// Draw Circles
		// Implement your logic for drawing circles here
		break;

	case SDLK_4:
		framebuffer.DrawTriangle(Vector2(mouse_position.x, mouse_position.y), Vector2(mouse_position.x + 50, mouse_position.y), Vector2(mouse_position.x + 25, mouse_position.y + 50)
		,Color::WHITE, false, Color::GREEN);
		break;

	case SDLK_5:
		// Paint
		isPainting = !isPainting; //press 5 to start painting and 5 again to stop
		break;

	case SDLK_6:
		// Animation
		// Implement your logic for animation here
		break;

	case SDLK_f:
		// Fill Shapes
		// Implement your logic for filling shapes here
		break;
	case  SDL_BUTTON_LEFT:
		if (triangleButton.IsMouseInside(Vector2(mouse_position.x, mouse_position.y))) {
			drawtriangle = true;
		}
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
			framebuffer.SetPixelSafe(mouse_position.x, mouse_position.y, Color::BLUE);
		}
	}


	

}

void Application::OnMouseButtonUp(SDL_MouseButtonEvent event)
{
	/*if (isDrawing) {
		// Check if the left mouse button is still pressed
		// This additional check prevents drawing when the mouse is moved without the button being pressed
			// Calculate the width and height
		int width = mouse_position.x - startX;
		int height = mouse_position.y - startY;

		framebuffer.DrawRect(startX, startY, width, height, Color::RED, borderWidth, true, Color::GREEN);
		isDrawing = false;
	}*/
	if (event.button == SDL_BUTTON_LEFT) {
		if (drawtriangle) {
			/*Vector2 p0(startX, startY);
			Vector2 p1(mouse_position.x, startY);
			Vector2 p2(mouse_position.x, mouse_position.y);

			framebuffer.DrawTriangle(p0, p1, p2, Color::RED, false, Color::RED);*/
			framebuffer.DrawTriangle(Vector2(200,200), Vector2(200, 400), Vector2(400, 200), Color::RED, false, Color::RED); 


			drawtriangle = false;
		}

	}




}	

void Application::OnMouseMove(SDL_MouseButtonEvent event){
	if (isPainting && (event.button == SDL_BUTTON_LEFT)) {
		// Paint at the current mouse position
		framebuffer.SetPixelSafe(mouse_position.x, mouse_position.y, Color::BLUE);

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