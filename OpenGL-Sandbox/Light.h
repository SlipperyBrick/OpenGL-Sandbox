#pragma once

#include "glm/glm.hpp"

class Light
{
public:

	Light();
	Light(glm::vec3 position, glm::vec3 colour, float intensity);
	~Light();

	void SetPosition(glm::vec3 position);
	void SetColour(glm::vec3 colour);
	void SetIntensity(float intensity);

	glm::vec3 GetPosition();
	glm::vec3 GetColour();
	float GetIntensity();
	
	glm::vec3* GetPositionPtr();
	glm::vec3* GetColourPtr();
	float    * GetIntensityPtr();

private:
	glm::vec3 m_position;
	glm::vec3 m_colour;
	float     m_intensity;
};

