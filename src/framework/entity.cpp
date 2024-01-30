#include "entity.h"


#include "main/includes.h"

Entity::Entity()
{}

/*void Entity::Render(Image* framebuffer, Camera* camera, const Color& c) {
	std::vector<Vector3> meshVertices = mesh.GetVertices();

	for (int i = 0; i +2 < meshVertices.size(); i+= 3) {
        Vector3 v0 = ModelMatrix * meshVertices[i];
        Vector3 v1 = ModelMatrix * meshVertices[i + 1];
        Vector3 v2 = ModelMatrix * meshVertices[i + 2];

        bool negZ0, negZ1, negZ2;
        Vector3 clip0 = camera->ProjectVector(v0, negZ0);
        Vector3 clip1 = camera->ProjectVector(v1, negZ1);
        Vector3 clip2 = camera->ProjectVector(v2, negZ2);

        // Discard triangles outside the camera frustum
			float screenWidth = static_cast<float>(framebuffer->width);
			float screenHeight = static_cast<float>(framebuffer->height);

            // Pass from clip space to the screen space
            Vector3 screen0 = Vector3((clip0.x + 1.0f) * 0.5f * screenWidth, (1.0f + clip0.y) * 0.5f * screenHeight, clip0.z);
            Vector3 screen1 = Vector3((clip1.x + 1.0f) * 0.5f * screenWidth, (1.0f + clip1.y) * 0.5f * screenHeight, clip1.z);
            Vector3 screen2 = Vector3((clip2.x + 1.0f) * 0.5f * screenWidth, (1.0f + clip2.y) * 0.5f * screenHeight, clip2.z);

            //draw the triangle
            framebuffer->DrawLineDDA(static_cast<int>(screen0.x), static_cast<int>(screen0.y),
                static_cast<int>(screen1.x), static_cast<int>(screen1.y),c);
            framebuffer->DrawLineDDA(static_cast<int>(screen1.x), static_cast<int>(screen1.y),
                static_cast<int>(screen2.x), static_cast<int>(screen2.y), c);
            framebuffer->DrawLineDDA(static_cast<int>(screen2.x), static_cast<int>(screen2.y),
                static_cast<int>(screen0.x), static_cast<int>(screen0.y), c);
		
	}
}*/

void Entity::Render(Image* framebuffer, Camera* camera, const Color& c) {
    std::vector<Vector3> meshVertices = mesh.GetVertices();

    for (int i = 0; i + 2 < meshVertices.size(); i += 3) {
        Vector4 v0 = Vector4(meshVertices[i].x, meshVertices[i].y, meshVertices[i].z, 1);
        Vector4 v1 = Vector4(meshVertices[i + 1].x, meshVertices[i + 1].y, meshVertices[i + 1].z, 1);
        Vector4 v2 = Vector4(meshVertices[i + 2].x, meshVertices[i + 2].y, meshVertices[i + 2].z, 1);

        Vector4 v0World4 = ModelMatrix * v0;
        Vector3 v0World3 = v0World4.GetVector3();

        Vector4 v1World4 = ModelMatrix * v1;
        Vector3 v1World3 = v1World4.GetVector3();

        Vector4 v2World4 = ModelMatrix * v2;
        Vector3 v2World3 = v2World4.GetVector3();

        bool negZ0, negZ1, negZ2;
        Vector3 clip0 = camera->ProjectVector(v0World3, negZ0);
        Vector3 clip1 = camera->ProjectVector(v1World3, negZ1);
        Vector3 clip2 = camera->ProjectVector(v2World3, negZ2);

        // Discard triangles outside the camera frustum
        float screenWidth = static_cast<float>(framebuffer->width);
        float screenHeight = static_cast<float>(framebuffer->height);

        // Pass from clip space to the screen space
        Vector3 screen0 = Vector3((clip0.x + 1.0f) * 0.5f * screenWidth, (1.0f + clip0.y) * 0.5f * screenHeight, clip0.z);
        Vector3 screen1 = Vector3((clip1.x + 1.0f) * 0.5f * screenWidth, (1.0f + clip1.y) * 0.5f * screenHeight, clip1.z);
        Vector3 screen2 = Vector3((clip2.x + 1.0f) * 0.5f * screenWidth, (1.0f + clip2.y) * 0.5f * screenHeight, clip2.z);

        //draw the triangle
        framebuffer->DrawLineDDA(static_cast<int>(screen0.x), static_cast<int>(screen0.y),
            static_cast<int>(screen1.x), static_cast<int>(screen1.y), c);
        framebuffer->DrawLineDDA(static_cast<int>(screen1.x), static_cast<int>(screen1.y),
            static_cast<int>(screen2.x), static_cast<int>(screen2.y), c);
        framebuffer->DrawLineDDA(static_cast<int>(screen2.x), static_cast<int>(screen2.y),
            static_cast<int>(screen0.x), static_cast<int>(screen0.y), c);
    }
}