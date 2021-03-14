#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	m_direction = glm::vec3(0.0f, -1.0f, 0.0f);
	m_projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);
}

DirectionalLight::DirectionalLight(float shadowWidth, float shadowHeight, glm::vec3 colour, float intensity, glm::vec3 direction) 
	: Light(shadowWidth, shadowHeight, colour,  intensity)
{
	m_direction = direction;
	m_projection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
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

DirectionalLight::~DirectionalLight()
{
}
