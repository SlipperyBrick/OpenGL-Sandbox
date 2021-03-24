#pragma once

#include "Texture.h"
#include "Shader.h"

class RenderTarget
{
public:

	RenderTarget(unsigned int width, unsigned int height);
	~RenderTarget();

	void Bind(unsigned int width, unsigned int height);
	void Unbind();

	Texture* GetTexture();

private:

	unsigned int m_FBO, m_RBO;
	Texture m_Buffer;

};

