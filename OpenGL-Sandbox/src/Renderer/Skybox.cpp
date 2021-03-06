#include "Skybox.h"

Skybox::Skybox() {

	this->m_shader = new Shader();
	m_shader->CreateFromFile("assets/Shaders/SkyboxVert.glsl", "assets/Shaders/SkyboxFrag.glsl");

	this->m_mesh = nullptr;
	this->m_cubemapID = 0;

	m_cubemap = nullptr;
	CreateMesh();
}

Skybox::Skybox(Texture* cubemap)
{

	this->m_shader = new Shader();
	m_shader->CreateFromFile("assets/Shaders/SkyboxVert.glsl", "assets/Shaders/SkyboxFrag.glsl");

	this->m_mesh = nullptr;
	this->m_cubemapID = 0;

	m_cubemap = cubemap;
	CreateMesh();

}

Skybox::~Skybox()
{
}

void Skybox::Render(Camera* camera) {
	m_shader->Bind();

	m_shader->SetMat4f("u_viewMatrix", camera->GetViewMatrix(), false);
	m_shader->SetMat4f("u_projectionMatrix", camera->GetProjectionMatrix(),  false);

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
