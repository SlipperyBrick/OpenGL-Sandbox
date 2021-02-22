#pragma once

#include <iostream>
#include <string>
#include <assert.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL2/SOIL2.h>
#include <stb_image.h>

#include "Shader.h"
#include "Utility.h"

class Texture
{
public:
	Texture();
	Texture(const char* fileName);
	~Texture();

	GLuint const getTextureID();
	void Bind(const GLint textureUnit);
	void BindCubemap(const GLint textureUnit);
	void Unbind();
	void loadFromFile(const char* filepath);

	void LoadCubemap(const char* rightFace, const char* leftFace,
		             const char* topFace, const char* bottomFace,
		             const char* backFace, const char* frontFace);

	void LoadHDRI(const char* filepath);
	void CreateCubemapFromHDRI(const char* filepath);

private:
	GLuint id;
	int m_width;
	int m_height;
	int m_components;
};