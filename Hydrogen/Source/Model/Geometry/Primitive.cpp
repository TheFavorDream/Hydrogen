#include "Primitive.h"

namespace Hydrogen
{



	Primitive::Primitive()
	{

	}

	Primitive::~Primitive()
	{
		m_VertexBuffer.DestroyBuffer();
		m_ElementBuffer.DestroyBuffer();
		m_VertexArrays.DestroyVertexArray();
		
	}

	Primitive::Primitive(Primitive && pOther)
	{
		this->m_Attributes = pOther.m_Attributes;
		this->m_RenderingMode = pOther.m_RenderingMode;

		this->m_VertexBuffer  = std::move(pOther.m_VertexBuffer);
		this->m_ElementBuffer = std::move(pOther.m_ElementBuffer);
		this->m_VertexArrays  = std::move(pOther.m_VertexArrays);
		this->m_Material	  = std::move(pOther.m_Material);
	}

	Primitive & Primitive::operator=(Primitive && pOther)
	{

		if (this != &pOther)
		{
			this->m_Attributes = pOther.m_Attributes;
			this->m_RenderingMode = pOther.m_RenderingMode;


			this->m_VertexBuffer  = std::move(pOther.m_VertexBuffer);
			this->m_ElementBuffer = std::move(pOther.m_ElementBuffer);
			this->m_VertexArrays  = std::move(pOther.m_VertexArrays);
			this->m_Material	  = std::move(pOther.m_Material);
		}

		return *this;
	}

};