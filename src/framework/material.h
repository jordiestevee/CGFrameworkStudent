#ifndef material_h
#define material_h


#include "framework.h"
#include "shader.h"
#include "texture.h"
#include "main/includes.h"

struct sLight {
	Vector3 position;
	Vector3 Id;
	Vector3 Is;
};

struct sUniformData {
	Matrix44 modelMatrix;
	Matrix44 viewProjectionMatrix;
	Vector3 Ia;
	Vector3 cameraPosition;
	sLight light;
	Vector2 flag;
};

class Material {
public:
	Shader* shader;
	Texture* colorTexture;
	Texture* normalTexture;
	Vector3 Ka; 
	Vector3 Kd; 
	Vector3 Ks;
	float shinninnes;

	Material(Shader* shader, Texture* colorTexture, Texture* normalTexture, Vector3 Ka, Vector3 Kd, Vector3 ks, float alpha);

	void Enable();
	void Disable();
	void Enable(const sUniformData& uniformData);
};

#endif 
