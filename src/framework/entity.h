#include "main/includes.h"
#include "mesh.h"
#include "framework.h"
#include "image.h"



class Entity {

	
public:
	Entity();
	void Entity::Render(Image* framebuffer, Camera* camera, const Color& c);
	Mesh mesh;
	Matrix44 ModelMatrix;
};