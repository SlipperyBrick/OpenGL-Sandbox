#include "Model.h"

Model::Model()
{
	m_path = "";
}

Model::Model(const char* path, const char* name)
{
	m_path = path;
	m_name = name;
}

Model::~Model()
{
}

void Model::Load() {

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(m_path, 
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
		printf("[MODEL LOADER]: Opening - %s \n", m_path.c_str());
		LoadNode(scene->mRootNode, scene);

	}

}

void Model::Create() {
	for (const auto& m : m_meshes) {
		m->Create();
	}
}

void Model::Render(Shader* shader)
{
	UpdateModel();
	for (size_t i = 0; i < m_meshes.size(); i++) {
		shader->SetMat4f("u_Model", GetModel() * m_meshes[i]->GetModel(), false);
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

void Model::Update(std::vector<Material*>& materials)
{
	if (ImGui::TreeNode(m_name.c_str())) {
		ImGui::DragFloat3(std::string(m_name + " Translation").c_str(), (float*)GetTranslationPtr(), 0.1f);
		ImGui::DragFloat3(std::string(m_name + " Scale").c_str(), (float*)GetScalePtr(), 0.1f);
		ImGui::DragFloat3(std::string(m_name + " Rotation").c_str(), (float*)GetRotationPtr(), 0.1f);
		for (size_t i = 0; i < m_meshes.size(); i++)
		{
			if (ImGui::TreeNode(m_meshes[i]->GetName().c_str())) {
				ImGui::DragFloat3(std::string(m_meshes[i]->GetName() + " Translation").c_str(), (float*)m_meshes[i]->GetTranslationPtr(), 0.1f);
				ImGui::DragFloat3(std::string(m_meshes[i]->GetName() + " Scale").c_str(), (float*)m_meshes[i]->GetScalePtr(), 0.1f);
				ImGui::DragFloat3(std::string(m_meshes[i]->GetName() + " Rotation").c_str(), (float*)m_meshes[i]->GetRotationPtr(), 0.1f);
				
				// Simple selection popup (if you want to show the current selection inside the Button itself,
				// you may want to build a string using the "###" operator to preserve a constant ID with a variable label)
				if (ImGui::Button("Select Material"))
					ImGui::OpenPopup("my_select_popup");
				ImGui::SameLine();
				ImGui::TextUnformatted(m_materials[i]->GetName().c_str());
				if (ImGui::BeginPopup("my_select_popup"))
				{
					ImGui::Text("Materials");
					ImGui::Separator();
					for (int j = 0; j < materials.size(); j++)
						if (ImGui::Selectable(materials[j]->GetName().c_str())) {
							m_materials[i] = materials[j];
						}
							
					ImGui::EndPopup();
				}

				ImGui::TreePop();
			}
		}
		/*
		static int selected_material = -1;
		static bool toggles[] = { true, false, false, false, false };
		
		// Simple selection popup (if you want to show the current selection inside the Button itself,
		// you may want to build a string using the "###" operator to preserve a constant ID with a variable label)
		if (ImGui::Button("Select.."))
			ImGui::OpenPopup("my_select_popup");
		ImGui::SameLine();
		ImGui::TextUnformatted(selected_material == -1 ? "<None>" : materials[selected_material]->GetName().c_str());
		if (ImGui::BeginPopup("my_select_popup"))
		{
			ImGui::Text("Materials");
			ImGui::Separator();
			for (int i = 0; i < materials.size(); i++)
				if (ImGui::Selectable(materials[i]->GetName().c_str()))
					selected_material = i;
			ImGui::EndPopup();
		}
		*/
		ImGui::TreePop();
	}
}

std::vector<Material*> Model::GetMaterials() const
{
	return m_materials;
}

void Model::SetMaterial(Material* material)
{
	for (size_t i = 0; i < m_materials.size(); i++)
	{
		m_materials[i] = material;
	}

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
	//
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
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	if (mesh->mMaterialIndex >= 0)
	{
		
		l_material->SetName(material->GetName().C_Str());
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
			l_material->SetNormal(t);
			l_material->UseNormalTexture(true);
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
			l_material->SetMetallic(0.f);
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
			l_material->SetRoughness(1.f);
		};

		if (!aoStr.empty()) {
			Texture* t = new Texture(aoStr);
			l_material->UseAOTexture(true);
			l_material->SetAO(t);
			m_textures.push_back(t);
		}
		else {
			l_material->UseAOTexture(false);
			l_material->SetAO(0.5);
		};

	}
	
	Mesh* l_mesh = new Mesh(material->GetName().C_Str(), m_vertices, m_indices);
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
