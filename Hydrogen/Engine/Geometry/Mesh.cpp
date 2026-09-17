#include "Mesh.h"
#include "HydPch.h"


#include "Primitive.h"
#include "../Render/Renderer.h"
#include <vulkan/vulkan_core.h>

namespace Hydrogen 
{


	Mesh Mesh::CreateGLTFMesh(const Xenon::Mesh& pMesh, GraphicsPipelineRef pPipeline)
	{
		//return Mesh();

		Mesh NewMesh;
		NewMesh.m_Name = pMesh.GetName();
		
		for (auto& pri : pMesh)
		{
			Primitive primitive = Primitive::CreatePrimitive(pri);	
			primitive.m_Pipeline = pPipeline;
			NewMesh.m_Primitives.push_back(std::move(primitive));
		}

		return std::move(NewMesh);
	}


	Mesh::Mesh()
	{

	}


	Mesh::Mesh(const std::string& pName, std::vector<Primitive>&& pPrimitives)
	{
		m_Name			 = pName;
		m_Primitives	 = std::move(pPrimitives);
	}

	Mesh::Mesh(Mesh&& pOther)
	{
		m_Name		 = std::move(pOther.m_Name);
		m_Primitives = std::move(pOther.m_Primitives);
	}

	Mesh::Mesh(const Mesh& pOther)
	{
	
	}

	Mesh& Mesh::operator=(const Mesh& pOther)
	{
		return *this;
	}


	Mesh& Mesh::operator=(Mesh&& pOther)
	{
		if (this != &pOther)
		{
			m_Name = std::move(pOther.m_Name);
			m_Primitives = std::move(pOther.m_Primitives);
		}
		return *this;
	}

	uint32 Mesh::PushPremitive(const Primitive& pPrimitive)
	{
		m_Primitives.emplace_back(pPrimitive);
		return HYD_OK;
	}

	Primitive& Mesh::GetPrimitve(uint64 pIndex)
	{
		
		ASSERT((pIndex < m_Primitives.size()), "Index Out of Range");
		return m_Primitives[pIndex];
	}



	void Mesh::Render(
		std::vector<Instruction>& pInstructionSet,
		UniformRef		      	  pUniform,
		LightCollection&          pLights,
		const Transformation& 	  pTransform
	) noexcept
	{

		//if (m_Transform.IsDirty)
		MatF4 Matrix = Transformation::CalculateMatrix(pTransform);

		for (auto& pri : m_Primitives)
		{
			pri.m_Transform = Matrix.Transpose();
			

			Renderer::Self().AccessUniformBuffer(pUniform).UploadData(pri.m_Transform.GetPointer(), sizeof(MatF4), 0);

			
			Instruction instruction;
			instruction.Vertices    = pri.m_VertexBuffer;
			instruction.Indices     = pri.m_IndexBuffer;
			instruction.Pipeline    = pri.m_Pipeline;
			instruction.Uniform     = pUniform;
			instruction.MaterialPtr = &pri.m_Material;
			instruction.Light       = &pLights;
			
			pInstructionSet.push_back(
				std::move(instruction)
			);
		}
	}


};