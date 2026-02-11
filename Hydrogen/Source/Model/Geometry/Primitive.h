#pragma once

#include "Common.h"
#include "ResourceHandler/BufferHandler.h"
#include "ResourceHandler/MaterialHandler.h"


namespace Hydrogen
{

	class Primitive
	{
	public:

		HYD  Primitive();
		HYD ~Primitive();
		
		HYD Primitive(const Primitive& pOther) = delete;
		HYD Primitive(Primitive&& pOther);
		
		HYD Primitive& operator=(const Primitive& pOther) = delete;
		HYD Primitive& operator=(Primitive&& pOther);

	private:
		Id		m_VertexArrays  =0;
		Id	    m_VertexBuffer  =0;
		Id      m_ElementBuffer =0;
		Id      m_Material      =0;

		uint32    m_RenderingMode = 4;
		Attribute m_Attributes;

		friend class Mesh;
		friend class GLTFLoader;
	};

};