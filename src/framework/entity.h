#include "main/includes.h"
#include "mesh.h"
#include "framework.h"
#include "image.h"
#include "camera.h"

enum class eRenderMode {
	PLAIN_COLOR, // color
	INTERPOLATED, // mesh
	OCCLUSION,
	TEXTURE 
};

class Entity {

	
public:
	Entity();
	void Entity::Render(Image* framebuffer, Camera* camera, Color c, FloatImage* zBuffer);
	void Entity::Update(float seconds_elapsed);
	Mesh mesh;
	Matrix44 ModelMatrix;
	Image* texture;
	Vector4 rotation = Vector4(0, 0, 0, 0); // w component is the angle
	Vector3 scale = Vector3(1, 1, 1);
	Vector3 translate = Vector3(0, 0, 0);
	int i = 0;
	Image::sTriangleInfo triangleInfo;
	eRenderMode mode = eRenderMode::TEXTURE;
};