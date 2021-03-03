#pragma once
#include "Light.h"

class DirectionalLight :
	public Light
{
public:
	DirectionalLight();
	DirectionalLight(float shadowWidth, float shadowHeight, glm::vec3 colour, float intensity, glm::vec3 direction);

	glm::vec3  GetDirection();
	glm::vec3 *GetDirectionPtr();
	glm::mat4 CalculateLightTransform();

	~DirectionalLight();

private:
	glm::vec3 m_direction;
};

