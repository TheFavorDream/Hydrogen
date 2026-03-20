#include "MeshGenerator.h"
#include "HydPch.h"



namespace Hydrogen
{

	uint32 MeshGenerator::GenerateCube(Scene* pTargetScene, Id pTargetModel, const Transform& pTransform)
	{

		//Calculate The Vertices:

		std::vector<Vertex> Vertices = 
		{
			
			{Vec3(-0.5f,  0.5f,  0.5f), Vec3(0.0f),  Vec2(0.0f, 0.0f)},
			{Vec3(-0.5f,  0.5f, -0.5f), Vec3(0.0f),  Vec2(0.0f, 0.0f)},
			{Vec3(-0.5f, -0.5f, -0.5f), Vec3(0.0f),  Vec2(0.0f, 0.0f)},
			{Vec3(-0.5f, -0.5f,  0.5f), Vec3(0.0f),  Vec2(0.0f, 0.0f)},
			{Vec3( 0.5f,  0.5f,  0.5f), Vec3(0.0f),  Vec2(0.0f, 0.0f)},
			{Vec3( 0.5f, -0.5f,  0.5f), Vec3(0.0f),  Vec2(0.0f, 0.0f)},
			{Vec3( 0.5f, -0.5f, -0.5f), Vec3(0.0f),  Vec2(0.0f, 0.0f)},
			{Vec3( 0.5f,  0.5f, -0.5f), Vec3(0.0f),  Vec2(0.0f, 0.0f)}
		};

		std::vector<uint16> Indices
		{
			0, 1, 2,
			2, 3, 0,
			
			4, 5, 6,
			6, 7, 4,
			
			0, 4, 7,
			7, 1, 0,
			
			3, 2, 6,
			6, 5, 3,
			
			1, 7, 6,
			6, 2, 1,
			
			0, 3, 5,
			5, 4, 0
		};

		Id mesh = pTargetScene->CreateMesh(pTargetModel, Vertices, Indices);
		pTargetScene->GetModel(pTargetModel).GetMesh(mesh).SetTransform(pTransform);
		return HYD_OK;
	}


	uint32 MeshGenerator::GenerateSphere(Scene* pTargetScene, Id pTargetModel, const Transform& pTransform)
	{
		std::vector<Vertex> Vertices;
		std::vector<uint16> Indices;

		
		uint16 SecNum   = 30;
		uint16 StackNum = 30;
		uint16 SecSize = 360 / SecNum;
		uint16 StackSize = 180 / StackNum;

		uint16 Index = 0;

		for (int Stack = 0; Stack < StackNum; Stack += 1)
		{
			for (int Sec = 0; Sec < SecNum; Sec += 1)
			{
				float Z = SinA(Sec*SecSize)*SinA(Stack*StackSize);
				float X = CosA(Sec*SecSize)*SinA(Stack*StackSize);
				float Y = CosA(Stack*StackSize);

				Vertices.push_back(
					{ Vec3(X, Y, Z), Vec3(0.0f), Vec2(0.0f) }
				);
				Indices.emplace_back(Index);
				Index++;
			}
		}

		

		Id meshID = pTargetScene->CreateMesh(pTargetModel, Vertices, Indices);
		Mesh& mesh = pTargetScene->GetModel(pTargetModel).GetMesh(meshID); 
		mesh.SetTransform(pTransform);
		mesh.GetPrimitve(0).m_RenderingMode = GL_POINTS;
		return HYD_OK;

	}
	uint32 MeshGenerator::GenerateCone(Scene* pTargetScene, Id pTargetModel, const Transform& pTransform)
	{
		return HYD_OK;

	}
	uint32 MeshGenerator::GeneratePlane(Scene* pTargetScene, Id pTargetModel, const Transform& pTransform)
	{
		return HYD_OK;
	}
};