#include "ApplicationLayer.h"

void AppLayer::Setup()
{



	//Camera
	m_Camera.SetupCamera(45.0f, glm::vec3(0.0f, 0.0f, -3.0f), 2.0f, 0.1f, 20000.0f);

	//Shaders
	m_BasicShader.CreateShader("C:\\Users\\TheVoltage\\Desktop\\Dev\\Hydrogen\\HydrogenViewer\\Resources\\Shaders\\base.glsl");
	m_MaterialShader.CreateShader("C:\\Users\\TheVoltage\\Desktop\\Dev\\Hydrogen\\HydrogenViewer\\Resources\\Shaders\\material.glsl");


	m_Room = Hydrogen::GLTFLoader::Load("C:\\Users\\TheVoltage\\Desktop\\Dev\\Hydrogen\\HydrogenViewer\\Resources\\Models\\StudyRoom\\scene.gltf");
	m_Char = Hydrogen::GLTFLoader::Load("C:\\Users\\TheVoltage\\Desktop\\Dev\\Hydrogen\\HydrogenViewer\\Resources\\Models\\Wednesday-2\\Wednesday.gltf");

	glm::mat4 Transform = glm::mat4(1.0f);
	
	Transform = glm::scale(Transform, glm::vec3(3.0f));
	Transform = glm::translate(Transform, glm::vec3(0.0f));
	m_Room->SetTransform(Transform);

	Transform = glm::mat4(1.0f);
	Transform = glm::scale(Transform, glm::vec3(6.0f));
	Transform = glm::translate(Transform, glm::vec3(90.0f/6.0f, 0.0f, -140.0f/6.0f));
	m_Char->SetTransform(Transform);


	Hydrogen::Log::SetInfo(Hydrogen::Log::FmtStr("Int:%i Float:%f  String:%s", 66 , 3.14159f, "Hello"));
	Hydrogen::Log::SetWarning("This is a Warning");
	Hydrogen::Log::SetError("This is an Error");


}

void AppLayer::Shutdown()
{
	Hydrogen::GLTFLoader::Free(&m_Room);
	Hydrogen::GLTFLoader::Free(&m_Char);

}

void AppLayer::Event(float pDeltaTime)
{
	
	if (Hydrogen::Window::GetCurrentWindow()->IsMouseInViewPort())
	{
		m_Camera.HandleCameraLooking();
		m_Camera.HandleCameraMovement(pDeltaTime);
	}
	if (Hydrogen::Keyboard::IsKeyPressed(GLFW_KEY_T))
		std::cout << m_Camera.GetCameraPos().x << '\t' << m_Camera.GetCameraPos().y << '\t' << m_Camera.GetCameraPos().z << '\n';

}

void AppLayer::Update()
{
	m_BasicShader.SetUniformMat4("View", glm::value_ptr(m_Camera.GetView()));
	m_BasicShader.SetUniformMat4("Projection", glm::value_ptr(m_Camera.GetProjection()));

	m_MaterialShader.SetUniformMat4("View", glm::value_ptr(m_Camera.GetView()));
	m_MaterialShader.SetUniformMat4("Projection", glm::value_ptr(m_Camera.GetProjection()));

}

void AppLayer::Render()
{
	m_Room->RenderModel(m_BasicShader);
	m_Char->RenderModel(m_MaterialShader);
}
