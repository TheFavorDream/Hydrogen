#include "ApplicationLayer.h"

using namespace Hydrogen;

void AppLayer::Setup()
{

	m_World = Memory::Allocate<World>();
	Core::PushWorld(m_World.Ptr);

	
	//m_World->LoadGltf("C:\\Users\\TheVoltage\\Desktop\\Dev\\Hydrogen\\SandBox\\Resources\\Models\\Scene\\scene.gltf");

	//m_Scene->GetModel(model).BakeTransform(Hydrogen::VecF3(1.5f), Hydrogen::VecF4(0.0f, 0.0f, 0.0f, 1.0f), Hydrogen::Vec3(0.0f, 0.25f, 0.0f));
	
}

void AppLayer::Shutdown()
{
	m_World.Delete();
}

void AppLayer::Event()
{
	m_World->Update();
	//m_Scene->GetCamera().HandleCameraMovement();
	//m_Scene->GetCamera().HandleCameraLooking();
}

void AppLayer::Update()
{

	m_World->Process();
	m_World->Render();

}


