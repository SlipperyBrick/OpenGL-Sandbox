#pragma once
#include <vector>

#include "Renderer/Material.h"
#include "Renderer/Model.h"

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	void Load(Texture* texture);
	void Load(Material* material);
	void Load(std::vector<Texture*> textureArray);
	void Load(std::vector<Material*> materialArray);
	void Load(Model* model);

	void Update(std::vector<Material*> &materialsList);

private:


	int m_textureCurrentCount, m_textureLastCount;
	int m_modelCurrentCount, m_modelLastCount;

	std::unordered_map<std::string, Texture*> m_textureMap;
	std::vector<Texture*> m_textures;
	std::vector<std::future<void>> m_textureFutures;

	std::unordered_map<std::string, Model*> m_modelMap;
	std::vector<Model*> m_models;
	std::vector<std::future<void>> m_modelFutures;
};

