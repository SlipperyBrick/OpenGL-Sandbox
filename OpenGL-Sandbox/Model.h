#pragma once

#include <iostream>
#include <vector>

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


class Model : public Entity
{
public:
	Model();
	~Model();

	void Load(const char* filepath);
	void Create();
	void Render(Shader* shader);

	void ResetModel();
	glm::mat4 GetModelMatrix();

	std::vector<Texture*> GetTextures() const;
	std::vector<Material*> GetMaterials() const;

	void SetMaterial(Material* material, unsigned int index);

private:

	void LoadNode(aiNode* node, const aiScene* scene);

	void LoadMesh(aiMesh* mesh, const aiScene* scene);

	std::string GetTextures(aiMaterial* aiMat, aiTextureType type, std::string typeName);

	std::string m_path;
	std::vector<Mesh*> m_meshes;
	std::vector<Texture*> m_textures;
	std::vector<Material*> m_materials;
};