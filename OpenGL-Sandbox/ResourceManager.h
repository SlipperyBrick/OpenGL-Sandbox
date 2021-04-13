#pragma once
#include <vector>

#include "Material.h"

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	void Load(Texture* texture);
	void Load(Material* texture);

	void Update();

private:


	int m_current, m_last;

	int m_iter; // TODO use stack method instead of standalone iterator 
	std::vector<Texture*> m_textures;
	std::vector<std::future<void>> m_textureFutures;
};

