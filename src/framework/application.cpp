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

	Projection.LoadPNG("images/projector.png");
	projectionButton.image = Projection;
	projectionButton.position = Vector2(15, 16);
	framebuffer.DrawImage(Projection, 15, 16, false);

	renderSingleEntity = false;  
	renderMultipleEntities = false;
	useOrthographicCamera = false;
	usePerspectiveCamera = false;


	Mesh mesh1;
	mesh1.LoadOBJ("meshes/lee.obj");
	entity.mesh = mesh1;
	entity.ModelMatrix.TranslateLocal(-1, 0, 0);
	entity.scale = Vector3(1.01, 1.01, 1.01);

	Mesh mesh2;
	mesh2.LoadOBJ("meshes/anna.obj");
	entity2.mesh = mesh2;
	entity2.rotation = Vector4(0, 1, 0, PI / 128);

	Mesh mesh3;
	mesh3.LoadOBJ("meshes/cleo.obj");
	entity3.mesh = mesh2;
	entity3.ModelMatrix.TranslateLocal(1, 0, 0);
	entity3.translate = Vector3(-0.01, -0.01, 0);

	camera.SetPerspective(60, framebuffer.width / (float)framebuffer.height, 0.01f, 100.0f);
	camera.LookAt(Vector3(0, 0.5, 1.5), Vector3(0,0,0), Vector3(0, 1, 0));
}

// Render one frame
void Application::Render(void)
{
	// ...
	if (renderSingleEntity) {
		entity.Render(&framebuffer, &camera, Color::BLUE);
	}

	if (renderMultipleEntities) {
		entity.Render(&framebuffer, &camera, Color::BLUE);
		entity2.Render(&framebuffer, &camera, Color::RED);
		entity3.Render(&framebuffer, &camera, Color::WHITE);
	}

	if (useOrthographicCamera) {
		camera.SetOrthographic(-5.0f, 5.0f, -5.0f, 5.0f, 0.01f, 100.0f);
	}

	if (usePerspectiveCamera) { 
		camera.SetPerspective(60, framebuffer.width / (float)framebuffer.height, 0.01f, 100.0f);
	}

	framebuffer.Render();
}

// Called after render
void Application::Update(float seconds_elapsed)
{
	if (renderMultipleEntities) {
		entity.Update(1);
		entity2.Update(1);
		entity3.Update(1);
		framebuffer.DrawRect(0, 0, framebuffer.width, framebuffer.height, Color::BLACK, 1, TRUE, Color::BLACK);
		framebuffer.DrawImage(Projection, 15, 16, false);
	}

}


//keyboard press event 
void Application::OnKeyPressed(SDL_KeyboardEvent event)
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
	switch (event.keysym.sym) {
	case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
	//if (camera::PERSPECTIVE)
	case SDLK_PLUS:
		camera.fov += 5.0f;
		camera.UpdateProjectionMatrix();
		break;
	case SDLK_MINUS:
		camera.fov -= 5.0f;
		camera.UpdateProjectionMatrix();
		break;
	case SDLK_n:
		if (SDLK_PLUS) {
			camera.near_plane += 100.0f;
			camera.UpdateProjectionMatrix();
			break;
		}

	case SDLK_1: // Draw SINGLE ENTITY
		renderSingleEntity = !renderSingleEntity;  
		break;

	case SDLK_2: // Draw MULTIPLE ANIMATED ENTITIES
		renderMultipleEntities = !renderMultipleEntities;  
		break;

	case SDLK_o: // Set ORTHOGRAPHIC camera mode
		useOrthographicCamera = !useOrthographicCamera;  
		break;

	case SDLK_p: // Set PERSPECTIVE camera mode
		usePerspectiveCamera = !usePerspectiveCamera;  
		break;

	//case SDLK_n && SDLK_MINUS:
		//camera.near_plane += 5.0f;
	//case (SDLK_f && SDLK_PLUS):
		//camera.far_plane += 5.0f;
	}
}

void Application::OnMouseButtonDown(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_LEFT) {

	}
	if (event.button == SDL_BUTTON_RIGHT) {
		isCameraMoving = TRUE;
		startX = mouse_position.x;
		startY = mouse_position.y;
	}
}

void Application::OnMouseButtonUp(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_LEFT) {
		if (projectionButton.IsMouseInside(Vector2(mouse_position.x, mouse_position.y))) {
			if (camera.type == camera.ORTHOGRAPHIC) {
				camera.SetPerspective(camera.fov, camera.aspect, camera.near_plane, camera.far_plane);
			}
			else camera.SetOrthographic(camera.left, camera.right, camera.top, camera.bottom, camera.near_plane, camera.far_plane);
		}
	}
	if (event.button == SDL_BUTTON_RIGHT) {
		isCameraMoving = FALSE;
	}
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_RIGHT) {

		int mouse_distanceX = mouse_position.x - startX;
		int mouse_distanceY = mouse_position.y - startY;
		camera.LookAt(camera.eye, Vector3(camera.center.x - mouse_distanceX, camera.center.y - mouse_distanceY, camera.center.z), camera.up);
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