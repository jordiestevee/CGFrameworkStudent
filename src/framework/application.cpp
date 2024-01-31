#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 


Application::Application(const char* caption, int width, int height)
{
	this->window = createWindow(caption, width, height);

	int w, h;
	SDL_GetWindowSize(window, &w, &h);

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
	Mesh mesh1;
	mesh1.LoadOBJ("meshes/lee.obj");
	entity.mesh = mesh1;
	//entity.rotation = Vector4(0, 1, 0, PI / 128);
	//entity.translate = Vector3(0.1, 0, 0);

	Mesh mesh2;
	mesh2.LoadOBJ("meshes/anna.obj");
	entity2.mesh = mesh2;

	camera.SetPerspective(45, framebuffer.width / (float)framebuffer.height, 0.01f, 1000.0f);
	camera.LookAt(Vector3(1, 1, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));
}

// Render one frame
void Application::Render(void)
{
	// ...

	entity.Render(&framebuffer, &camera, Color::BLUE);
	entity.ModelMatrix.Translate(0.1, 0, 0);



	framebuffer.Render();
}

// Called after render
void Application::Update(float seconds_elapsed)
{
	//framebuffer.DrawRect(0, 0, framebuffer.width, framebuffer.height, Color::BLACK, 1, TRUE, Color::BLACK);
	//entity.Update(1);
}


//keyboard press event 
void Application::OnKeyPressed(SDL_KeyboardEvent event)
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
	switch (event.keysym.sym) {
	case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
	}
}

void Application::OnMouseButtonDown(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_LEFT) {

	}
}

void Application::OnMouseButtonUp(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_LEFT) {

	}
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{

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