#pragma once
#include <vector>

#include "Material.h"

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	void Load(Texture* texture);
	void Load(Material* material);
	void Load(std::vector<Texture*> textureArray);
	void Load(std::vector<Material*> materialArray);

	void Update();

private:


	int m_current, m_last;

	int m_iter; // TODO use stack method instead of standalone iterator 
	std::unordered_map<std::string, Texture*> m_textureMap;
	std::vector<Texture*> m_textures;
	std::vector<std::future<void>> m_textureFutures;
};

