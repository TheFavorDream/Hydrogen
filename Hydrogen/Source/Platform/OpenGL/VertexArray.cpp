#include "VertexArray.h"

namespace Hydrogen
{
	VertexArray::VertexArray(VertexArray && Other)
	{
		m_VertexID = Other.m_VertexID;
		m_Offset = Other.m_Offset;
		m_EnabledAttributes = Other.m_EnabledAttributes;

		Other.m_VertexID = 0;
	}

	VertexArray::~VertexArray()
	{
		DestroyVertexArray();
	}

	int VertexArray::CreateVertexArray()
	{
		GL_CALL(glGenVertexArrays(1, &m_VertexID));
		return HYD_OK;
	}

	int VertexArray::DestroyVertexArray()
	{
		Unbind();
		GL_CALL(glDeleteVertexArrays(1, &m_VertexID));
		m_VertexID = 0;
		return 0;
	}

	int VertexArray::AddAttribute(const Accessor& pAccessor)
	{		
		uint32 CountPerAttrib = (uint32)pAccessor.DataType;
		uint32 Stride = CountPerAttrib *GetTypeSize(pAccessor.ComponentType);

		GL_CALL(glEnableVertexAttribArray(m_EnabledAttributes));
		GL_CALL(glVertexAttribPointer(m_EnabledAttributes, CountPerAttrib, (GLenum)pAccessor.ComponentType, false,  Stride, (void*)m_Offset));
		m_Offset += Stride*pAccessor.Count;
		m_EnabledAttributes++;
		return HYD_OK;
	}


	int VertexArray::DisableAttributes()
	{
		return 0;
	}


	void VertexArray::Bind()
	{
		GL_CALL(glBindVertexArray(m_VertexID));
	}
	void VertexArray::Unbind()
	{
		GL_CALL(glBindVertexArray(0));
	}


	uint32 VertexArray::GetTypeSize(uint32 pType)
	{

		switch (pType)
		{
		case GL_FLOAT:
			return sizeof(float);
		case GL_UNSIGNED_BYTE:
			return sizeof(uint8);
		}
		return 0;
	}
};