#include "Skybox.h"

Skybox::Skybox() {

	this->m_shader = new Shader();
	m_shader->CreateFromFile("Shaders/SkyboxVert.glsl", "Shaders/SkyboxFrag.glsl");

	this->m_mesh = nullptr;
	this->m_cubemapID = 0;

	m_cubemap = nullptr;
	CreateMesh();
}

Skybox::Skybox(Texture* cubemap)
{
	this->m_shader = new Shader();
	m_shader->CreateFromFile("Shaders/SkyboxVert.glsl", "Shaders/SkyboxFrag.glsl");

	this->m_mesh = nullptr;
	this->m_cubemapID = 0;

	m_cubemap = cubemap;
	CreateMesh();
}

Skybox::~Skybox()
{
}

void Skybox::Render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	m_shader->Bind();

	m_shader->SetMat4f(viewMatrix, "u_viewMatrix", false);
	m_shader->SetMat4f(projectionMatrix, "u_projectionMatrix", false);

	m_cubemap->Bind(0);

	m_mesh->Render();
}

void Skybox::SetCubemap(Texture* cubemap)
{
	this->m_cubemap = cubemap;
}

void Skybox::CreateMesh()
{
	this->m_mesh = new Mesh();
	m_mesh->Create(GetCubeVerticesPtr(), GetCubeIndicesPtr(), cubeVerticesCount, cubeIndicesCount);

}
