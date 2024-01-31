#include "main/includes.h"
#include "mesh.h"
#include "framework.h"
#include "image.h"
#include "camera.h"



class Entity {

	
public:
	Entity();
	void Entity::Render(Image* framebuffer, Camera* camera, const Color& c);
	void Entity::Update(float seconds_elapsed);
	Mesh mesh;
	Matrix44 ModelMatrix;
	Vector4 rotation = Vector4(0, 0, 0, 0);
	Vector3 scale = Vector3(0, 0, 0);
	Vector3 translate = Vector3(0, 0, 0);
};