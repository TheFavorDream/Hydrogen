#pragma once


#include "../Common.h"
#include "../Core/ResourcePool.h"
#include "../VecMath/Matrix/MatDef.h"
#include "../Render/Material/Material.h"
#include "../Render/Vulkan/Buffer.h"
#include "../Render/Vulkan/VertexAttribute.h"
#include "Xenon/include/Xenon.h"
#include <vector>


namespace Hydrogen
{



	struct Vertex
	{
		VecF3 Position;
		VecF3 Normal;
		VecF4 Tangent;
		VecF2 TexCoord00;
		VecF2 TexCoord01;
	};





	class Primitive
	{
	public:
		static Primitive CreatePrimitive(
			Xenon::Primitive& pPrimitive
		) noexcept;
	
	public:
		HYD  Primitive() = default;
		HYD ~Primitive() = default;
		
		HYD  Primitive(
			const Primitive& pOther
		) noexcept;
		
		HYD  Primitive(
			Primitive&& pOther
		) noexcept;


		/*
			Purpose: Uploads the Vertex Data to the GPU Buffers, if they don't exist, creates them
		*/
		HYD uint32 UpdateVertexData() noexcept;

		/*
			Purpose: Set the Material used by this primitive
		*/

		HYD void SetMaterial(
			Material&& pMaterial
		) noexcept;

		inline const std::vector<VecF3>&  GetPosition()   const { return m_Positions; }
		inline const std::vector<VecF3>&  GetNormal()     const { return m_Normals; }
		inline const std::vector<VecF4>&  GetTangent()    const { return m_Tangent; }
		inline const std::vector<VecF2>&  GetTexCoord00() const { return m_TexCoord00; }
		inline const std::vector<VecF2>&  GetTexCoord01() const { return m_TexCoord01; }
		inline const std::vector<VecF3>&  GetColor()      const { return m_Color00; }
		

	protected:
		Material							m_Material;
		GraphicsPipelineRef    			    m_Pipeline;
		Internal::Vulkan::VertexAttribute   m_Attributes;
		VertexBufferRef    					m_VertexBuffer;
		IndexBufferRef     					m_IndexBuffer;

		//Geometric Data 
		std::vector<VecF3> 				    m_Positions; 
		std::vector<VecF3> 				    m_Normals;
		std::vector<VecF4> 				    m_Tangent;
		std::vector<VecF2> 				    m_TexCoord00;
		std::vector<VecF2> 				    m_TexCoord01;
		std::vector<VecF3> 				    m_Color00;

		Buffer								m_IndicesRaw;
		uint32 								m_IndexCount;
		uint32 								m_IndexType;
		PrimitiveTopology 					m_Topology;
		
	protected:
		friend class Mesh;
	};
};