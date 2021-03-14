#include "SpotLight.h"

SpotLight::SpotLight(glm::vec3 position, glm::vec4 colour, glm::vec3 direction, float innerCutOff, float outterCutOff,
	unsigned int shadowWidth, unsigned int shadowHeight, float nearPlane, float farPlane)
	: PointLight(colour, position, shadowWidth, shadowHeight, nearPlane, farPlane)
{
	m_direction = glm::normalize(direction);
	m_innerCutOff = glm::radians(innerCutOff);
	m_outterCutOff = glm::radians(outterCutOff);
}

SpotLight::~SpotLight()
{
}


