#include "Model.h"

Model::Model()
{
}

Model::~Model()
{
}

void Model::Load(const char* filepath) {

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath, 
		aiProcess_Triangulate | 
		aiProcess_FlipUVs | 
		aiProcess_CalcTangentSpace | 
		aiProcess_GenSmoothNormals | 
		aiProcess_JoinIdenticalVertices);

	if (!scene) {
		printf("[MODEL LOADER]: %s\n", importer.GetErrorString());
		return;
	}
	else {
		printf("[MODEL LOADER]: Opening - %s \n", filepath);
		LoadNode(scene->mRootNode, scene);
	}

}

void Model::Create()
{
	Mesh* tempMesh = new Mesh();
	tempMesh->Create(m_vertices.data(), m_indices.data(), m_vertices.size(), m_indices.size());
	m_meshes.push_back(tempMesh);
}

void Model::Render()
{
	for (auto m : m_meshes)
		m->Render();
}

void Model::ResetModel()
{
	for (auto m : m_meshes)
		m->ResetModel();
}

glm::mat4 Model::GetModel()
{
	return glm::mat4(1);
}

void Model::SetPosition(const glm::vec3 position)
{
	for (auto m : m_meshes)
		m->SetTranslate(position);
}

void Model::SetRotate(const glm::vec3 rotation)
{
	for (auto m : m_meshes)
		m->SetRotation(rotation);
}

void Model::SetScale(const glm::vec3 scale)
{
	for (auto m : m_meshes)
		m->SetScale(scale);
}

void Model::LoadNode(aiNode* node, const aiScene* scene) {

	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene);
	}

}

void Model::LoadMesh(aiMesh* mesh, const aiScene* scene) {

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		m_vertices.push_back(mesh->mVertices[i].x);
		m_vertices.push_back(mesh->mVertices[i].y);
		m_vertices.push_back(mesh->mVertices[i].z);

		if (mesh->mTextureCoords[0])
		{
			m_vertices.push_back(mesh->mTextureCoords[0][i].x);
			m_vertices.push_back(mesh->mTextureCoords[0][i].y);
		}
		else {
			m_vertices.push_back(0);
			m_vertices.push_back(0);
		}

		m_vertices.push_back(mesh->mNormals[i].x);
		m_vertices.push_back(mesh->mNormals[i].y);
		m_vertices.push_back(mesh->mNormals[i].z);
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++) {

		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++) {
			m_indices.push_back(face.mIndices[j]);
		}
	}

}

std::string Model::GetFolderName(const aiScene* scene, const char* filepath) {

	std::string temp = scene->GetShortFilename(filepath);
	std::string buffer;
	int size = 0;
	bool isReachedExtention = false;
	for (size_t i = 0; i < temp.length(); i++)
	{
		if (temp[i] == '.') {
			isReachedExtention = true;

		}
		else if (!isReachedExtention) {
			buffer += temp[i];
		}

	}
	return buffer;
}
