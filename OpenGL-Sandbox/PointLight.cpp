#include "PointLight.h"

PointLight::PointLight()
{
    m_constant = 1.f;
    m_linear = 1.f;
    m_exponent = 1.f;
}

PointLight::~PointLight()
{
}

void PointLight::SetConstant(float constant)
{
    m_constant = constant;
}

void PointLight::SetLinear(float linear)
{
    m_linear = linear;
}

void PointLight::SetQuadratic(float exponent)
{
    m_exponent = exponent;
}

float PointLight::GetConstant()
{
    return m_constant;
}

float PointLight::GetLinear()
{
    return m_linear;
}

float PointLight::GetQuadratic()
{
    return m_exponent;
}

float* PointLight::GetConstantPtr()
{
    return &m_constant;
}

float* PointLight::GetLinearPtr()
{
    return &m_linear;
}

float* PointLight::GetQuadraticPtr()
{
    return &m_exponent;
}

void PointLight::SetUniforms(Shader* shader)
{
    /*
     vec3 m_position;
     vec3 m_colour;
     float m_intensity;
     float m_constant;
     float m_linear;
     float m_quadratic;
    */
    shader->SetVec3f(this->GetPosition(), "pointLight.m_position");
    shader->SetVec3f(this->GetColour(), "pointLight.m_colour");
    shader->Set1f(this->GetIntensity(), "pointLight.m_intensity");
    shader->Set1f(this->GetConstant(), "pointLight.m_constant");
    shader->Set1f(this->GetLinear(), "pointLight.m_linear");
    shader->Set1f(this->GetQuadratic(), "pointLight.m_quadratic");
}
