#include "Primitive.h"
#include "../Render/Renderer.h"
#include <vulkan/vulkan_core.h>

namespace Hydrogen
{


	Primitive::Primitive() noexcept
    {

    }

	Primitive::~Primitive() noexcept
	{
		//m_IndexBuffer.ResetWithoutRefDrop();
		//m_VertexBuffer.ResetWithoutRefDrop();
		//m_Pipeline.ResetWithoutRefDrop();
	}

	Primitive::Primitive(const Primitive& pOther) noexcept
    {

    }

	Primitive::Primitive(Primitive&& pOther) noexcept
    {
		m_Transform = std::move(pOther.m_Transform);
		m_Material  = std::move(pOther.m_Material);

		m_Pipeline        = std::move(pOther.m_Pipeline);
        
		
		m_Attributes    = pOther.m_Attributes;
		m_VertexBuffer  = std::move(pOther.m_VertexBuffer);
		m_IndexBuffer   = std::move(pOther.m_IndexBuffer);
    }


		
	Primitive Primitive::CreatePrimitive(Xenon::Primitive& pPrimitive) noexcept
    {

        Primitive NewPrimitive;

        
		const Xenon::Accessor&   IndexAccessor     = pPrimitive.GetIndices();
		Xenon::BinaryData        Data 			   = IndexAccessor.RetriveData();
        
        NewPrimitive.m_VertexBuffer  = Renderer::Self().InstanceVertexBuffer(); 
        NewPrimitive.m_IndexBuffer = Renderer::Self().InstanceIndexBuffer();
        


		NewPrimitive.m_IndexBuffer->CreateBuffer(
			Data.ByteLength, 
			IndexAccessor.Count,
			(VkIndexType)Xenon::Accessor::GetVulkanIndexTypeEnum(IndexAccessor.ComponentType));
        
		Internal::Vulkan::StagingBuffer stagingBuffer;
		stagingBuffer.CreateBuffer(Data.ByteLength);
		stagingBuffer.UploadData(Data.Ptr, Data.ByteLength);
		stagingBuffer.CopyBuffer(*NewPrimitive.m_IndexBuffer);
		stagingBuffer.DestroyBuffer();
		Data.Free();
		
		
		//Vertex Buffer:
		std::vector<Vertex> Vertices = std::move(RepackVertexData(pPrimitive));
		uint32 DataSize = Vertices.size() * sizeof(Vertex);

		NewPrimitive.m_VertexBuffer->CreateBuffer(DataSize);
		stagingBuffer.CreateBuffer(DataSize);
		stagingBuffer.UploadData(Vertices.data(), DataSize);
		stagingBuffer.CopyBuffer(*NewPrimitive.m_VertexBuffer);
		stagingBuffer.DestroyBuffer();

		Vertices.clear();

        return std::move(NewPrimitive);
    }



	std::vector<Vertex> RepackVertexData(
		Xenon::Primitive& pPrimitive
	) noexcept
	{
		std::vector<Vertex> Vertices;

		Xenon::BinaryData PosData     = pPrimitive.GetPosition().RetriveData();
		Xenon::BinaryData NormalData; 
		Xenon::BinaryData TangentData;
		Xenon::BinaryData TexCoord0Data;
		Xenon::BinaryData TexCoord1Data;


		if (pPrimitive.HasNormal())
			NormalData  = pPrimitive.GetNormal().RetriveData();

		if (pPrimitive.HasTangent())
			TangentData = pPrimitive.GetTangent().RetriveData();
		
		if (pPrimitive.HasTexCoord_0())
			TexCoord0Data = pPrimitive.GetTexCoord_0().RetriveData();

		if (pPrimitive.HasTexCoord_1())
			TexCoord1Data = pPrimitive.GetTexCoord_1().RetriveData();

		uint32 Vec3Offset   = 0;
		uint32 Vec4Offset   = 0;
		uint32 Vec2Offset   = 0;

		while (Vec3Offset <= PosData.ByteLength)
		{
			Vertex vertex;

			memcpy((void*)&vertex.Position, (void*)(PosData.Ptr+Vec3Offset), sizeof(VecF3));

			if (NormalData.Ptr)
				memcpy((void*)&vertex.Normal, (void*)(NormalData.Ptr+Vec3Offset), sizeof(VecF3));
			
			if (TangentData.Ptr)
				memcpy((void*)&vertex.Tangent, (void*)(TangentData.Ptr+Vec4Offset), sizeof(VecF4));

			if (TexCoord0Data.Ptr)
				memcpy((void*)&vertex.TexCoord00, (void*)(TexCoord0Data.Ptr+Vec2Offset), sizeof(VecF2));

			if (TexCoord1Data.Ptr)
				memcpy((void*)&vertex.TexCoord01, (void*)(TexCoord1Data.Ptr+Vec2Offset), sizeof(VecF2));

			
			Vertices.push_back(vertex);

			Vec4Offset  += sizeof(VecF4);
			Vec3Offset  += sizeof(VecF3);
			Vec2Offset  += sizeof(VecF2);
		}
		
		return std::move(Vertices);
	}

};