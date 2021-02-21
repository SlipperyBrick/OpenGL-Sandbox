#include "Skybox.h"

Skybox::Skybox(std::vector<std::string> facesPaths)
{
	this->m_shader = new Shader();
	m_shader->CreateFromFile("Shaders/SkyboxVert.glsl", "Shaders/SkyboxFrag.glsl");

	this->m_mesh = nullptr;
	this->m_cubemapID = 0;

	CreateCubeMap(facesPaths.data());
	CreateMesh();
	std::cout << this->m_cubemapID << "\n";
}

Skybox::~Skybox()
{
}

void Skybox::Render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{

	viewMatrix = glm::mat4(glm::mat3(viewMatrix));

	glDepthMask(GL_FALSE);

	m_shader->Bind();

	m_shader->SetMat4f(viewMatrix, "u_viewMatrix", false);
	m_shader->SetMat4f(projectionMatrix, "u_projectionMatrix", false);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapID);

	m_mesh->Render();

	m_shader->Unbind();
	glDepthMask(GL_TRUE);

}

void Skybox::CreateCubeMap(std::string imagePath[6])
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
	//Define Cube
	unsigned int cubeIndices[] = {
		// front
		0, 1, 2,
		2, 1, 3,
		// right
		2, 3, 5,
		5, 3, 7,
		// back
		5, 7, 4,
		4, 7, 6,
		// left
		4, 6, 0,
		0, 6, 1,
		// top
		4, 0, 5,
		5, 0, 2,
		// bottom
		1, 6, 3,
		3, 6, 7
	};

	float cubeVertices[] = {
		-1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,

		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f
	};


	this->m_mesh = new Mesh();
	m_mesh->Create(cubeVertices, cubeIndices, 64, 36);

}
