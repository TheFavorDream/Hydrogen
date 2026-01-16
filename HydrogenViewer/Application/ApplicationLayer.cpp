#include "ApplicationLayer.h"

void AppLayer::Setup()
{

	float CommonRatio = 3.0f;

	//Camera
	m_Camera.SetupCamera(45.0f, glm::vec3(20.0f*CommonRatio, 60.0f*CommonRatio, -94.0f*CommonRatio), 2.0f, 0.1f, 20000.0f);
	m_BaseShader.CreateShader("C:\\Users\\TheVoltage\\Desktop\\Dev\\Hydrogen\\HydrogenViewer\\Resources\\Shaders\\base.glsl");
	m_CharacterShader.CreateShader("C:\\Users\\TheVoltage\\Desktop\\Dev\\Hydrogen\\HydrogenViewer\\Resources\\Shaders\\Character.glsl");

	m_Model = Hydrogen::Loader::Load("C:\\Users\\TheVoltage\\Desktop\\Dev\\Hydrogen\\HydrogenViewer\\Resources\\Models\\StudyRoom\\scene.gltf");
	m_CharcterModel = Hydrogen::Loader::Load("C:\\Users\\TheVoltage\\Desktop\\Dev\\Hydrogen\\HydrogenViewer\\Resources\\Models\\Wednesday-2\\Wednesday.gltf");

	m_CharModelBaseColor.CreateTexture("C:\\Users\\TheVoltage\\Desktop\\Dev\\Hydrogen\\HydrogenViewer\\Resources\\Models\\Wednesday-2\\Body_baseColor.png");



	m_ModelMatrix = Hydrogen::Scale(m_ModelMatrix, Hydrogen::Vec3(2.0f*CommonRatio));
	m_ModelMatrix = Hydrogen::Translation(m_ModelMatrix, Hydrogen::Vec3(0.0f*CommonRatio));

	m_CharMatrix = Hydrogen::Scale(m_CharMatrix, Hydrogen::Vec3(4.0f*CommonRatio));
	m_CharMatrix = Hydrogen::Translation(m_CharMatrix, Hydrogen::Vec3(83.0f*CommonRatio, 0.0f*CommonRatio, -104.0f*CommonRatio));

	m_CharacterShader.SetUniformInt1("TEX", 0);
}

void AppLayer::Shutdown()
{

}

void AppLayer::Event(float pDeltaTime)
{
	
		//std::cout << m_Camera.GetCameraPos().x << '\t' << m_Camera.GetCameraPos().y << '\t' << m_Camera.GetCameraPos().z << '\n';
	if (Hydrogen::Window::GetCurrentWindow()->IsMouseInViewPort())
	{
		m_Camera.HandleCameraLooking();
		m_Camera.HandleCameraMovement(pDeltaTime);
	}

}

void AppLayer::Update()
{
	m_BaseShader.SetUniformMat4("View", glm::value_ptr(m_Camera.GetView()));
	m_BaseShader.SetUniformMat4("Projection", glm::value_ptr(m_Camera.GetProjection()));

	m_CharacterShader.SetUniformMat4("View", glm::value_ptr(m_Camera.GetView()));
	m_CharacterShader.SetUniformMat4("Projection", glm::value_ptr(m_Camera.GetProjection()));

}

void AppLayer::Render()
{
	m_Model->RenderScene(m_BaseShader, 0, &m_ModelMatrix);

	m_CharModelBaseColor.Bind();
	m_CharcterModel->RenderScene(m_CharacterShader, 0, &m_CharMatrix);
	m_CharModelBaseColor.Unbind();
}
