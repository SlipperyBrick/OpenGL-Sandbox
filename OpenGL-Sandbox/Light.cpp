#include "Light.h"

Light::Light()
{
	m_position = glm::vec3(0.f);
	m_colour = glm::vec3(1.f);
	m_intensity = 10.f;
}

Light::Light(glm::vec3 position, glm::vec3 colour, float intensity)
{
	m_position = position;
	m_colour = colour;
	m_intensity = intensity;
}

Light::~Light()
{
}

void Light::SetPosition(glm::vec3 position)
{
	m_position = position;
}

void Light::SetColour(glm::vec3 colour)
{
	m_colour = colour;
}

void Light::SetIntensity(float intensity)
{
	m_intensity = intensity;
}

glm::vec3 Light::GetPosition()
{
	return m_position;
}

glm::vec3* Light::GetPositionPtr()
{
	return &m_position;
}

glm::vec3* Light::GetColourPtr()
{
	return &m_colour;
}

float* Light::GetIntensityPtr()
{
	return &m_intensity;
}

glm::vec3 Light::GetColour()
{
	return m_colour;
}

float Light::GetIntensity()
{
	return m_intensity;
}
