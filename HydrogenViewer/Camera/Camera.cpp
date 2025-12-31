#include "Camera.h"

namespace HydrogenViewer
{



	void Camera::SetupCamera(float pFOV, glm::vec3 pPosition, float pAspectRatio, float pNearPlane, float pFarPlane)
	{
		m_FOV = pFOV;
		m_Position = pPosition;
		m_AspectRatio = pAspectRatio;
		m_NearPlane = pNearPlane;
		m_FarPlane = pFarPlane;

		m_Up    = glm::vec3(0.0f, 1.0f, 0.0f);
		m_Front = glm::vec3(0.0f, 0.0f, -1.0f);

	}

	void Camera::HandleCameraMovement(float pDeltaTime)
	{

		if (m_CameraLocked)
			return;

		if (Keyboard::GetKeyState(GLFW_KEY_W) == KEY_DOWN)
		{
			m_Position += m_Front * m_Speed*pDeltaTime;
		}
		if (Keyboard::GetKeyState(GLFW_KEY_S) == KEY_DOWN)
		{
			m_Position += -m_Front * m_Speed*pDeltaTime;
		}
		if (Keyboard::GetKeyState(GLFW_KEY_D) == KEY_DOWN)
		{
			m_Position += glm::normalize(glm::cross(m_Front, m_Up))* m_Speed*pDeltaTime;
		}
		if (Keyboard::GetKeyState(GLFW_KEY_A) == KEY_DOWN)
		{
			m_Position += -glm::normalize(glm::cross(m_Front, m_Up))* m_Speed*pDeltaTime;
		}

		if (Keyboard::GetKeyState(GLFW_KEY_SPACE) == KEY_DOWN)
		{
			m_Position += m_Up * m_Speed*pDeltaTime;
		}
		if (Keyboard::GetKeyState(GLFW_KEY_LEFT_SHIFT) == KEY_DOWN)
		{
			m_Position += -m_Up * m_Speed*pDeltaTime;
		}
	}

	void Camera::HandleCameraLooking()
	{
		static bool FirstTime = true;

		if (m_CameraLocked)
			return;

		if (Mouse::GetLeftKeyState() == KEY_DOWN)
		{
			if (FirstTime)
			{
				Mouse::DisableCursor(true);
			}


			float OffsetX = Mouse::GetOffsetX();
			float OffsetY = Mouse::GetOffsetY();

			if (!FirstTime)
			{
				m_Yaw += OffsetX*m_Sensitivity;
				m_Pitch += OffsetY*m_Sensitivity;
			}

			if (OffsetX != 0.0f && OffsetY != 0.0f)
				FirstTime = false;

			if (m_Pitch >= 90.0f || m_Pitch <= -90.0f)
			{
				m_Pitch = 89.9f * ((m_Pitch < 0.0f) ? -1.0f : 1.0f);
			}

			if (m_Yaw > 360.0f)
				m_Yaw -= 360.0f;
			
			else if (m_Yaw < -360.0f)
				m_Yaw += 360.0f;
		}

		if (Mouse::GetLeftKeyState() == KEY_UP)
		{
			Mouse::DisableCursor(false);
			FirstTime = true;
		}

		CalculateCameraAngle();
	}

	void Camera::DisableCameraMovement(bool pDisableCamera)
	{
		m_CameraLocked = pDisableCamera;
	}

	glm::mat4 & Camera::GetView()
	{
		m_View = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		return m_View;
	}

	glm::mat4 & Camera::GetProjection()
	{
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearPlane, m_FarPlane);
		return m_Projection;
	}

	void Camera::CalculateCameraAngle()
	{
		glm::vec3 Dir;
		Dir.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		Dir.y = sin(glm::radians(m_Pitch));
		Dir.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Front = glm::normalize(Dir);

	}

};