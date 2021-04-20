#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "Window.h"

class Camera
{
public:
	Camera();
	Camera(Window* window, glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed, float fov);

	void SetWindow(Window* window) { m_window = window; };

	void KeyControl();
	void MouseControl();

	glm::vec3 GetCameraPosition();
	glm::vec3 GetCameraDirection();

	GLfloat GetYaw();
	GLfloat GetPitch();

	float GetFOV() { return m_fov; };
	float* GetFOVptr() { return &m_fov; };

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

	void Update();
	
	~Camera();

private:
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;

	glm::mat4 m_view, m_projection;

	GLfloat m_yaw,m_pitch;

	float m_fov;

	GLfloat m_moveSpeed;
	GLfloat m_startingMoveSpeed;
	GLfloat m_turnSpeed;

	Window* m_window;
	GLfloat m_bufferWidth, m_bufferHeight;

	GLfloat m_x, m_y;
};

