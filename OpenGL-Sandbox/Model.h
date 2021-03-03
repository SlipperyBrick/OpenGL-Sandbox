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


class Model
{
public:
	Model();
	~Model();

	void Load(const char* filepath);
	void Create();
	void Render();

	void ResetModel();
	glm::mat4 GetModel();

	void SetPosition(const glm::vec3 position);
	void SetRotate(const glm::vec3 rotation);
	void SetScale(const glm::vec3 scale);

private:

	void LoadNode(aiNode* node, const aiScene* scene);

	void LoadMesh(aiMesh* mesh, const aiScene* scene);

	std::string GetFolderName(const aiScene* scene, const char* filepath);

	std::vector<Mesh*> m_meshes;
	std::vector<GLfloat> m_vertices;
	std::vector<unsigned int> m_indices;
};