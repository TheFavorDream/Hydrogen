#pragma once


#include "../Common.h"
#include "../Core/ResourcePool.h"
#include "../VecMath/Matrix/MatDef.h"
#include "../Render/Material/Material.h"
#include "../Render/Vulkan/Buffer.h"
#include "../Render/Vulkan/VertexAttribute.h"
#include "Xenon/include/Xenon.h"


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



	std::vector<Vertex> RepackVertexData(
		Xenon::Primitive& pPrimitive
	) noexcept;


	
	class Primitive
	{
	public:
		MatF4				    					m_Transform;
		Material									m_Material;
		GraphicsPipelineRef    			    		m_Pipeline;
		Internal::Vulkan::VertexAttribute 		    m_Attributes;
		Instance<Internal::Vulkan::VertexBuffer>    m_VertexBuffer;
		Instance<Internal::Vulkan::IndexBuffer>     m_IndexBuffer;

		HYD  Primitive() 					   noexcept;
		HYD	~Primitive() 					   noexcept;
		HYD Primitive(const Primitive& pOther) noexcept;
		HYD Primitive(Primitive&& pOther) 	   noexcept;



		static Primitive CreatePrimitive(Xenon::Primitive& pPrimitive) noexcept;
	};
};