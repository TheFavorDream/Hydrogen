#include "Core/Core.h"
#include "Camera.h"
#include "HydPch.h"

namespace Hydrogen
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

	void Camera::HandleCameraMovement()
	{

		if (m_CameraLocked)
			return;

		float DeltaTime = Core::GetDeltaTime();

		if (Hydrogen::Keyboard::GetKeyState(GLFW_KEY_W) == Hydrogen::KEY_DOWN)
		{
			m_Position += m_Front * m_Speed * DeltaTime;
		}
		if (Hydrogen::Keyboard::GetKeyState(GLFW_KEY_S) == Hydrogen::KEY_DOWN)
		{
			m_Position += -m_Front * m_Speed*DeltaTime;
		}
		if (Hydrogen::Keyboard::GetKeyState(GLFW_KEY_D) == Hydrogen::KEY_DOWN)
		{
			m_Position += glm::normalize(glm::cross(m_Front, m_Up))* m_Speed*DeltaTime;
		}
		if (Hydrogen::Keyboard::GetKeyState(GLFW_KEY_A) == Hydrogen::KEY_DOWN)
		{
			m_Position += -glm::normalize(glm::cross(m_Front, m_Up))* m_Speed*DeltaTime;
		}

		if (Hydrogen::Keyboard::GetKeyState(GLFW_KEY_SPACE) == Hydrogen::KEY_DOWN)
		{
			m_Position += m_Up * m_Speed*DeltaTime;
		}
		if (Hydrogen::Keyboard::GetKeyState(GLFW_KEY_LEFT_SHIFT) == Hydrogen::KEY_DOWN)
		{
			m_Position += -m_Up * m_Speed*DeltaTime;
		}
	}

	void Camera::HandleCameraLooking()
	{
		static bool FirstTime = true;

		if (m_CameraLocked)
			return;

		float DeltaTime = Core::GetDeltaTime()/1000.0f;

		if (Hydrogen::Mouse::GetLeftKeyState() == Hydrogen::KEY_DOWN)
		{
			if (FirstTime)
			{
				Hydrogen::Mouse::DisableCursor(true);
			}


			float OffsetX = Hydrogen::Mouse::GetOffsetX();
			float OffsetY = Hydrogen::Mouse::GetOffsetY();


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

		if (Hydrogen::Mouse::GetLeftKeyState() == Hydrogen::KEY_UP)
		{
			Hydrogen::Mouse::DisableCursor(false);
			FirstTime = true;
		}

		CalculateCameraAngle();
	}

	void Camera::DisableCameraMovement(bool pDisableCamera)
	{
		m_CameraLocked = pDisableCamera;
	}

	float* Camera::GetView()
	{
		m_View = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		return glm::value_ptr( m_View);
	}

	float* Camera::GetProjection()
	{
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearPlane, m_FarPlane);
		return glm::value_ptr(m_Projection);
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