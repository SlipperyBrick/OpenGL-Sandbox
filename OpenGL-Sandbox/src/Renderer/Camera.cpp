#include "Camera.h"

Camera::Camera() {

	m_window = nullptr;

	m_view = glm::mat4(1.f);
	m_projection = glm::mat4(1.f);

	m_bufferHeight = 1;
	m_bufferWidth = 1;

	m_x = 0.f;
	m_y = 0.f;

	m_position = {0, 0, 0};
	m_front    = {0, 0, 0};
	m_up       = {0, 1, 0};
	m_right    = {0, 0, 0};
	m_worldUp  = { 0, 1, 0 };

	m_yaw = -90.f;
	m_pitch = 0.f;

	m_fov = 60.f;

	m_moveSpeed = 5;
	m_startingMoveSpeed = 5;
	m_turnSpeed = 110;

}

Camera::Camera(Window* window, glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed, float fov) {
	
	m_window = window;

	m_view = glm::mat4(1.f);
	m_projection = glm::mat4(1.f);

	m_bufferHeight = 1;
	m_bufferWidth = 1;

	m_x = 0.f;
	m_y = 0.f;

	m_position = startPosition;
	m_worldUp = startUp;
	
	m_yaw = -90.f;
	m_pitch = 0.f;
	m_front = glm::vec3(0.0f);
	m_up = startUp;
	m_right = glm::vec3(0.f);

	m_startingMoveSpeed = startMoveSpeed;
	m_moveSpeed = startMoveSpeed;
	m_turnSpeed = startTurnSpeed;

	m_fov = fov;

}

void Camera::KeyControl()
{
	bool* keys = m_window->GetsKeys();
	
	GLfloat velocity = m_moveSpeed * m_window->GetDeltaTime();

	if (keys[GLFW_KEY_E]) {
		m_position.y += velocity;
	}

	if (keys[GLFW_KEY_Q]) {
		m_position.y -= velocity;
	}

	if (keys[GLFW_KEY_W])
	{
		m_position += m_front * velocity;
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
		m_moveSpeed = 10.0f;
	}
	else
	{
		m_moveSpeed = m_startingMoveSpeed;
	}
}

void Camera::MouseControl() {

	m_x = m_window->GetXChange();
	m_y = m_window->GetYChange();

	m_x *= m_turnSpeed * m_window->GetDeltaTime();
	m_y *= m_turnSpeed * m_window->GetDeltaTime();

	m_yaw += m_x;
	m_pitch += m_y;

	if (m_pitch > 89.0f)
	{
		m_pitch = 89.0f;
	}

	if (m_pitch < -89.0f)
	{
		m_pitch = -89.0f;
	}

}

void Camera::Update()
{
	m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front.y = sin(glm::radians(m_pitch));
	m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(m_front);

	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));

	m_view = glm::lookAt(m_position, m_position + m_front, m_worldUp);

	m_bufferWidth = m_window->GetBufferWidth();
	m_bufferHeight = m_window->GetBufferHeight();

	(m_bufferWidth + m_bufferHeight) == 0 ? m_bufferWidth = 1 : m_bufferWidth = m_bufferWidth; //Make sure buffers arent 0 so program doesnt divide 0 by 0
	m_projection = glm::perspective(glm::radians(m_fov), (m_bufferWidth) / m_bufferHeight, 0.1f, 100.f);
	
}

glm::mat4 Camera::GetViewMatrix() {
	return m_view;
}

glm::mat4 Camera::GetProjectionMatrix() {   
	return m_projection;
}

glm::vec3 Camera::GetCameraPosition()
{
	return m_position;
}

glm::vec3 Camera::GetCameraDirection()
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

Camera::~Camera()
{
}
