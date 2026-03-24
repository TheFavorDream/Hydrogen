#include "ApplicationLayer.h"

void AppLayer::Setup()
{

	m_Scene = new Hydrogen::Scene();

	Hydrogen::Id Model = m_Scene->LoadModel("C:\\Users\\TheVoltage\\Desktop\\Dev\\Hydrogen\\SandBox\\Resources\\Models\\Hall\\scene.gltf");


	//Hydrogen::Transform transform;
	//transform.Scale = Hydrogen::Vec3(5.0f);
	//
	//Hydrogen::Id Model = m_Scene->NewModel("DefModel");
	//m_Scene->GetModel(Model).SetTransform(transform);
	//
	//
	//for (int k = 0; k < 10; k++)
	//{
	//	for (int j = 0; j < 10; j++)
	//	{
	//		for (int i = 0; i < 10; ++i)
	//		{
	//			transform.Translation = Hydrogen::Vec3(float(i * 10), float(k*10), float(j * 10));
	//			Hydrogen::MeshGenerator::GenerateCube(m_Scene, Model, transform);
	//		}
	//	}
	//}

	Hydrogen::Core::PushScene(m_Scene);

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


