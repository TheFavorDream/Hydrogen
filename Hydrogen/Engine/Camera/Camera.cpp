#include "Core/Core.h"
#include "Camera.h"
#include "HydPch.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

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

	void Camera::HandleCameraMovement(
		FrameEvent& pEvents		  ,
		uint32 		pForwardKey   ,//= GLFW_KEY_W,
		uint32 		pBackwardKey  ,//= GLFW_KEY_S,
		uint32 		pLeftKey 	  ,//= GLFW_KEY_A,
		uint32 		pRightKey 	  ,//= GLFW_KEY_D,
		uint32 		pUpKey 	      ,//= GLFW_KEY_SPACE,
		uint32 		pDownKey      ,//= GLFW_KEY_LEFT_SHIFT,
		uint32 		pSpeedKey 	   //= GLFW_KEY_LEFT_CONTROL
	) noexcept
	{
		
		if (m_CameraLocked)
			return;

		float DeltaTime = Core::GetDeltaTime();

		if (pEvents.QueueKeyState(pForwardKey) == KEY_DOWN)
		{
			m_Position += m_Front * m_Speed;// *DeltaTime;
		}
		if (pEvents.QueueKeyState(pBackwardKey) == KEY_DOWN)
		{
			m_Position += -m_Front * m_Speed;// *DeltaTime;
		}
		if (pEvents.QueueKeyState(pRightKey) == KEY_DOWN)
		{
			m_Position += glm::normalize(glm::cross(m_Front, m_Up))* m_Speed;// *DeltaTime;
		}
		if (pEvents.QueueKeyState(pLeftKey) == KEY_DOWN)
		{
			m_Position += -glm::normalize(glm::cross(m_Front, m_Up))* m_Speed;// *DeltaTime;
		}

		if (pEvents.QueueKeyState(pUpKey) == KEY_DOWN)
		{
			m_Position += m_Up * m_Speed;// *DeltaTime;
		}
		if (pEvents.QueueKeyState(pDownKey) == KEY_DOWN)
		{
			m_Position += -m_Up * m_Speed;// *DeltaTime;
		}
		
	}

	void Camera::HandleCameraLooking(
		VecD2 pCurserOffset
	) noexcept
	{
		static bool FirstTime = true;

		if (m_CameraLocked)
			return;

		float DeltaTime = Core::GetDeltaTime()/1000.0f;

		
		if (Mouse::MouseLeftKey == Hydrogen::KEY_DOWN)
		{
			if (FirstTime)
			{
				Hydrogen::Mouse::DisableCursor(true);
			}


			float OffsetX = (float)pCurserOffset.X;
			float OffsetY = -(float)pCurserOffset.Y;


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
		

		if (Mouse::MouseLeftKey == KEY_UP)
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

	float* Camera::GetViewPtr()
	{
		GetView();
		return glm::value_ptr(m_View);
	}

	float* Camera::GetProjectionPtr()
	{
		GetProjection();
		return glm::value_ptr(m_Projection);
	}

	glm::mat4& Camera::GetView()
	{
		m_View = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		return m_View;	
	}

	glm::mat4& Camera::GetProjection()
	{
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearPlane, m_FarPlane);
		m_Projection[1][1] *= -1; // y-flip for vulkan
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