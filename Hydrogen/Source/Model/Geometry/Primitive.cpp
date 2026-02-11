#include "Primitive.h"

namespace Hydrogen
{



	Primitive::Primitive()
	{

	}

	Primitive::~Primitive()
	{
		if (m_VertexArrays != 0)
		{
			BufferHandler::DestroyBuffer(&m_VertexBuffer);
			BufferHandler::DestroyBuffer(&m_ElementBuffer);
			BufferHandler::DestroyVertexArray(&m_VertexArrays);

		}
	}
	Primitive::Primitive(Primitive && pOther)
	{
		this->m_Attributes = pOther.m_Attributes;
		this->m_RenderingMode = pOther.m_RenderingMode;

		this->m_VertexBuffer  = pOther.m_VertexBuffer ;
		this->m_ElementBuffer = pOther.m_ElementBuffer;
		this->m_VertexArrays  = pOther.m_VertexArrays ;
		this->m_Material	  = pOther.m_Material     ;


		pOther.m_VertexBuffer  = 0;
		pOther.m_ElementBuffer = 0;
		pOther.m_VertexArrays  = 0;
		pOther.m_Material      = 0;
	}

	Primitive & Primitive::operator=(Primitive && pOther)
	{

		if (this != &pOther)
		{
			this->m_Attributes = pOther.m_Attributes;
			this->m_RenderingMode = pOther.m_RenderingMode;


			this->m_VertexBuffer  = pOther.m_VertexBuffer;
			this->m_ElementBuffer = pOther.m_ElementBuffer;
			this->m_VertexArrays  = pOther.m_VertexArrays;
			this->m_Material	  = pOther.m_Material;

			pOther.m_VertexBuffer  = 0;
			pOther.m_ElementBuffer = 0;
			pOther.m_VertexArrays  = 0;
			pOther.m_Material      = 0;
		}

		return *this;
	}

};