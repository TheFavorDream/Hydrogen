#pragma once


#include "../Common.h"
#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "../VecMath/Vector/Vectors.h"
#include "Event/Keyboard.h"
#include "Event/Mouse.h"


namespace Hydrogen
{
	class Camera
	{
	public:

		

		HYD void SetupCamera(float pFOV, glm::vec3 pPosition, float pAspectRatio = 2.0f, float pNearPlane = 0.1, float pFarPlane = 100.0f);
		HYD void HandleCameraMovement();
		HYD void HandleCameraLooking();
		 
		HYD void DisableCameraMovement(bool pDisableCamera);
		 
		HYD float* GetViewPtr();
		HYD float* GetProjectionPtr();

		HYD glm::mat4& GetView();
		HYD glm::mat4& GetProjection();

		HYD inline VecF3 GetCameraPos()  { return VecF3(m_Position.x, m_Position.y, m_Position.z); }
		HYD inline VecF3 GetCameraLook() { return VecF3(m_Front.x, m_Front.y, m_Front.z); }
		HYD inline float GetZNear() const {return m_NearPlane;}
		HYD inline float GetZFar()  const {return m_FarPlane;}
		HYD inline float GetFOV()  const {return m_FOV;}

	private:

		HYD void CalculateCameraAngle();

	private:

		float m_FOV = 0.0f;
		float m_NearPlane = 0.1f;
		float m_FarPlane = 1000.0f;
		float m_AspectRatio = 0.0f;
		float m_Speed = 0.05f;

		float m_Sensitivity = 0.1f;

		bool m_CameraLocked = false;
		float m_Yaw = 0.0f;
		float m_Pitch = 0.0f;

		glm::vec3 m_Position;
		glm::vec3 m_Front;
		glm::vec3 m_Up;
		glm::vec3 m_Right;

		glm::mat4 m_View;
		glm::mat4 m_Projection;
	};
};