#include "ResourceManager.h"

void static LoadTexture(Texture* texture)
{
	texture->LoadImageData();

	switch (texture->GetComponents()) {

	case 1: texture->SetFormat(GL_RED); break;
	case 3: texture->SetFormat(GL_RGB); break;
	case 4: texture->SetFormat(GL_RGBA); break;

	default: break;
	}
}

ResourceManager::ResourceManager()
{
	m_current = 0;
	m_last = 0;
	m_iter = 0;
}

ResourceManager::~ResourceManager()
{
	m_textures.clear();
	m_textureFutures.clear();
}

void ResourceManager::Load(Texture* texture)
{
	std::string path = texture->GetPath();
	if (m_textureMap.find(path) != m_textureMap.end()) {
		texture = m_textureMap[path];
		return;
	}

    m_textureMap[path] = texture;
	m_textures.push_back(texture);
}

void ResourceManager::Load(std::vector<Texture*> textureArray)
{
	for (const auto t : textureArray)
		Load(t);
}

void ResourceManager::Load(std::vector<Material*> materialArray)
{
	for (const auto m : materialArray)
		Load(m);
}

void ResourceManager::Load(Material* material)
{
	if (material->HasAlbedoTexture())
		Load(material->GetAlbedo());
	
	if (material->HasNormalTexture())
		Load(material->GetNormal());

	if (material->HasRoughnessTexture())
		Load(material->GetRoughness());

	if (material->HasMetallicTexture())
		Load(material->GetMetallic());

	if (material->HasAOTexture())
		Load(material->GetAO());

	if (material->HasDisplacementTexture()) 
		Load(material->GetDisplacement());
	

}

void ResourceManager::Update()
{
	
	m_current = m_textures.size();
	for (size_t i = 0; i < m_current - m_last; i++) { //TODO Make loop iterate once per frame
		m_textureFutures.push_back(std::async(std::launch::async, LoadTexture, m_textures[m_last + i]));
	}
	m_last = m_current;

	if (m_iter != m_textures.size()) { 
		if (m_textureFutures[m_iter]._Is_ready()) {
			m_textures[m_iter]->CreateTexture2D();
			m_iter++;
		}
	}

	
}
