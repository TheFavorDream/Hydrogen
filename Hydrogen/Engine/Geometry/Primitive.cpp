#include "Primitive.h"
#include "../Render/Renderer.h"
#include <algorithm>
#include <utility>
#include <vulkan/vulkan_core.h>

namespace Hydrogen
{


	Primitive Primitive::CreatePrimitive(
		Xenon::Primitive& pPrimitive
	) noexcept
    {
        Primitive NewPrimitive;

		if (pPrimitive.HasPosition())
		{
			Xenon::BinaryData Positions = pPrimitive.GetPosition().RetriveData();
			NewPrimitive.m_Positions.resize(Positions.ByteLength / sizeof(VecF3));
			memmove(NewPrimitive.m_Positions.data(), Positions.Ptr, Positions.ByteLength);
		}

		if (pPrimitive.HasNormal())
		{
			Xenon::BinaryData Normals = pPrimitive.GetNormal().RetriveData();
			NewPrimitive.m_Normals.resize(Normals.ByteLength / sizeof(VecF3));
			memmove(NewPrimitive.m_Normals.data(), Normals.Ptr, Normals.ByteLength);		
		}

		if (pPrimitive.HasTangent())
		{
			Xenon::BinaryData Tangents = pPrimitive.GetTangent().RetriveData();
			NewPrimitive.m_Tangent.resize(Tangents.ByteLength / sizeof(VecF4));
			memmove(NewPrimitive.m_Tangent.data(), Tangents.Ptr, Tangents.ByteLength);
		}

		if (pPrimitive.HasTexCoord_0())
		{
			Xenon::BinaryData TexCoord00 = pPrimitive.GetTexCoord_0().RetriveData();
			NewPrimitive.m_TexCoord00.resize(TexCoord00.ByteLength / sizeof(VecF2));
			memmove(NewPrimitive.m_TexCoord00.data(), TexCoord00.Ptr, TexCoord00.ByteLength);
		}

		if (pPrimitive.HasTexCoord_1())
		{
			Xenon::BinaryData TexCoord01 = pPrimitive.GetTexCoord_1().RetriveData();
			NewPrimitive.m_TexCoord01.resize(TexCoord01.ByteLength / sizeof(VecF2));
			memmove(NewPrimitive.m_TexCoord01.data(), TexCoord01.Ptr, TexCoord01.ByteLength);
		}

		if (pPrimitive.HasColor())
		{
			Xenon::BinaryData Color = pPrimitive.GetColor().RetriveData();
			NewPrimitive.m_Color00.resize(Color.ByteLength / sizeof(VecF3));
			memmove(NewPrimitive.m_Color00.data(), Color.Ptr, Color.ByteLength);
		}

		if (pPrimitive.HasIndices())
		{
			Xenon::BinaryData Indices = pPrimitive.GetIndices().RetriveData();
			NewPrimitive.m_IndicesRaw = Buffer(Indices.ByteLength);
			memcpy(NewPrimitive.m_IndicesRaw.AccessPtr(), Indices.Ptr, Indices.ByteLength); 

			NewPrimitive.m_IndexCount = pPrimitive.GetIndices().Count;
			NewPrimitive.m_IndexType  = pPrimitive.GetIndices().GetVulkanIndexTypeEnum(pPrimitive.GetIndices().ComponentType);
		}

		NewPrimitive.UpdateVertexData();

		switch(pPrimitive.GetTopology())
		{
			case 0: //Points
				NewPrimitive.m_Topology = HYD_PRIMITIVE_TOPOLOGY_POINT_LIST;
			break;
			case 1: //Lines
				NewPrimitive.m_Topology = HYD_PRIMITIVE_TOPOLOGY_LINE_LIST;
			break;
			case 4: //Triangles
				NewPrimitive.m_Topology = HYD_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			break;
			case 5: //Triangle Strips
				NewPrimitive.m_Topology = HYD_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			break;
			case 6: //Triangle Fan
				NewPrimitive.m_Topology = HYD_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
			break;
		}

        return std::move(NewPrimitive);
    }


	Primitive::Primitive(
		const Primitive& pOther
	) noexcept
    {

    }

