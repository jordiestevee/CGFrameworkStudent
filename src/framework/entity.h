#include "mesh.h"
#include "framework.h"
#include "image.h"

#include "main/includes.h"

class Entity {
	Mesh mesh;
	Matrix44 ModelMatrix;
	
public:

	Entity();
	void Entity::Render(Image* framebuffer, Camera* camera, const Color& c);
};