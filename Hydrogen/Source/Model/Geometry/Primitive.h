#pragma once

#include "Common.h"
#include "Platform/OpenGL/Buffer.h"
#include "Platform/OpenGL/VertexArray.h"
#include "Model/Material/Material.h"


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
		VertexArray m_VertexArrays;
		Buffer	    m_VertexBuffer;
		Buffer      m_ElementBuffer;
		Material    m_Material;

		uint32 m_RenderingMode = 4;
		Attribute m_Attributes;

		friend class Mesh;
		friend class GLTFLoader;
	};

};