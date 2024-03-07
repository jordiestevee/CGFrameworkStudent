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

	Task = 0;

	// Set camera
	camera.SetPerspective(60, framebuffer.width / (float)framebuffer.height, 0.01f, 100.0f);
	camera.LookAt(Vector3(0.0 , 0 , 1), Vector3(0,0,0), Vector3(0, 1, 0));
	camera.UpdateViewProjectionMatrix();
	
	// Load mesh
	myMesh.LoadOBJ("meshes/lee.obj");

	// Load shaders
	myShader1 = new Shader();
	myShader1 = Shader::Get("shaders/phong.vs", "shaders/phong.fs");
	myShader2 = new Shader();
	myShader2 = Shader::Get("shaders/gouraud.vs", "shaders/gouraud.fs");

	// Load textures
	texture = new Texture();
	texture->Load("textures/lee_color_specular.tga");
	texture2 = new Texture();
	texture2->Load("textures/lee_normal.tga");

	// Create material
	material = new Material(myShader1, texture, texture2, Vector3(1, 1, 1), Vector3(1, 1, 1), Vector3(1, 1, 1), 5);
	material2 = new Material(myShader2, texture, texture2, Vector3(1, 1, 1), Vector3(1, 1, 1), Vector3(1, 1, 1), 5);
	
	// Load entities
	entity.mesh = myMesh;
	entity.material = material;

	entity2.mesh = myMesh;
	entity2.material = material2;

	// Create lights
	lights[0].Is.Set(1, 1, 1);
	lights[0].Id.Set(1, 1, 1);
	lights[0].position.Set(1, 1, 1);
	
	lights[1].Is.Set(1, 1, 1);
	lights[1].Id.Set(1, 0, 0);
	lights[1].position.Set(-5, 0, 0);

	lights[2].Is.Set(1, 1, 1);
	lights[2].Id.Set(0, 0, 1);
	lights[2].position.Set(0, 5, 0);

	// Set data
	data.numLights = 1;
	data.lights[0] = lights[0]; // White on the right
	data.lights[1] = lights[1]; // Red on the left
	data.lights[2] = lights[2]; // Blue on top
	data.viewProjectionMatrix = camera.viewprojection_matrix;
	data.cameraPosition = camera.eye;
	Ia.Set(0.1, 0.1, 0.1);
	data.Ia = Ia;
	data.flag = Vector3(0.0, 0.0, 0.0);
}

// Render one frame
void Application::Render(void)
{
	// ...
	if (Task == 1) {
		entity.Render(data);
	}
	if (Task == 2) {
		entity2.Render(data);
	}
}

// Called after render
void Application::Update(float seconds_elapsed)
{
}


//keyboard press event 
void Application::OnKeyPressed(SDL_KeyboardEvent event)
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
	switch (event.keysym.sym) {
	case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
	case SDLK_g:
		Task = 2;
		data.numLights = 1;
		break;
	case SDLK_p:
		Task = 1;
		break;
	case SDLK_c:
		if (Task == 1) {
			if (data.flag.x == 1.0){
			data.flag.x = 0.0;
			}else data.flag.x = 1.0;
		}
		break;
	case SDLK_s:
		if (Task == 1) {
			if (data.flag.y == 1.0) {
				data.flag.y = 0.0;
			} else data.flag.y = 1.0;
		}
		break;
	case SDLK_n:
		if (Task == 1) {
			if (data.flag.z == 1.0) {
				data.flag.z = 0.0;
			} else data.flag.z = 1.0;
		}
		break;
	//lights
	case SDLK_1:
		if (Task == 1) {
			data.numLights = 1;
		}
		break;
	case SDLK_2: 
		if (Task == 1) {
			data.numLights = 2;
		}
		break;
	case SDLK_3:
		if (Task == 1) {
			data.numLights = 3;
		}
		break;
	}
}

void Application::OnMouseButtonDown(SDL_MouseButtonEvent event)
{

}

void Application::OnMouseButtonUp(SDL_MouseButtonEvent event)
{

}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_LEFT) {
		camera.Orbit(-mouse_delta.x * 0.01, Vector3::UP);
		camera.Orbit(-mouse_delta.y * 0.01, Vector3::RIGHT);
	}

}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
	float dy = event.preciseY;	
	camera.Zoom(dy < 0 ? 1.1 : 0.9);
}

void Application::OnFileChanged(const char* filename)
{
	Shader::ReloadSingleShader(filename);
}