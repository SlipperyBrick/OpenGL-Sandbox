#pragma once

#include <iostream>
#include <string>
#include <assert.h>
#include <future>
#include <mutex>
#include <unordered_map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stb_image.h>

#include "Shader.h"
#include "Utility.h"
#include "Timer.hpp"

class Texture
{
public:
	Texture();
	Texture(std::string path);
	~Texture();

	GLuint const GetTextureID();
	void Bind(const GLint textureUnit);
	void Unbind();

	void CreateTexture2D();

	void LoadImageData();
	void LoadHDRIData();

	void CreateDrawTexture(unsigned int width, unsigned int height);

	void LoadCubemap(const char* rightFace, const char* leftFace,
		             const char* topFace, const char* bottomFace,
		             const char* backFace, const char* frontFace);

	void CreateHDRI();

	void CreateCubemapFromHDRI(Texture& HDRI);
	void CreateIrradianceTexture(Texture* Cubemap);
	void CreatePrefilterMap(Texture* environmentMap);
	void CreateBRDFLookUpTable();

	void inline SetPath(std::string path) { m_path = path.c_str(); };
	void inline SetID(GLuint id) { m_id = id; };

	inline void SetData2D(unsigned char* data) { m_data2D = data; };
	inline unsigned char* GetData2DPtr() { return m_data2D; };

	inline unsigned int GetWidth()  { return m_width; };
	inline int*  GetWidthPtr() { return &m_width; };
	
	inline unsigned int GetHeight() { return m_height; };
	inline int* GetHeightPtr() { return &m_height; };

	inline unsigned int GetComponents() { return m_components; };
	inline int* GetComponentsPtr() { return &m_components; };

	inline void SetFormat(GLenum format) { m_format = format; };
	inline GLenum GetFormat() { return m_format; };

	void SetPath(const char* path) { m_path = path; };
	std::string inline GetPath() { return std::string(m_path); };

private:

	GLuint m_id;
	int m_width;
	int m_height;
	int m_components;
	int m_textureType;
	GLenum m_format;
	std::string m_path;
	unsigned char* m_data2D;
	float* m_imageHDRI;
	unsigned int m_captureFBO, m_captureRBO;
};