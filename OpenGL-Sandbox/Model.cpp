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

void Model::Create() {

	Mesh* m = new Mesh(m_vertices, m_indices);
	m_meshes.push_back(m);

}

void Model::Render()
{
	UpdateModel();
	for (auto m : m_meshes) {
		m->SetModel(this->GetModelPtr());
		m->Render();
	}
}

void Model::ResetModel()
{
	for (auto m : m_meshes)
		m->ResetModel();
}

glm::mat4 Model::GetModelMatrix()
{
	return this->GetModel();
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

	Vertex v;
	for (size_t i = 0; i < mesh->mNumVertices; i++) {
		
		v.m_position =  glm::vec3(mesh->mVertices[i].x ,  mesh->mVertices[i].y,   mesh->mVertices[i].z);
		v.m_normal =    glm::vec3(mesh->mNormals[i].x,    mesh->mNormals[i].y,    mesh->mNormals[i].z);
		v.m_tangent =   glm::vec3(mesh->mTangents[i].x,   mesh->mTangents[i].y,   mesh->mTangents[i].z);
		v.m_bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
	
		if (mesh->mTextureCoords[0])
			v.m_texcoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		else 
			v.m_texcoords = glm::vec2(0.f);

		m_vertices.push_back(v);
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
