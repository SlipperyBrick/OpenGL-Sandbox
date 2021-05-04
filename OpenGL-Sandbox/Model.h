#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <array>
#include <iostream>
#include <string_view>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "Timer.hpp"
#include "GuiLayer.h"
#include "Window.h"

class Model : public Entity
{
public:
	Model();
	Model(const char* path, const char* name);
	~Model();

	void Load();
	void Create();
	void Render(Shader* shader);

	void Update(std::vector<Material*>& materials);

	std::vector<Material*> GetMaterials() const;

	void SetMaterial(Material* material);
	std::string GetPath() const { return m_path; };
private:

	void LoadNode(aiNode* node, const aiScene* scene);

	void LoadMesh(aiMesh* mesh, const aiScene* scene);

	std::string GetTextures(aiMaterial* aiMat, aiTextureType type, std::string typeName);

	std::string m_path, m_name;
	std::vector<Mesh*> m_meshes;
	std::vector<Texture*> m_textures;
	std::vector<Material*> m_materials;

};