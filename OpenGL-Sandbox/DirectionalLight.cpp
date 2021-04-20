#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	m_direction = glm::vec3(0.0f, -1.0f, 0.0f);
	m_projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);
	m_filterLevel = 0;
}

DirectionalLight::DirectionalLight(float shadowWidth, float shadowHeight, int filterLevel, glm::vec3 colour, float intensity, glm::vec3 direction)
	: Light(shadowWidth, shadowHeight, colour,  intensity)
{
	m_direction = direction;
	m_projection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
	m_filterLevel = filterLevel;
}

glm::vec3 DirectionalLight::GetDirection()
{
	return m_direction;
}

glm::vec3* DirectionalLight::GetDirectionPtr()
{
	return &m_direction;
}

glm::mat4 DirectionalLight::CalculateLightTransform()
{
	return m_projection * glm::lookAt(-m_direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));	
}

int DirectionalLight::GetFilterLevel()
{
	return m_filterLevel;
}

int* DirectionalLight::GetFilterLevelPtr()
{
	return &m_filterLevel;
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::Bind(Shader* shader) {

	GetShadowMapPtr()->BindTexture(TU_DIRECTIONAL_SHADOW_MAP);
	
	shader->SetVec4f("DirLight.m_colour", glm::vec4(m_colour, m_intensity));
	shader->SetVec3f("DirLight.m_direction", m_direction);
	shader->SetMat4f("u_DirectionLightTransform", CalculateLightTransform(), false);
	shader->Set1i("DirLight.m_shadowMap", TU_DIRECTIONAL_SHADOW_MAP);
	shader->Set1i("DirLight.m_filterLevel", m_filterLevel);

}
