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

Skybox::Skybox(std::vector<std::string> facesPaths) {
	this->m_shader = new Shader();
	m_shader->CreateFromFile("Shaders/SkyboxVert.glsl", "Shaders/SkyboxFrag.glsl");

	this->m_mesh = nullptr;
	this->m_cubemapID = 0;

	CreateCubemap(facesPaths.data());
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

//TODO remove
void Skybox::CreateCubemap(std::string imagePath[6])
{

	glGenTextures(1, &m_cubemapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapID);

	int width, height;
	
	for (size_t i = 0; i < 6; i++) {
		
		unsigned char* image = SOIL_load_image(imagePath[i].c_str(), &width, &height, NULL, SOIL_LOAD_RGB);
	    
	    if (!image) {
	    	std::cout << "[ERROR]: Failed to load texture " << '"' << imagePath[i].c_str() << '"' << "\n";
	    }

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);

	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


}

void Skybox::CreateMesh()
{
	this->m_mesh = new Mesh();
	m_mesh->Create(GetCubeVerticesPtr(), GetCubeIndicesPtr(), cubeVerticesCount, cubeIndicesCount);

}
