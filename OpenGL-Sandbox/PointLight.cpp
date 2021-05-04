#include "PointLight.h"

PointLight::PointLight()
{
    m_position = glm::vec3(0.f);
    m_farPlane = 50;
}

PointLight::PointLight(glm::vec4 colour, glm::vec3 position, float shadowWidth, float shadowHeight, float nearPlane, float farPlane)
    :Light(shadowWidth, shadowHeight, glm::vec3(colour.x, colour.y, colour.z), colour.w)
{
    m_position = position;
    m_farPlane = farPlane;

    float aspectRatio = ((float)shadowWidth / (float)shadowHeight);
    m_projection = glm::perspective(glm::radians(90.f), aspectRatio, nearPlane, farPlane);

    m_shadowMap = new OmniShadowMap();
    m_shadowMap->Init(shadowWidth, shadowHeight);

}

PointLight::~PointLight()
{
}

void PointLight::Bind(Shader* shader, int index)
{

    const std::string pos = "pointLight[" + std::to_string(index) + "].m_position";
    shader->SetVec3f(pos.c_str(), m_position);

    const std::string col = "pointLight[" + std::to_string(index) + "].m_colour";
    shader->SetVec4f(col.c_str(), glm::vec4(m_colour, m_intensity));

    const std::string shadowMap = "pointLight[" + std::to_string(index) + "].m_omniShadowMap";
    shader->Set1i(shadowMap.c_str(), TU_POINT_SHADOW_MAP0 + index);

    const std::string shadowFarPlane = "pointLight[" + std::to_string(index) + "].m_omniFarPlane";
    shader->Set1f(shadowFarPlane.c_str(), m_farPlane);

    GetShadowMapPtr()->BindTexture(TU_POINT_SHADOW_MAP0 + index);

}

std::vector<glm::mat4> PointLight::CalculateLightTransform()
{
    std::vector<glm::mat4> lightTransforms;

    //pos x
    lightTransforms.push_back(m_projection * glm::lookAt(m_position, m_position + glm::vec3(1, 0, 0) , glm::vec3(0, -1, 0)));
    //neg x
    lightTransforms.push_back(m_projection * glm::lookAt(m_position, m_position + glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)));
    //pos y
    lightTransforms.push_back(m_projection * glm::lookAt(m_position, m_position + glm::vec3(0, 1, 0) , glm::vec3(0, 0, 1)));
    //neg y
    lightTransforms.push_back(m_projection * glm::lookAt(m_position, m_position + glm::vec3(0, -1, 0), glm::vec3(0, 0, -1)));
    //pos z
    lightTransforms.push_back(m_projection * glm::lookAt(m_position, m_position + glm::vec3(0, 0, 1), glm::vec3(0, -1, 0)));
    //neg z
    lightTransforms.push_back(m_projection * glm::lookAt(m_position, m_position + glm::vec3(0, 0, -1), glm::vec3(0, -1, 0)));

    return lightTransforms;
}

void PointLight::SetPosition(glm::vec3 position)
{
    m_position = position;
}

glm::vec3 PointLight::GetPosition()
{
    return m_position;
}

glm::vec3* PointLight::GetPositionPtr()
{
    return &m_position;
}

