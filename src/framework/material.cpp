#include "material.h"
#include "main/includes.h"

Material::Material(Shader* shader, Texture* colorTexture, Texture* normalTexture, Vector3 Ka, Vector3 Kd, Vector3 ks, float alpha) {
    this->shader = shader;
    this->colorTexture = colorTexture;
    this->normalTexture = normalTexture;
    this->Ka = Ka;
    this->Kd = Kd;
    this->Ks = Ks;
    shinninnes = alpha;
}

void Material::Enable() {
	this->shader->Enable();
}

void Material::Enable(const sUniformData& uniformData) {
    //material properties
    shader->Enable();
    shader->SetTexture("u_texture", colorTexture);
    shader->SetTexture("normalMap", normalTexture);
    shader->SetVector3("Ka", Ka);
    shader->SetVector3("Kd", Kd);
    shader->SetVector3("Ks", Ks);
    shader->SetFloat("shinninnes", shinninnes);

    //3D object properties
    shader->SetMatrix44("u_model", uniformData.modelMatrix);

    //Camera properties
    shader->SetMatrix44("u_viewprojection", uniformData.viewProjectionMatrix);
    shader->SetVector3("cameraPosition", uniformData.cameraPosition);

    //Light prooperties
    shader->SetVector3("Ia", uniformData.Ia);
    shader->SetVector3("lightPosition", uniformData.light.position);
    shader->SetVector3("Id", uniformData.light.Id);
    shader->SetVector3("Is", uniformData.light.Is);

    shader->SetVector3("flag", Vector3(uniformData.flag.x, uniformData.flag.y, 0.0));
}

void Material::Disable() {
    this->shader->Disable();
}