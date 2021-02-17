#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed, float fov)
{
	m_position = startPosition;
	m_worldUp = startUp;
	
	m_yaw = -90.f;
	m_pitch = 0.f;
	m_front = glm::vec3(0.0f);
	m_up = startUp;
	m_right = glm::vec3(0.f);

	startingMoveSpeed = startMoveSpeed;
	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	m_fov = fov;

	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;


	if (keys[GLFW_KEY_E]) {
		m_position.y += velocity;
	}

	if (keys[GLFW_KEY_Q]) {
		m_position.y -= velocity;
	}

	if (keys[GLFW_KEY_W])
	{
		m_position += m_front * velocity; //old
	}

	if (keys[GLFW_KEY_S])
	{
		m_position -= m_front * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		m_position -= m_right * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		m_position += m_right * velocity;
	}

	if (keys[GLFW_KEY_LEFT_SHIFT]) 
	{
		moveSpeed = 10.0f;
	}
	else
	{
		moveSpeed = startingMoveSpeed;
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	m_yaw += xChange;
	m_pitch += yChange;

	if (m_pitch > 89.0f)
	{
		m_pitch = 89.0f;
	}

	if (m_pitch < -89.0f)
	{
		m_pitch = -89.0f;
	}

	update();
}

glm::mat4 Camera::CalculateViewMatrix()
{
	return glm::lookAt(m_position, m_position + m_front, m_worldUp);
}

glm::mat4 Camera::CalculateProjectionMatrix(GLfloat bufferWidth, GLfloat bufferHeight)
{          
	return glm::perspective(glm::radians(m_fov), static_cast<float>(bufferWidth) / bufferHeight, 0.1f, 100.f);
}

glm::vec3 Camera::GetCameraPosition()
{
	return m_position;
}

glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(m_front);
}

GLfloat Camera::GetYaw()
{
	return m_yaw;
}

GLfloat Camera::GetPitch()
{
	return m_pitch;
}

void Camera::update()
{
	m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front.y = sin(glm::radians(m_pitch));
	m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(m_front);

	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

Camera::~Camera()
{
}
