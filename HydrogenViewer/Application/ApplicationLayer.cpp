#include "ApplicationLayer.h"

void AppLayer::Setup()
{

	m_Scene = new Hydrogen::Scene();

	Hydrogen::Id Model = m_Scene->LoadModel("C:\\Users\\TheVoltage\\Desktop\\Dev\\Hydrogen\\HydrogenViewer\\Resources\\Models\\Hall\\scene.gltf");


	Hydrogen::Transform transform;
	transform.Scale = Hydrogen::Vec3(10.0f);
	transform.Translation = Hydrogen::Vec3(0.0f, 0.0f, 3.0f);
	m_Scene->GetModel(Model).SetTransform(transform);
	
	//Hydrogen::Id Model = m_Scene->NewModel("DefModel");
	//Hydrogen::MeshGenerator::GenerateSphere(m_Scene, Model, transform);


	Hydrogen::Core::PushScene(m_Scene);

}

void AppLayer::Shutdown()
{
	delete m_Scene;
}

void AppLayer::Event(float pDeltaTime)
{
	m_Scene->GetCamera().HandleCameraMovement(pDeltaTime);
	m_Scene->GetCamera().HandleCameraLooking();
}

void AppLayer::Update()
{

}


