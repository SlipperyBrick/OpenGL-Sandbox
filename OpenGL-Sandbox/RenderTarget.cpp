#include "RenderTarget.h"

RenderTarget::RenderTarget(unsigned int width, unsigned int height) {

	// Generate and bind a framebuffer
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	// Generate and bind textures
	m_Buffer = Texture();
	m_Buffer.CreateDrawTexture(width, height);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Buffer.GetTextureID(), 0);

	glGenRenderbuffers(1, &m_FBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_FBO);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_FBO);

	// Check framebuffer is correctly configured
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "RenderTarget framebuffer failed..\n";
	}

}

RenderTarget::~RenderTarget()
{
	//if(m_FBO != -1)
	//	glDeleteFramebuffers(GL_FRAMEBUFFER, &m_FBO);

	//if(m_RBO != -1)
	//	glDeleteRenderbuffers(GL_RENDERBUFFER, &m_RBO);
}

void RenderTarget::Bind(unsigned int width, unsigned int height)
{
	//Resize everything to window size here
	glViewport(0, 0, width, height);
	m_Buffer.Bind(0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	m_Buffer.Unbind();

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderTarget::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Texture* RenderTarget::GetTexture()
{
	return &m_Buffer;
}
