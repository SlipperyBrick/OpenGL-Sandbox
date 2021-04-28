#include "Material.h"

Material::Material() {

	m_albedo = nullptr;
	m_normal = nullptr;
	m_roughness = nullptr;
	m_ao = nullptr;
	m_metallic = nullptr;
	m_displacement = nullptr;

	m_albedoColour = glm::vec3(1.f, 1.f, 1.f);
	m_roughnessValue = 0.5f;
	m_aoValue = 0.5f;
	m_metallicValue = 0.5f;

	m_hasAlbedoTexture = false;
	m_hasNormalTexture = false;
	m_hasRoughnessTexture = false;
	m_hasAOTexture = false;
	m_hasMetallicTexture = false;
	m_hasDisplacementTexture = false;

}

Material::Material(Texture& albedo)
{
	m_albedo = &albedo;
	m_normal = nullptr;
	m_roughness = nullptr;
	m_ao = nullptr;
	m_metallic = nullptr;
	m_displacement = nullptr;

	m_albedoColour = glm::vec3(1.f, 1.f, 1.f);
	m_roughnessValue = 1.f;
	m_aoValue = 1.f;
	m_metallicValue = 1.f;

	m_hasAlbedoTexture = true;
	m_hasNormalTexture = false;
	m_hasRoughnessTexture = false;
	m_hasAOTexture = false;
	m_hasMetallicTexture = false;
	m_hasDisplacementTexture = false;
}

Material::Material(glm::vec3 albedo, Texture& normal, Texture& roughness, Texture& ao, Texture& metallic)
{
	m_albedo = nullptr;
	m_normal = &normal;
	m_roughness = &roughness;
	m_ao = &ao;
	m_metallic = &metallic;
	m_displacement = nullptr;

	m_albedoColour = albedo;
	m_roughnessValue = 0.5f;
	m_aoValue = 0.5f;
	m_metallicValue = 0.5f;

	m_hasAlbedoTexture = false;
	m_hasNormalTexture = true;
	m_hasRoughnessTexture = true;
	m_hasAOTexture = true;
	m_hasMetallicTexture = true;
	m_hasDisplacementTexture = false;
}

Material::Material(Texture& albedo, Texture& normal, Texture& roughness, Texture& ao, Texture& metallic)
{
	m_albedo = &albedo;
	m_normal = &normal;
	m_roughness = &roughness;
	m_ao = &ao;
	m_metallic = &metallic;
	m_displacement = nullptr;

	m_albedoColour = glm::vec3(1.f);
	m_roughnessValue = 0.5f;
	m_aoValue = 0.5f;
	m_metallicValue = 0.5f;

	m_hasAlbedoTexture = true;
	m_hasNormalTexture = true;
	m_hasRoughnessTexture = true;
	m_hasAOTexture = true;
	m_hasMetallicTexture = true;
	m_hasDisplacementTexture = false;

}

Material::Material(Texture& albedo, Texture& normal, Texture& roughness, Texture& ao, float metallic)
{
	m_albedo = &albedo;
	m_normal = & normal;
	m_roughness = &roughness;
	m_ao = &ao;
	m_metallic = nullptr;
	m_displacement = nullptr;

	m_albedoColour = glm::vec3(1.f);
	m_roughnessValue = 0.5f;
	m_aoValue = 0.5f;
	m_metallicValue = metallic;

	m_hasAlbedoTexture = true;
	m_hasNormalTexture = true;
	m_hasRoughnessTexture = true;
	m_hasAOTexture = true;
	m_hasMetallicTexture = false;
	m_hasDisplacementTexture = false;

}

Material::Material(Texture& albedo, Texture& normal, Texture& roughness, Texture& ao, Texture& metallic, Texture& displacement)
{
	m_albedo = &albedo;
	m_normal = &normal;
	m_roughness = &roughness;
	m_ao = &ao;
	m_metallic = &metallic;
	m_displacement = &displacement;

	m_albedoColour = glm::vec3(1.f);
	m_roughnessValue = 0.5f;
	m_aoValue = 0.5f;
	m_metallicValue = 0.5f;

	m_hasAlbedoTexture = true;
	m_hasNormalTexture = true;
	m_hasRoughnessTexture = true;
	m_hasAOTexture = true;
	m_hasMetallicTexture = true;
	m_hasDisplacementTexture = false;

}

Material::~Material()
{
}

void Material::Bind(Shader* shader)
{

	shader->Set1i("material.m_HasAlbedoTexture",       m_hasAlbedoTexture);
	shader->Set1i("material.m_HasNormalTexture",       m_hasNormalTexture);
	shader->Set1i("material.m_HasRoughnessTexture",    m_hasRoughnessTexture);
	shader->Set1i("material.m_HasAOTexture",           m_hasAOTexture);
	shader->Set1i("material.m_HasMetallicTexture",     m_hasMetallicTexture);
	shader->Set1i("material.m_HasDisplacementTexture", m_hasDisplacementTexture);

	if (m_hasAlbedoTexture) {
		m_albedo->Bind(TU_ALBEDO);
	}
	else {
		shader->SetVec3f("material.m_Albedo", m_albedoColour);
	}

	if (m_hasNormalTexture) {
		m_normal->Bind(TU_NORMAL);
	}

	if (m_hasRoughnessTexture) {
		m_roughness->Bind(TU_ROUGHNESS);
	}
	else {
		shader->Set1f("material.m_Roughness", m_roughnessValue);
	}

	if (m_hasAOTexture) {
		m_ao->Bind(TU_AO);
	}
	else {
		shader->Set1f("material.m_AO", m_aoValue);
	}

	if (m_hasMetallicTexture) {
		m_metallic->Bind(TU_METALLIC);
	}
	else {
		shader->Set1f("material.m_Metallic", m_metallicValue);
	}

	if (m_hasDisplacementTexture) {
		m_displacement->Bind(TU_DISPLACEMENT);
	}
	
}
