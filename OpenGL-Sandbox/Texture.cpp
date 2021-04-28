#include "Texture.h"

//view and projection matrix for prospective
const glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
const glm::mat4 captureViews[] = {
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
};

Texture::Texture() {

	m_id = NULL;
	m_width = NULL;
	m_height = NULL;
	m_textureType = NULL;
	m_captureFBO = NULL;
	m_captureRBO = NULL;
	m_components = NULL;
	m_path = "";
	m_data2D = nullptr;
	m_imageHDRI = nullptr;	
	m_format = NULL;
	glGenTextures(1, &m_id);
}

Texture::Texture(std::string path) {

	m_id = NULL;
	m_width = NULL;
	m_height = NULL;
	m_textureType = GL_TEXTURE_2D;
	m_captureFBO = NULL;
	m_captureRBO = NULL;
	m_components = NULL;
	m_path = path;
	m_data2D =  nullptr;
	m_imageHDRI = nullptr;
	m_format = NULL;
	glGenTextures(1, &m_id);
}

Texture::~Texture() {
	glDeleteTextures(1, &m_id);
}

GLuint const Texture::GetTextureID()
{
	return m_id;
}

void Texture::Bind(const GLint textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(this->m_textureType, m_id);
}

void Texture::Unbind()
{
	glActiveTexture(0);
	glBindTexture(m_textureType, 0);
}

void Texture::CreateTexture2D()
{
	if (m_id == 0)
		glGenTextures(1, &m_id);

	if (m_data2D) {

		m_textureType = GL_TEXTURE_2D;
		glBindTexture(this->m_textureType, m_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, m_data2D);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(this->m_textureType, 0);

	} else {
		
		if (!m_path.c_str()) {
			printf("[ERROR]: No path to load texture. \n");
		} 
		std::cout << "[ERROR]: No texture data - " << m_path.c_str() << "\n";
	}

	stbi_image_free(m_data2D);

}

void Texture::LoadImageData() {	
	
	if (m_path.c_str()) {
		m_data2D = stbi_load(m_path.c_str(), &m_width, &m_height, &m_components, 0);
		if (!m_data2D) {
			if (stbi_failure_reason())
				std::cout << "[ERROR]: " << m_path << " - " <<stbi_failure_reason() << std::endl;
			__debugbreak();
		}

	} else {
		std::cout << "[ERROR]: failed to load data, no path \n";
	}
}

void Texture::LoadHDRIData()
{
	stbi_set_flip_vertically_on_load(true);
	m_imageHDRI = stbi_loadf(m_path.c_str(), &this->m_width, &this->m_height, &this->m_components, NULL);
}

void Texture::CreateDrawTexture(unsigned int width, unsigned int height) {
	this->m_textureType = GL_TEXTURE_2D;

	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);	// Framebuffer texture size - resolution scale, i.e., width * height = 100% scale
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::LoadCubemap(const char* rightFace, const char* leftFace, 
     const char* topFace, const char* bottomFace, 
	 const char* backFace, const char* frontFace) {

	std::string imagePaths[6];
	imagePaths[0] = rightFace;
	imagePaths[1] = leftFace;
	imagePaths[2] = topFace;
	imagePaths[3] = bottomFace;
	imagePaths[4] = backFace;
	imagePaths[5] = frontFace;

	m_textureType = GL_TEXTURE_CUBE_MAP;

	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

	for (size_t i = 0; i < 6; i++) {

		unsigned char* image = stbi_load(imagePaths[i].c_str(), &m_width, &m_height, &m_components, 0);

		if (!image) {
			std::cout << "[ERROR]: Failed to load cubemap " << '"' << imagePaths[i].c_str() << '"' << "\n";
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		stbi_image_free(image);

	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_2D, NULL);

}

void Texture::CreateHDRI() {
	
	m_textureType = GL_TEXTURE_2D;
	
	this->LoadHDRIData();
	if (!m_imageHDRI) {

		if (!m_path.c_str())
			std::cout << "[ERROR]: No HDRI image path to load \n";
		else
			std::cout << "[ERROR]: Failed to load texture " << '"' << m_path << '"' << "\n";
	}
	else {

		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_width, m_height, 0, GL_RGB, GL_FLOAT, m_imageHDRI);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


		glBindTexture(GL_TEXTURE_2D, NULL);

	}
	stbi_image_free(m_imageHDRI);

}

