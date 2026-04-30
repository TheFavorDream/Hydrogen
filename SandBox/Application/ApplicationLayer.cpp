#include "ApplicationLayer.h"

using namespace Hydrogen;

void AppLayer::Setup()
{

	m_Scene = ResourcePool<Scene>::New();
	Core::PushScene(m_Scene);

	
	//model = m_Scene->LoadModel("C:\\Users\\TheVoltage\\Desktop\\Dev\\Hydrogen\\SandBox\\Resources\\Models\\JustACube.gltf");
	model = m_Scene->LoadModel("C:\\Users\\TheVoltage\\Desktop\\Dev\\Hydrogen\\SandBox\\Resources\\Models\\Scene\\scene.gltf");

	m_Scene->GetModel(model).BakeTransform(Hydrogen::VecF3(1.5f), Hydrogen::VecF4(0.0f, 0.0f, 0.0f, 1.0f), Hydrogen::Vec3(0.0f, 0.25f, 0.0f));
	
}

void AppLayer::Shutdown()
{
	delete m_Scene;
}

void AppLayer::Event()
{
	m_Scene->GetCamera().HandleCameraMovement();
	m_Scene->GetCamera().HandleCameraLooking();
}

void AppLayer::Update()
{



}


