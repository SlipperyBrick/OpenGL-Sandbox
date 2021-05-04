#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Entity
{
public:
	Entity();
	~Entity();

	void SetTranslation(glm::vec3 translate);
	void SetRotation(glm::vec3 rotation);
	void SetScale(glm::vec3 scale);

	glm::vec3 GetTranslation();
	glm::vec3 GetRotation ();
	glm::vec3 GetScale    ();

	glm::vec3* GetTranslationPtr();
	glm::vec3* GetScalePtr();
	glm::vec3* GetRotationPtr();

	void SetModel(glm::mat4* model);
	glm::mat4 GetModel();
	glm::mat4* GetModelPtr();
	
	void UpdateModel();
	void ResetModel();

	glm::mat4 m_model;
private:
	glm::vec3 m_translate;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
};

