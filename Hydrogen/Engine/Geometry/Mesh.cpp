#include "Mesh.h"
#include "HydPch.h"


#include "Primitive.h"
#include "../Render/Renderer.h"
#include <utility>
#include <vulkan/vulkan_core.h>

namespace Hydrogen 
{


	Mesh Mesh::CreateGLTFMesh(
		const Xenon::Mesh& 	pMesh,
		uint32 			    pMeshID,
		GraphicsPipelineRef pPipeline
	) noexcept
	{
		//return Mesh();

		Mesh NewMesh;
		NewMesh.m_Name 	   = pMesh.GetName();
		NewMesh.m_ObjectID = pMeshID;
		for (auto& pri : pMesh)
		{
			Primitive primitive  = Primitive::CreatePrimitive(pri);	
			primitive.m_Pipeline = pPipeline;
			NewMesh.m_Primitives.push_back(std::move(primitive));
		}

		return std::move(NewMesh);
	}


	Mesh::Mesh() noexcept
		: m_Enable(true)
	{}


	Mesh::Mesh(
		const std::string&	     pName,
		std::vector<Primitive>&& pPrimitives
	) noexcept
	{
		m_Name			 = pName;
		m_Primitives	 = std::move(pPrimitives);
	}

	Mesh::Mesh(
		Mesh&& pOther
	) noexcept
	{
		m_Name		  = std::move(pOther.m_Name);
		m_Primitives  = std::move(pOther.m_Primitives);
		m_ModelMatrix = std::move(pOther.m_ModelMatrix);
		m_Enable      = pOther.m_Enable;
		m_ObjectID    = pOther.m_ObjectID;
	}

	Mesh::Mesh(
		const Mesh& pOther
	) noexcept
	{
		//m_Primitives = pOther.m_Primitives;
		m_Name       = pOther.m_Name;
	}

	Mesh& Mesh::operator=(
		const Mesh& pOther
	) noexcept
	{
		//m_Primitives = pOther.m_Primitives;
		m_Name       = pOther.m_Name;
		return *this;
	}


	Mesh& Mesh::operator=(
		Mesh&& pOther
	) noexcept
	{
		if (this == &pOther)
			return *this;

		m_Name        = std::move(pOther.m_Name);
		m_Primitives  = std::move(pOther.m_Primitives);
		m_ModelMatrix = std::move(pOther.m_ModelMatrix);
		m_Enable      = pOther.m_Enable;
		m_ObjectID    = pOther.m_ObjectID;
		
		return *this;
	}


	void Mesh::PushPremitive(
		Primitive&& pPrimitive
	) noexcept
	{
		m_Primitives.push_back(pPrimitive);
	}

	Primitive& Mesh::GetPrimitve(
		uint32 pIndex
	) noexcept
	{	
		ASSERT((pIndex < m_Primitives.size()), "Index Out of Range");
		return m_Primitives.at(pIndex);
	}

	Primitive Mesh::operator[](
		uint32 pIndex
	) noexcept
	{
		ASSERT((pIndex < m_Primitives.size()), "Index Out of Range");
		return m_Primitives.at(pIndex);
	}

	void Mesh::SetMeshRenderingStatus(
		bool pEnable
	) noexcept
	{
		m_Enable = pEnable;
	}


	InstructionSet Mesh::Render(
		const Transformation& 	  pTransform
	) noexcept
	{
		InstructionSet Instructions;

		//if (m_Transform.IsDirty)
		m_ModelMatrix = Transformation::CalculateMatrix(pTransform).Transpose();

		for (auto& pri : m_Primitives)
		{
			Instruction& ins = Instructions.NewInstruction();

			PushConstantData pc;

			pc.Size   = sizeof(uint32)*2;
			pc.Offset = 0;
			pc.Stages = HYD_SHADER_STAGE_VERTEX_BIT;

			*reinterpret_cast<uint32*>(&pc.Data) 				    = m_ObjectID;
			*reinterpret_cast<uint32*>((&pc.Data) + sizeof(uint32)) = 0; 

			ins.Topology	 = pri.m_Topology;
			ins.Vertices     = pri.m_VertexBuffer;
			ins.Indices      = pri.m_IndexBuffer;
			ins.Pipeline     = pri.m_Pipeline;

			ins.PushConstants.push_back(
				std::pair<PipelineLayoutRef,  PushConstantData>(
					pri.m_Pipeline->GetPipelineLayout(), pc
				)
			);
		}

		return std::move(Instructions);
	}


};