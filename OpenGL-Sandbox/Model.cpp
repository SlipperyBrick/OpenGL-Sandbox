#include "Model.h"

Model::Model()
{
}

Model::~Model()
{
}

void Model::Load(const char* filepath) {

	m_path = filepath;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath, 
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_OptimizeMeshes |
		aiProcess_OptimizeGraph |
		aiProcess_SortByPType |
		aiProcess_SplitLargeMeshes |
		aiProcess_CalcTangentSpace |
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

}

void Model::Render(Shader* shader)
{
	UpdateModel();
	for (size_t i = 0; i < m_meshes.size(); i++) {
		shader->SetMat4f("u_Model", this->GetModel(), false);
		m_materials[i]->Bind(shader);
		m_meshes[i]->Render();
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

std::vector<Material*> Model::GetMaterials() const
{
	return m_materials;
}

void Model::SetMaterial(Material* material, unsigned int index)
{
	if (index <= m_materials.size())
		m_materials[index] = material;

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

	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	m_vertices = std::vector<Vertex>(mesh->mNumVertices);

	bool hasTangents = mesh->HasTangentsAndBitangents();

	Vertex v;
	for (size_t i = 0; i < mesh->mNumVertices; i++) {
		v.m_position =  glm::vec3(mesh->mVertices[i].x,  mesh->mVertices[i].y,   mesh->mVertices[i].z);
		v.m_normal =    glm::vec3(mesh->mNormals[i].x,    mesh->mNormals[i].y,    mesh->mNormals[i].z);
		if (hasTangents) {
			v.m_tangent =   glm::vec3(mesh->mTangents[i].x,   mesh->mTangents[i].y,   mesh->mTangents[i].z);
			v.m_bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
		}
		else {
			v.m_tangent	  = glm::vec3(0.f);
		    v.m_bitangent =	glm::vec3(0.f);
		}
	
		if (mesh->mTextureCoords[0])
			v.m_texcoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		else 
			v.m_texcoords = glm::vec2(0.f);

		m_vertices[i] = v;
	}
	
	for (unsigned int  i = 0; i < mesh->mNumFaces; i++) {

		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			m_indices.push_back(face.mIndices[j]);
		}
	}

	Material* l_material = new Material;
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		
		std::string texturePath = m_path.substr(0, m_path.find_last_of("\\/"));
		texturePath.append("/");

		std::string albedoStr = GetTextures(material, aiTextureType_DIFFUSE, texturePath);
		std::string normalStr = GetTextures(material, aiTextureType_NORMALS, texturePath);
		std::string specularStr = GetTextures(material, aiTextureType_SPECULAR, texturePath);
		std::string roughnessStr = GetTextures(material, aiTextureType_DIFFUSE_ROUGHNESS, texturePath);
		std::string aoStr = GetTextures(material, aiTextureType_AMBIENT_OCCLUSION, texturePath);
		
		if (!albedoStr.empty()) {
			Texture* t = new Texture(albedoStr);
			l_material->UseAlbedoTexture(true);
			l_material->SetAlbedo(t);
			m_textures.push_back(t);
		}
		else {
			l_material->UseAlbedoTexture(false);
			aiColor3D color(0.f, 0.f, 0.f);
			material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			l_material->SetAlbedo(glm::vec3(color.r, color.g, color.b));
		};

		if (!normalStr.empty()) {
			Texture* t = new Texture(normalStr);
			l_material->UseNormalTexture(true);
			l_material->SetNormal(t);
			m_textures.push_back(t);
		}

		if (!specularStr.empty()) {
			Texture* t = new Texture(specularStr);
			l_material->UseMetallicTexture(true);
			l_material->SetMetallic(t);
			m_textures.push_back(t);
		}
		else {
			l_material->UseMetallicTexture(false);
			aiColor3D color(0.f, 0.f, 0.f);
			material->Get(AI_MATKEY_COLOR_SPECULAR, color);
			color.r == 0.f ? color.r = 0.1 : color.r = color.r;
			l_material->SetMetallic(color.r);
		};

		if (!roughnessStr.empty()) {
			Texture* t = new Texture(roughnessStr);
			l_material->UseRoughnessTexture(true);
			l_material->SetRoughness(t);
			m_textures.push_back(t);
		}
		else {
			l_material->UseRoughnessTexture(false);
			aiColor3D color(0.f, 0.f, 0.f);
			material->Get(AI_MATKEY_COLOR_REFLECTIVE, color);
			l_material->SetRoughness(color.r);
		};

		if (!aoStr.empty()) {
			Texture* t = new Texture(aoStr);
			l_material->UseAOTexture(true);
			l_material->SetAO(t);
			m_textures.push_back(t);
		}
		else {
			l_material->UseAOTexture(false);
			l_material->SetAO(1.0);
		};

	}

	Mesh* l_mesh = new Mesh(m_vertices, m_indices);
	m_meshes.push_back(l_mesh);
	m_materials.push_back(l_material);
	m_vertices.clear();
	m_indices.clear();
}

std::string Model::GetTextures(aiMaterial* aiMat, aiTextureType type, std::string path) {

	aiString str;
	for (unsigned int i = 0; i < aiMat->GetTextureCount(type); i++)
	{
		aiMat->GetTexture(type, 0, &str);
		if (str.C_Str()) {
			return path.append(str.C_Str());
		}
		str.Clear();
	}
	return std::string();
}
