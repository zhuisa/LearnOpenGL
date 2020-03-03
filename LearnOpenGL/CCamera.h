#pragma once
#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>


enum ECameraMovement
{
	FORWARD, BACKWARD, LEFT, RIGHT
};
//摄像机的默认值
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class CCamera
{
public:
	glm::vec3 m_Position;
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::vec3 m_WorldUp;
	// Euler Angles
	float m_Yaw;
	float m_Pitch;
	// Camera options
	float m_MovementSpeed;
	float m_MouseSensitivity;
	float m_Zoom;


	// Constructor with vectors
	CCamera(glm::vec3 vPosition = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 vUp = glm::vec3(0.0f, 1.0f, 0.0f), float vYaw = YAW, float vPitch = PITCH)
		: m_Front(glm::vec3(0.0f, 0.0f, 1.0f)), m_MovementSpeed(SPEED), m_MouseSensitivity(SENSITIVITY), m_Zoom(ZOOM)
	{
		m_Position = vPosition;
		m_WorldUp = vUp;
		m_Yaw = vYaw;
		m_Pitch = vPitch;
		updateCameraVectors();
	}
	// Constructor with scalar values
	CCamera(float vPosX, float vPosY, float vPosZ, float vUpX, float vUpY, float vUpZ, float vYaw, float vPitch)
		: m_Front(glm::vec3(0.0f, 0.0f, 1.0f)), m_MovementSpeed(SPEED), m_MouseSensitivity(SENSITIVITY), m_Zoom(ZOOM)
	{
		m_Position = glm::vec3(vPosX, vPosY, vPosZ);
		m_WorldUp = glm::vec3(vUpX, vUpY, vUpZ);
		m_Yaw = vYaw;
		m_Pitch = vPitch;
		updateCameraVectors();
	}

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 getViewMatrix()
	{
		return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void processKeyboard(ECameraMovement vDirection, float vDeltaTime)
	{
		float velocity = m_MovementSpeed * vDeltaTime;
		if (vDirection == FORWARD)
			m_Position += m_Front * velocity;
		if (vDirection == BACKWARD)
			m_Position -= m_Front * velocity;
		if (vDirection == LEFT)
			m_Position -= m_Right * velocity;
		if (vDirection == RIGHT)
			m_Position += m_Right * velocity;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void processMouseMovement(float vXoffset, float vYoffset, GLboolean vConstrainPitch = true)
	{
		vXoffset *= m_MouseSensitivity;
		vYoffset *= m_MouseSensitivity;

		m_Yaw += vXoffset;
		m_Pitch += vYoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (vConstrainPitch)
		{
			if (m_Pitch > 89.0f)
				m_Pitch = 89.0f;
			if (m_Pitch < -89.0f)
				m_Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void processMouseScroll(float vYoffset)
	{
		if (m_Zoom >= 1.0f && m_Zoom <= 45.0f)
			m_Zoom -= vYoffset;
		if (m_Zoom <= 1.0f)
			m_Zoom = 1.0f;
		if (m_Zoom >= 45.0f)
			m_Zoom = 45.0f;
	}

private:
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 Front;
		Front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		Front.y = sin(glm::radians(m_Pitch));
		Front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Front = glm::normalize(Front);
		// Also re-calculate the Right and Up vector
		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));
	}
};