void Texture::CreateCubemapFromHDRI(Texture& HDRI) {

	this->m_textureType = GL_TEXTURE_CUBE_MAP;

	//Create Shader for converting;
	Shader equirectangularToCubemapShader;
	equirectangularToCubemapShader.CreateFromFile("Shaders/EquirectangularToCubemapVert.glsl", "Shaders/EquirectangularToCubemapFrag.glsl");
	
	//create framebuffer and renderbuffer
	glGenFramebuffers(1, &m_captureFBO);
	glGenRenderbuffers(1, &m_captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, m_captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_captureRBO);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_captureRBO);

	//create empty cubemap to place results into
	glGenTextures(1, &this->m_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->m_id);
	for (unsigned int i = 0; i < 6; i++)
	{
		// note that we store each face with 16 bit floating point values
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// convert HDR equirectangular environment map to cubemap equivalent
	HDRI.Bind(0);
	equirectangularToCubemapShader.Bind();
	equirectangularToCubemapShader.Set1i("u_equirectangularMap", 0);
	equirectangularToCubemapShader.SetMat4f("u_projection", captureProjection, false);

	glViewport(0, 0, 512, 512);
	glBindFramebuffer(GL_FRAMEBUFFER, m_captureFBO);
	for (unsigned int i = 0; i < 6; i++)
	{
	
		equirectangularToCubemapShader.SetMat4f("u_view", captureViews[i],  false);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, this->m_id, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderCube();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDeleteBuffers(1, &m_captureFBO);
	glDeleteRenderbuffers(1, &m_captureRBO);
}

void Texture::CreateIrradianceTexture(Texture* Cubemap) {

	this->m_textureType = GL_TEXTURE_CUBE_MAP;
	
	unsigned int resoultion = 64;

	Shader IrradianceConvolutionShader;
	IrradianceConvolutionShader.CreateFromFile("Shaders/SkyboxVert.glsl", "Shaders/IrradianceConvolutionFrag.glsl");
	
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

	for (size_t i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, resoultion, resoultion, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenFramebuffers(1, &m_captureFBO);
	glGenRenderbuffers(1, &m_captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, m_captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, resoultion, resoultion);

	Cubemap->Bind(0);
	IrradianceConvolutionShader.Bind();
	IrradianceConvolutionShader.Set1i("u_environmentMap", 0);
	IrradianceConvolutionShader.SetMat4f("u_projectionMatrix", captureProjection, false);

	glViewport(0, 0, resoultion, resoultion);
	glBindFramebuffer(GL_FRAMEBUFFER, m_captureFBO);
	for (unsigned int i = 0; i < 6; i++)
	{
		IrradianceConvolutionShader.SetMat4f("u_viewMatrix", captureViews[i], false);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_id, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderCube();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteBuffers(1, &m_captureFBO);
	glDeleteRenderbuffers(1, &m_captureRBO);
}

void Texture::CreatePrefilterMap(Texture* Cubemap) {

	this->m_textureType = GL_TEXTURE_CUBE_MAP;

	unsigned int resoultion = 128;

	Shader prefilterShader;
	prefilterShader.CreateFromFile("Shaders/SkyboxVert.glsl", "Shaders/PrefilterFrag.glsl");

	glBindTexture(GL_TEXTURE_CUBE_MAP, Cubemap->GetTextureID());
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, resoultion, resoultion, 0, GL_RGB, GL_FLOAT, nullptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minification filter to mip_linear 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	prefilterShader.Bind();
	prefilterShader.Set1i("u_environmentMap", 0);
	prefilterShader.SetMat4f("u_projectionMatrix", captureProjection, false);
	Cubemap->Bind(0);

	glGenFramebuffers(1, &m_captureFBO);
	glGenRenderbuffers(1, &m_captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, m_captureFBO);

	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		// reisze framebuffer according to mip-level size.
		unsigned int mipWidth = resoultion * std::pow(0.5, mip);
		unsigned int mipHeight = resoultion * std::pow(0.5, mip);
		glBindRenderbuffer(GL_RENDERBUFFER, m_captureFBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		prefilterShader.Set1f("u_roughness", roughness);
		for (unsigned int i = 0; i < 6; ++i)
		{
			prefilterShader.SetMat4f("u_viewMatrix", captureViews[i], false);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_id, mip);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			renderCube();
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDeleteBuffers(1, &m_captureFBO);
	glDeleteRenderbuffers(1, &m_captureRBO);

}

void Texture::CreateBRDFLookUpTable() {

	this->m_textureType = GL_TEXTURE_2D;

	Shader brdfShader;
	brdfShader.CreateFromFile("Shaders/BRDFShaderVert.glsl", "Shaders/BRDFShaderFrag.glsl");

	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenFramebuffers(1, &m_captureFBO);
	glGenRenderbuffers(1, &m_captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, m_captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_id, 0);
	
	glViewport(0, 0, 512, 512);
	brdfShader.Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderQuad();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDeleteFramebuffers(1, &m_captureFBO);
	glDeleteRenderbuffers(1, &m_captureRBO);
	
}
