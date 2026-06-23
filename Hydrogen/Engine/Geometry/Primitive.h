#pragma once


#include "Common.h"
#include "Core/ResourcePool.h"
#include "VecMath/Matrix/MatDef.h"
#include "Render/ElementBuffer.h"
#include "Render/VertexBuffer.h"
#include "Render/VertexArray.h"

namespace Hydrogen
{

	struct Primitive
	{
		Instance<VertexArray>   m_VertexArrays;
		Instance<VertexBuffer>	m_VertexBuffer;
		Instance<ElementBuffer> m_ElementBuffer;
		Instance<Material>      m_Material;
		Instance<Shader>        m_Shader;
		MatF4	   m_Transform;
		uint16     m_RenderingMode = 4;


		HYD Primitive();

	};
};