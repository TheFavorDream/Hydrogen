#include "ApplicationLayer.h"

void AppLayer::Setup()
{



	//Camera
	m_Camera.SetupCamera(45.0f, glm::vec3(0.0f, 0.0f, -3.0f), 2.0f, 0.1f, 20000.0f);

	//Shaders
	m_BasicShader.CreateShader("C:\\Users\\TheVoltage\\Desktop\\Dev\\Hydrogen\\HydrogenViewer\\Resources\\Shaders\\base.glsl");
//	m_MaterialShader.CreateShader("C:\\Users\\TheVoltage\\Desktop\\Dev\\Hydrogen\\HydrogenViewer\\Resources\\Shaders\\material.glsl");

	//m_Interior.reset(Hydrogen::GLTFLoader::Load("C:\\Users\\TheVoltage\\Desktop\\Dev\\Hydrogen\\HydrogenViewer\\Resources\\Models\\Hall\\scene.gltf"));
	m_Character.reset(Hydrogen::GLTFLoader::Load("C:\\Users\\TheVoltage\\Desktop\\Dev\\Hydrogen\\HydrogenViewer\\Resources\\Models\\Wednesday-2\\Wednesday.gltf"));
	
	glm::mat4 Transform = glm::mat4(1.0f);
	
	Transform = glm::scale(Transform, glm::vec3(20.0f));
	Transform = glm::translate(Transform, glm::vec3(0.0f));
	//m_Interior->BakeTransform(Transform);


	Transform = glm::mat4(1.0f);

	Transform = glm::scale(Transform, glm::vec3(5.0f));
	Transform = glm::translate(Transform, glm::vec3(0.0f));
	m_Character->BakeTransform(Transform);

	m_Character.reset(nullptr);
}

void AppLayer::Shutdown()
{

}

void AppLayer::Event(float pDeltaTime)
{

	//If UI doesn't want to capture input
	if (!Hydrogen::UICore::Self()->WantToCapture())
	{
		m_Camera.HandleCameraLooking();
		m_Camera.HandleCameraMovement(pDeltaTime);
	}
}

void AppLayer::Update()
{
	m_BasicShader.SetUniformMat4("View", glm::value_ptr(m_Camera.GetView()));
	m_BasicShader.SetUniformMat4("Projection", glm::value_ptr(m_Camera.GetProjection()));
}

void AppLayer::Render()
{
	//m_Interior->RenderModel(m_BasicShader);
//	m_Character->RenderModel(m_BasicShader);
}
