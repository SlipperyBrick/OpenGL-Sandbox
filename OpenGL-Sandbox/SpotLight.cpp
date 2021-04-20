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

void SpotLight::Bind(Shader* shader, int Index) {

	shader->SetVec3f("spotlight.base.m_position", (glm::vec3&)GetPosition());
	shader->SetVec3f("spotlight.m_direction", (glm::vec3&)GetDirection());
	shader->SetVec4f("spotlight.base.m_colour", glm::vec4(GetColour(), GetIntensity()));

	shader->Set1f("spotlight.m_outterEdge", GetOutterCutOff());
	shader->Set1f("spotlight.m_innerEdge", GetInnerCutOff());

	GetShadowMapPtr()->BindTexture(TU_SPOT_SHADOW_MAP0);
	shader->Set1i("spotlight.base.m_omniShadowMap", TU_SPOT_SHADOW_MAP0);
	shader->Set1f("spotlight.base.m_omniFarPlane", GetFarPlane());

}


