#include "Model.h"

//https://realitymultiplied.wordpress.com/2016/07/24/assimp-skeletal-animation-tutorial-3-something-about-skeletons/

aiMatrix4x4 GLMMat4ToAi(glm::mat4 mat)
{
	return aiMatrix4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
		mat[1][0], mat[1][1], mat[1][2], mat[1][3],
		mat[2][0], mat[2][1], mat[2][2], mat[2][3],
		mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
}

glm::mat4 AiToGLMMat4(aiMatrix4x4& in_mat)
{
	glm::mat4 tmp;
	tmp[0][0] = in_mat.a1;
	tmp[1][0] = in_mat.b1;
	tmp[2][0] = in_mat.c1;
	tmp[3][0] = in_mat.d1;

	tmp[0][1] = in_mat.a2;
	tmp[1][1] = in_mat.b2;
	tmp[2][1] = in_mat.c2;
	tmp[3][1] = in_mat.d2;

	tmp[0][2] = in_mat.a3;
	tmp[1][2] = in_mat.b3;
	tmp[2][2] = in_mat.c3;
	tmp[3][2] = in_mat.d3;

	tmp[0][3] = in_mat.a4;
	tmp[1][3] = in_mat.b4;
	tmp[2][3] = in_mat.c4;
	tmp[3][3] = in_mat.d4;
	return tmp;
}

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
		printf("[MODEL LOADER]: Opened - %s \n", filepath);

		LoadNode(scene->mRootNode, scene);
	}

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
	return this->m_meshes[0]->GetModel();
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

	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.push_back(mesh->mVertices[i].x);
		vertices.push_back(mesh->mVertices[i].y);
		vertices.push_back(mesh->mVertices[i].z);

		if (mesh->mTextureCoords[0])
		{
			vertices.push_back(mesh->mTextureCoords[0][i].x);
			vertices.push_back(mesh->mTextureCoords[0][i].y);
		}
		else {
			vertices.push_back(0);
			vertices.push_back(0);
		}

		vertices.push_back(mesh->mNormals[i].x);
		vertices.push_back(mesh->mNormals[i].y);
		vertices.push_back(mesh->mNormals[i].z);
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++) {

		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh* tempMesh = new Mesh();
	tempMesh->Create(vertices.data(), indices.data(), vertices.size(), indices.size());
	m_meshes.push_back(tempMesh);
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
