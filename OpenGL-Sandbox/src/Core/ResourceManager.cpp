#include "ResourceManager.h"


void static LoadTexture(Texture* texture){
	texture->LoadImageData();
}

void static LoadModel(Model* model) {
	model->Load();
}

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{
	m_textures.clear();
	m_textureFutures.clear();
}

void ResourceManager::Load(Texture* texture)
{
    m_textureMap[texture->GetPath()] = texture;
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

void ResourceManager::Load(Model* model)
{
	std::string path = model->GetPath();
	if (m_modelMap.find(path) != m_modelMap.end()) {
		model = m_modelMap[path];
		return;
	}

	m_modelMap[path] = model;
	m_models.push_back(model);
}

void ResourceManager::Load(Material* material)
{

	if (material->HasAlbedoTexture()) {
	
		std::string path = material->GetAlbedo()->GetPath();
		if (m_textureMap.find(path) != m_textureMap.end()) {
			material->SetAlbedo(m_textureMap[path]);
		}
		else {
			Load(material->GetAlbedo());
		}

	}
	
	if (material->HasNormalTexture()) {
		std::string path = material->GetNormal()->GetPath();
		if (m_textureMap.find(path) != m_textureMap.end()) {
			material->SetNormal(m_textureMap[path]);
		}
		else {
			Load(material->GetNormal());
		}
	}

	if (material->HasRoughnessTexture()) {
		std::string path = material->GetRoughness()->GetPath();
		if (m_textureMap.find(path) != m_textureMap.end()) {
			material->SetRoughness(m_textureMap[path]);
		} else {
			Load(material->GetRoughness());
		}
	}

	if (material->HasMetallicTexture()) {
		std::string path = material->GetMetallic()->GetPath();
		if (m_textureMap.find(path) != m_textureMap.end()) {
			material->SetMetallic(m_textureMap[path]);
		}
		else {
			Load(material->GetMetallic());
		}
	}

	if (material->HasAOTexture()) {
		std::string path = material->GetAO()->GetPath();
		if (m_textureMap.find(path) != m_textureMap.end()) {
			material->SetAO(m_textureMap[path]);
		}
		else {
			Load(material->GetAO());
		}
	}

	if (material->HasDisplacementTexture()) {
		std::string path = material->GetDisplacement()->GetPath();
		if (m_textureMap.find(path) != m_textureMap.end()) {
			material->SetDisplacement(m_textureMap[path]);
		}
		else {
			Load(material->GetDisplacement());
		}
	}
	

}

void ResourceManager::Update(std::vector<Material*> &materialsList)
{

	m_textureCurrentCount = m_textures.size();
	for (int i = 0; i < m_textureCurrentCount - m_textureLastCount; i++) { //TODO Make loop iterate once per frame
		m_textureFutures.push_back(std::async(std::launch::async, LoadTexture, m_textures[m_textureLastCount + i]));
	}
	m_textureLastCount = m_textureCurrentCount;

	m_modelCurrentCount = m_models.size();
	for (int i = 0; i < m_modelCurrentCount - m_modelLastCount; i++) { //TODO Make loop iterate once per frame
		m_modelFutures.push_back(std::async(std::launch::async, LoadModel, m_models[m_modelLastCount + i]));
	}
	m_modelLastCount = m_modelCurrentCount;


	if (!m_textures.empty()) {
		for (size_t i = 0; i < m_textures.size(); i++)
		{
			if (m_textureFutures[i]._Is_ready()) {
				m_textures[i]->CreateTexture2D();
				m_textures.erase(m_textures.begin() + i);
				m_textureFutures.erase(m_textureFutures.begin() + i);
			}
		}
	}
	
	if (!m_models.empty()) {
		for (size_t i = 0; i < m_models.size(); i++)
		{
			if (m_modelFutures[i]._Is_ready()) {
				m_models[i]->Create();
				Load(m_models[m_models.size() - 1]->GetMaterials());
				for (size_t i = 0; i < m_models[m_models.size() - 1]->GetMaterials().size(); i++)
				{
					materialsList.push_back(m_models[m_models.size() - 1]->GetMaterials()[i]);
				}
				m_models.erase(m_models.begin() + i);
				m_modelFutures.erase(m_modelFutures.begin() + i);
			}
		}

	}

}

