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
	m_textures.push_back(texture);
}

void ResourceManager::Load(Material* material)
{
	if (!material->GetAlbedo()->GetPath().empty())
		m_textures.push_back(material->GetAlbedo());
	
	if (!material->GetNormal()->GetPath().empty())
		m_textures.push_back(material->GetNormal());

	if (!material->GetRoughness()->GetPath().empty())
		m_textures.push_back(material->GetRoughness());

	if (!material->GetMetallic()->GetPath().empty())
		m_textures.push_back(material->GetMetallic());

	if (!material->GetAO()->GetPath().empty())
		m_textures.push_back(material->GetAO());

	if (!material->GetDisplacement()->GetPath().empty())
		m_textures.push_back(material->GetDisplacement());

}

void ResourceManager::Update()
{
	m_current = m_textures.size();
	for (size_t i = 0; i < m_current - m_last; i++) {
		m_textureFutures.push_back(std::async(std::launch::async, LoadTexture, m_textures[m_last + i]));
	}

	if (m_textureFutures[m_iter]._Is_ready() && m_iter < m_textures.size() - 1) {
		m_textures[m_iter]->CreateTexture2D();
		m_iter++;
	}

	m_last = m_current;
}
