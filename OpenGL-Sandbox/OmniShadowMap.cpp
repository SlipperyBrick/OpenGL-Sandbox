#include "OmniShadowMap.h"

OmniShadowMap::OmniShadowMap() : ShadowMap() {
}

OmniShadowMap::~OmniShadowMap()
{
}

bool OmniShadowMap::Init(unsigned int width, unsigned int height)
{
    m_width = width; m_height = height;

    glGenFramebuffers(1, &m_FBO);
    glGenTextures(1, &m_shadowMap);

    glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMap);

    for (size_t i = 0; i < 6; i++) 
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_shadowMap, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE) {
        printf("[OMNI SHADOW MAP ERROR]: %i\n", status);
        return false;
    }

    return true;
}

void OmniShadowMap::BindFBO()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

void OmniShadowMap::BindTexture(const int textureUnit)
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMap);
}