	Primitive::Primitive(
		Primitive&& pOther
	) noexcept
		: m_Material(    std::move(pOther.m_Material)),
		  m_Pipeline(    std::move(pOther.m_Pipeline)),
		  m_Attributes(pOther.m_Attributes),
		  m_VertexBuffer(std::move(pOther.m_VertexBuffer)),
		  m_IndexBuffer( std::move(pOther.m_IndexBuffer)),
		  m_Positions(   std::move(pOther.m_Positions)),
		  m_Normals(     std::move(pOther.m_Normals)),
		  m_Tangent(     std::move(pOther.m_Tangent)),
		  m_TexCoord00(  std::move(pOther.m_TexCoord00)),
		  m_TexCoord01(  std::move(pOther.m_TexCoord01)),
		  m_Color00(	 std::move(pOther.m_Color00)),
		  m_IndicesRaw(  std::move(pOther.m_IndicesRaw)),
		  m_IndexCount(  pOther.m_IndexCount),
		  m_IndexType (  pOther.m_IndexType),
		  m_Topology  (  pOther.m_Topology)
    {	
    }


	/*
		Purpose: Uploads the Vertex Data to the GPU Buffers, if they don't exist, creates them
	*/
	uint32 Primitive::UpdateVertexData() noexcept
	{   
        

		if (m_IndexBuffer.IsNull())
		{
			m_IndexBuffer = Renderer::Self().InstanceIndexBuffer();
			m_IndexBuffer->CreateBuffer(
				m_IndicesRaw.Length(), 
				m_IndexCount,
				VkIndexType(m_IndexType)
			);
		}
        
		Internal::Vulkan::StagingBuffer stagingBuffer;
		stagingBuffer.CreateBuffer(m_IndicesRaw.Length());
		stagingBuffer.UploadData(m_IndicesRaw.GetPtr(), m_IndicesRaw.Length());
		stagingBuffer.CopyBuffer(*m_IndexBuffer);
		stagingBuffer.DestroyBuffer();
		

		uint32 VertexCount = std::max({
			m_Positions.size(),
			m_Normals.size(),
			m_Tangent.size(),
			m_TexCoord00.size(),
			m_TexCoord01.size()
	});

		std::vector<Vertex> Vertices; Vertices.resize(VertexCount);

		for (uint32 PosIndex = 0 ; PosIndex < m_Positions.size() ; ++PosIndex)
		{
			Vertices.at(PosIndex).Position   = m_Positions.at(PosIndex);
		}

		for (uint32 NormIndex = 0 ; NormIndex < m_Normals.size() ; ++NormIndex)
		{
			Vertices.at(NormIndex).Normal     = m_Normals.at(NormIndex);
		}
		
		for (uint32 TanIndex = 0 ; TanIndex < m_Tangent.size() ; ++TanIndex)
		{
			Vertices.at(TanIndex).Tangent    = m_Tangent.at(TanIndex);
		}
				
		for (uint32 TexCoordIndex = 0 ; TexCoordIndex < m_TexCoord00.size() ; ++TexCoordIndex)
		{
			Vertices.at(TexCoordIndex).TexCoord00 = m_TexCoord00.at(TexCoordIndex);
		}	
		
		for (uint32 TexCoordIndex = 0 ; TexCoordIndex < m_TexCoord01.size() ; ++TexCoordIndex)
		{
			Vertices.at(TexCoordIndex).TexCoord01 = m_TexCoord01.at(TexCoordIndex);
		}


		uint32 DataSize = VertexCount * sizeof(Vertex);
	
		//Create the Vertex Buffer
		if (m_VertexBuffer.IsNull())
		{
			m_VertexBuffer  = Renderer::Self().InstanceVertexBuffer(); 
			m_VertexBuffer->CreateBuffer(DataSize);

		}

		//Vertex Buffer:
		stagingBuffer.CreateBuffer(DataSize);
		stagingBuffer.UploadData(Vertices.data(), DataSize);
		stagingBuffer.CopyBuffer(*m_VertexBuffer);
		stagingBuffer.DestroyBuffer();

		Vertices.clear();


		return HYD_OK;
	}



	/*
		Purpose: Set the Material used by this primitive
	*/
	
	void Primitive::SetMaterial(
		Material&& pMaterial
	) noexcept
	{
		m_Material = std::move(pMaterial);
	}

};