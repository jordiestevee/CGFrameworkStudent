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

	Projection.LoadPNG("images/projector.png");//Button to change perspective
	projectionButton.image = Projection;
	projectionButton.position = Vector2(15, 16);
	framebuffer.DrawImage(Projection, 15, 16, false);

	renderSingleEntity = false;  
	renderMultipleEntities = false;
	isCameraMoving = false;
	isEyeMoving = false;

	// Initialize the entities
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
	entity3.mesh = mesh3;
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

	framebuffer.Render();
}

// Called after render
void Application::Update(float seconds_elapsed)
{
	if (renderMultipleEntities) {
		entity.Update(seconds_elapsed);
		entity2.Update(seconds_elapsed);
		entity3.Update(seconds_elapsed);

	}
	framebuffer.Fill(Color(0, 0, 0));
	framebuffer.DrawImage(Projection, 15, 16, false);
}


//keyboard press event 
void Application::OnKeyPressed(SDL_KeyboardEvent event)
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
	switch (event.keysym.sym) {
	case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
	case SDLK_PLUS:
		if (isNear == true) { //Increase CURRENT PROPERTY	
			camera.near_plane += 1.0f;
		}
		else camera.far_plane += 1.0f;
		break;
	case SDLK_MINUS: // Decrease CURRENT PROPERTY
		if (isNear == true) {
			camera.near_plane -= 1.0f;
		}
		else camera.far_plane -= 1.0f;
		break;
	case SDLK_1: // Draw SINGLE ENTITY
		renderSingleEntity = !renderSingleEntity;  
		break;
	case SDLK_2: // Draw MULTIPLE ANIMATED ENTITIES
		renderMultipleEntities = !renderMultipleEntities;  
		break;
	case SDLK_o: // Set ORTHOGRAPHIC camera mode
		camera.SetOrthographic(camera.left, camera.right, camera.top, camera.bottom, camera.near_plane, camera.far_plane);
		break;
	case SDLK_p: // Set PERSPECTIVE camera mode
		camera.SetPerspective(camera.fov, camera.aspect, camera.near_plane, camera.far_plane);
		break;
	case SDLK_n: //Set current property to CAMERA NEAR
		isNear = true;
		break;
	case SDLK_f: // Set current property to CAMERA FAR
		isNear = true;
		break;
	case SDLK_DOWN: // Decrease FOV
		camera.fov -= 5.0f;
		camera.UpdateProjectionMatrix();
		break;
	case SDLK_UP: // Increase FOV
		camera.fov += 5.0f;
		camera.UpdateProjectionMatrix();
		break;
	}
}

void Application::OnMouseButtonDown(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_LEFT) {
		isEyeMoving = true;
		startX = event.x;
		startY = event.y;
	}
	if (event.button == SDL_BUTTON_RIGHT) {
		isCameraMoving = true;
		startX = event.x;
		startY = event.y;
	}
}

void Application::OnMouseButtonUp(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_LEFT) {
		if (projectionButton.IsMouseInside(Vector2(mouse_position.x, mouse_position.y))) { // Change projection if the projection button is  clicked
			if (camera.type == camera.ORTHOGRAPHIC) {
				camera.SetPerspective(camera.fov, camera.aspect, camera.near_plane, camera.far_plane);
			}
			else camera.SetOrthographic(camera.left, camera.right, camera.top, camera.bottom, camera.near_plane, camera.far_plane);
		}
		isEyeMoving = false;
	}
	if (event.button == SDL_BUTTON_RIGHT) {
		isCameraMoving = false;
	}
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
	if (isEyeMoving) {
		int mouse_distanceX = event.x - startX;
		int mouse_distanceY = event.y - startY;

		float sens = 0.002f;

		camera.Orbit(sens * mouse_distanceX, mouse_distanceY * sens);
	}

	if (isCameraMoving) {
		int mouse_distanceX = event.x - startX;
		int mouse_distanceY = event.y - startY;

		float sensXY = 0.01f;

		camera.MoveCenter(mouse_distanceX* sensXY, mouse_distanceY*sensXY, 0.0f);// no movement on z

		startX = event.x;
		startY = event.y;
	}
}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
	float dy = event.preciseY;
}

void Application::OnFileChanged(const char* filename)
{
	Shader::ReloadSingleShader(filename);
}