#include "RenderTarget.h"

RenderTarget::RenderTarget(unsigned int width, unsigned int height) {
	
	// Generate and bind a framebuffer
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	// Generate and bind textures
	m_Buffer.CreateDrawTexture(width, height);
	m_Buffer.Bind(0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Buffer.GetTextureID(), 0);
	glGenRenderbuffers(1, &m_RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

	// Check framebuffer is correctly configured
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "RenderTarget framebuffer failed..\n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_Buffer.Unbind();
}

RenderTarget::~RenderTarget()
{
	if(m_FBO)
		glDeleteFramebuffers(1, &m_FBO);

	if (m_RBO)
		glDeleteRenderbuffers(1, &m_RBO);
}

void RenderTarget::Bind(Window& window) {
	
	window.SetViewport();
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO); //Bind Framebuffer
	glBindRenderbuffer(GL_RENDERBUFFER, m_RBO); //Bind Renderbuffer
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window.GetBufferWidth(), window.GetBufferHeight()); //resize framebuffer
	
	m_Buffer.Bind(0); //bind buffer texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window.GetBufferWidth(), window.GetBufferHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); //resize texture

	glEnable(GL_DEPTH_TEST); 

	glClearColor(0.25f, 0.25f, 0.25f, 1.0f); //clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void RenderTarget::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
}

Texture* RenderTarget::GetTexture()
{
	return &m_Buffer;
}
