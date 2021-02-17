#pragma once
#include "Light.h"
#include "Shader.h"

class PointLight : public Light
{
public:
    
    PointLight();
    ~PointLight();

    void SetConstant(float constant);
    void SetLinear(float linear);
    void SetQuadratic(float quadratic);

    float GetConstant();
    float GetLinear();
    float GetQuadratic();

    float* GetConstantPtr();
    float* GetLinearPtr();
    float* GetQuadraticPtr();

    void SetUniforms(Shader* shader);

private:
    float m_constant;
    float m_linear;
    float m_quadratic;
};

