#include "entity.h"


#include "main/includes.h"

Entity::Entity()
{}

void Entity::Render(Image* framebuffer, Camera* camera, const Color& c) {
	std::vector<Vector3> meshVertices = mesh.GetVertices();

	for (int i = 0; i < mesh.GetVertices().size(); i++) {
		Vector3 localVertex = meshVertices[i];

		Vector3 localVertex = meshVertices[i];

		Vector3 worldVertex = ModelMatrix * localVertex;

		bool negZ;
		Vector3 clipVertex = camera->ProjectVector(worldVertex, negZ);

		if (negZ  == false){
			float screenWidth = static_cast<float>(framebuffer->GetWidth());
			float screenHeight = static_cast<float>(framebuffer->GetHeight());

			Vector3 screenVertex = Vector3((clipVertex.x + 1.0f) * 0.5f * screenWidth,
				(1.0f - clipVertex.y) * 0.5f * screenHeight,
				clipVertex.z);

			// Now you can use the screenVertex or implement the DDA algorithm to draw the point on the framebuffer
			// For simplicity, let's assume there is a DrawPoint method in your Image class
			framebuffer->DrawPoint(static_cast<int>(screenVertex.x), static_cast<int>(screenVertex.y), c);

		}
	}

}