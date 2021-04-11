#include "Material.h"

Material::Material() {
	//ADD DEFALUT
}

Material::Material(Texture& albedo, Texture& normal, Texture& roughness, Texture& ao, Texture& metallic)
{
	m_albedo = &albedo;
	m_normal = &normal;
	m_roughness = &roughness;
	m_ao = &ao;
	m_metallic = &metallic;
	m_displacement = new Texture();
}

Material::Material(Texture& albedo, Texture& normal, Texture& roughness, Texture& ao)
{
	m_albedo = &albedo;
	m_normal = & normal;
	m_roughness = &roughness;
	m_ao = &ao;
	m_metallic = new Texture();
	m_displacement = new Texture();
}

Material::Material(Texture& albedo, Texture& normal, Texture& roughness, Texture& ao, Texture& metallic, Texture& displacement)
{
	m_albedo = &albedo;
	m_normal = &normal;
	m_roughness = &roughness;
	m_ao = &ao;
	m_metallic = &metallic;
	m_displacement = &displacement;
}

Material::~Material()
{
}

void Material::Create()
{
	if(m_albedo->GetPath() != "")
		m_albedo->CreateTexture2D();

	if (m_normal->GetPath() != "")
		m_normal->CreateTexture2D();

	if (m_roughness->GetPath() != "")
		m_roughness->CreateTexture2D();

	if (m_ao->GetPath() != "")
		m_ao->CreateTexture2D();

	if (m_metallic->GetPath() != "")
		m_metallic->CreateTexture2D();

	if (m_displacement->GetPath() != std::string())
		m_displacement->CreateTexture2D();
}

void Material::SetAlbedo(Texture albedo) {
	m_albedo = &albedo;
}

void Material::Bind()
{
    m_albedo->Bind(TU_ALBEDO);
	m_normal->Bind(TU_NORMAL);
	m_roughness->Bind(TU_ROUGHNESS);
	m_ao->Bind(TU_AO);
	m_metallic->Bind(TU_METALLIC);
	m_displacement->Bind(TU_DISPLACEMENT);
}
