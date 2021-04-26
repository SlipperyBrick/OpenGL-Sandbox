#pragma once

#include <vector>

#include <GL/glew.h>

#include "Light.h"
#include "Shader.h"
#include "OmniShadowMap.h"

class PointLight : public Light
{
public:
    
    PointLight();
    PointLight(glm::vec4 colour, glm::vec3 position, 
        float shadowWidth, float shadowHeight, 
        float nearPlane, float farPlane);
    ~PointLight();

    void Bind(Shader* shader, int index);

    std::vector<glm::mat4> CalculateLightTransform();
    inline float GetFarPlane() const { return m_farPlane; };

    void SetPosition(glm::vec3 position);

    glm::vec3 GetPosition();
    glm::vec3* GetPositionPtr();

private:
    glm::vec3 m_position;
    float m_farPlane;
};

