#include "entity.h"


#include "main/includes.h"

Entity::Entity()
{}

void Entity::Render(Image* framebuffer, Camera* camera, FloatImage* zBuffer) {

    std::vector<Vector3> meshVertices = mesh.GetVertices();
    std::vector<Vector2> meshUVs = mesh.GetUVs();

    for (size_t i = 0; (i + 2) < meshVertices.size(); i += 3) {

        // From vector3 to vector4
        Vector4 v0 = Vector4(meshVertices[i].x, meshVertices[i].y, meshVertices[i].z, 1);
        Vector4 v1 = Vector4(meshVertices[i + 1].x, meshVertices[i + 1].y, meshVertices[i + 1].z, 1);
        Vector4 v2 = Vector4(meshVertices[i + 2].x, meshVertices[i + 2].y, meshVertices[i + 2].z, 1);

        //Change back to vector3
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

        Vector2 uv0 = Vector2(meshUVs[i].x, meshUVs[i].y);
        Vector2 uv1 = Vector2(meshUVs[i + 1].x, meshUVs[i + 1].y);
        Vector2 uv2 = Vector2(meshUVs[i + 2].x, meshUVs[i + 2].y);

        // Discard triangles outside the camera frustum
        if (negZ0 == FALSE && negZ1 == FALSE && negZ2 == FALSE) {
            float screenWidth = static_cast<float>(framebuffer->width);
            float screenHeight = static_cast<float>(framebuffer->height);

            // Pass from clip space to the screen space
            Vector3 screen0 = Vector3((clip0.x + 1.0f) * 0.5f * screenWidth, (1.0f + clip0.y) * 0.5f * screenHeight, clip0.z);
            Vector3 screen1 = Vector3((clip1.x + 1.0f) * 0.5f * screenWidth, (1.0f + clip1.y) * 0.5f * screenHeight, clip1.z);
            Vector3 screen2 = Vector3((clip2.x + 1.0f) * 0.5f * screenWidth, (1.0f + clip2.y) * 0.5f * screenHeight, clip2.z);

            triangleInfo.p0 = Vector3(screen0.x, screen0.y, screen0.z);
            triangleInfo.p1 = Vector3(screen1.x, screen1.y, screen1.z);
            triangleInfo.p2 = Vector3(screen2.x, screen2.y, screen2.z);
            triangleInfo.c0 = Color::RED;
            triangleInfo.c1 = Color::BLUE;
            triangleInfo.c2 = Color::GREEN;
            triangleInfo.uv0 = uv0;
            triangleInfo.uv1 = uv1;
            triangleInfo.uv2 = uv2;
            triangleInfo.texture = texture;

            //draw the triangle
            //framebuffer->DrawTriangle(Vector2(screen0.x, screen0.y), Vector2(screen1.x, screen1.y), Vector2(screen2.x, screen2.y), c, true, c);
            framebuffer->DrawTriangleInterpolated(triangleInfo, zBuffer);
        }
    }
}

void Entity::Update(float seconds_elapsed) 
{
    // rotate, translate and scale
    // if they are not changed, would do nothing
    ModelMatrix.Rotate(rotation.w, Vector3(rotation.x, rotation.y, rotation.z));
    ModelMatrix.Translate(translate.x, translate.y, translate.z);
    ModelMatrix.Scale(scale.x, scale.y, scale.z);

    // it lets the mesh increase or decrease at max by 2 
    if (ModelMatrix.M[0][0] > 2  || ModelMatrix.M[0][0] < 0.5 || ModelMatrix.M[1][1] > 2 || ModelMatrix.M[1][1] < 0.5 || ModelMatrix.M[1][1] > 2 || ModelMatrix.M[1][1] < 0.5) {
        scale.x = 1/scale.x;
        scale.y = 1/scale.y;
        scale.z = 1/scale.z;
    }
    // iterate 50 times then change direction
    if (i == 50) {
        translate.x = -translate.x;
        translate.y = -translate.y;
        translate.z = -translate.z;
        i = 0;
    }
    i++;
}