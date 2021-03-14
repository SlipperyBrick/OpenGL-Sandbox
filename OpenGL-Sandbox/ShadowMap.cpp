#include "ShadowMap.h"

ShadowMap::ShadowMap()
{
    m_FBO = 0;
    m_shadowMap = 0;
    m_width = 0;
    m_height = 0;
}

ShadowMap::~ShadowMap()
{
    if (m_FBO)
        glDeleteFramebuffers(1, &m_FBO);

    if (m_shadowMap)
        glDeleteTextures(1, &m_shadowMap);
}

bool ShadowMap::Init(unsigned int width, unsigned int height) {

    m_height = height;
    m_width = width;

    glGenFramebuffers(1, &m_FBO);

    glGenTextures(1, &m_shadowMap);
    glBindTexture(GL_TEXTURE_2D, m_shadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    //GL_NEAREST 
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
   
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMap, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE) {
        printf("[SHADOW MAP ERROR]: %i\n", status);
        return false;
    }


    return true;
}

void ShadowMap::BindFBO()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

void ShadowMap::BindTexture(const int textureUnit)
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_shadowMap);
}
