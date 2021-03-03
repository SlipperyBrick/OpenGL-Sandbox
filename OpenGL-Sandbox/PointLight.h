#pragma once
#include "Light.h"
#include "Shader.h"

class PointLight : public Light
{
public:
    
    PointLight();
    ~PointLight();

    void SetPosition(glm::vec3 position);
    void SetConstant(float constant);
    void SetLinear(float linear);
    void SetQuadratic(float quadratic);

    glm::vec3 GetPosition();
    float GetConstant();
    float GetLinear();
    float GetQuadratic();

    float* GetConstantPtr();
    float* GetLinearPtr();
    float* GetQuadraticPtr();

    void SetUniforms(Shader* shader);

private:
    glm::vec3 m_position;
    float m_constant;
    float m_linear;
    float m_exponent;
};

