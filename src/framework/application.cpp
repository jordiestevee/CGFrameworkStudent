#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 


Application::Application(const char* caption, int width, int height)
{

	this->window = createWindow(caption, width, height);
	rotationAngle = 0.2 * PI;
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

	isCameraMoving = false;

	Task = 0;
	subTask = 0;

	camera.SetPerspective(60, framebuffer.width / (float)framebuffer.height, 0.01f, 100.0f);
	camera.LookAt(Vector3(0 , 0 , 1), Vector3(0,0,0), Vector3(0, 1, 0));

	myMesh.LoadOBJ("meshes/lee.obj");
	myShader1 = new Shader();
	myShader1 = Shader::Get("shaders/gouraud.vs", "shaders/gouraud.fs");
	//myShader1 = Shader::Get("shaders/phong.vs", "shaders/phong.fs");
	if (myShader1 != nullptr) {
		// Shader loaded successfully
		std::cout << "Shader loaded successfully!" << std::endl;
	}
	else {
		// Shader failed to load
		std::cout << "Failed to load shader!" << std::endl;
	}
	texture = new Texture();
	texture->Load("textures/lee_color_specular.tga");
	texture2 = new Texture();
	texture2->Load("textures/lee_normal.tga");

	material = new Material(myShader1, texture, texture2, Vector3(1, 1, 1), Vector3(1, 1, 1), Vector3(1, 1, 1), 30);

	entity.mesh = myMesh;
	entity.material = material;

	light1.Is.Set(1, 1, 1);
	light1.Id.Set(1, 1, 1);
	light1.position.Set(1, 1, 1);

	data.light = light1;
	data.viewProjectionMatrix = camera.viewprojection_matrix;
	data.cameraPosition = camera.eye;

	Ia.Set(0.1, 0.1, 0.1);
	data.Ia = Ia;
	data.flag = Vector2(1.0, 1.0);
}

// Render one frame
void Application::Render(void)
{
	// ...
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	entity.Render(data);

	glDisable(GL_DEPTH_TEST);

}

// Called after render
void Application::Update(float seconds_elapsed)
{
	entity.Render(data);

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
	case SDLK_1:
		subTask = 0;
		Task = 1;
		break;
	case SDLK_2: 
		subTask = 0;
		Task = 2;
		break;
	case SDLK_3:
		subTask = 0;
		Task = 3;
		break;
	case SDLK_4:
		Task = 4;
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
	/*case SDLK_f: // Set current property to CAMERA FAR
		isNear = true;
		break;*/
	case SDLK_DOWN: // Decrease FOV
		camera.fov -= 5.0f;
		camera.UpdateProjectionMatrix();
		break;
	case SDLK_UP: // Increase FOV
		camera.fov += 5.0f;
		camera.UpdateProjectionMatrix();
		break;
	case SDLK_a:
		subTask = 1;
		break;
	case SDLK_b:
		subTask = 2;
		break;
	case SDLK_c:
		subTask = 3;
		break;
	case SDLK_d:
		subTask = 4;
		break;
	case SDLK_e:
		subTask = 5;
		break;
	case SDLK_f:
		subTask = 6;
		break;

	case SDLK_z:
		entity.triangleInfo.occlusion = !entity.triangleInfo.occlusion;
		entity2.triangleInfo.occlusion = !entity2.triangleInfo.occlusion;
		entity3.triangleInfo.occlusion = !entity3.triangleInfo.occlusion;
		break;
	
	case SDLK_t:
		if (entity.mode == eRenderMode::TEXTURE) {
			entity.mode = eRenderMode::PLAIN_COLOR;
			entity2.mode = eRenderMode::PLAIN_COLOR;
			entity3.mode = eRenderMode::PLAIN_COLOR;
		}
		else {
			entity.mode = eRenderMode::TEXTURE;
			entity2.mode = eRenderMode::TEXTURE;
			entity3.mode = eRenderMode::TEXTURE;
		}
		break;
	}
}

void Application::OnMouseButtonDown(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_LEFT) {
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
	}
	if (event.button == SDL_BUTTON_RIGHT) {
		isCameraMoving = false;
	}
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_LEFT) {
		camera.Orbit(-mouse_delta.x * 0.01, Vector3::UP);
		camera.Orbit(-mouse_delta.y * 0.01, Vector3::RIGHT);
	}

	if (isCameraMoving) {
		int mouse_distanceX = event.x - startX;
		int mouse_distanceY = event.y - startY;

		float sensXY = 0.01f;

		camera.MoveCenter(mouse_distanceX* sensXY, mouse_distanceY*sensXY, 0.0f);

		startX = event.x;
		startY = event.y;
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