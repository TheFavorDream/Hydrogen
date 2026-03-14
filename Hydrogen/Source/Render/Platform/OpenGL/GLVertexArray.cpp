#include "GLVertexArray.h"

namespace Hydrogen
{
	VertexArray::VertexArray(VertexArray&& Other)
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


	VertexArray& VertexArray::operator=(VertexArray&& pOther)
	{
		if (this != &pOther)
		{
			this->m_VertexID =			pOther.m_VertexID;
			this->m_Offset =			pOther.m_Offset;
			this->m_EnabledAttributes = pOther.m_EnabledAttributes;

			pOther.m_VertexID = 0;
		}

		return *this;
	}

	uint32 VertexArray::CreateVertexArray()
	{

		GL_CALL(glGenVertexArrays(1, &m_VertexID));
		return HYD_OK;
	}

	uint32 VertexArray::DestroyVertexArray()
	{
		Unbind();
		GL_CALL(glDeleteVertexArrays(1, &m_VertexID));
		m_VertexID = 0;
		return 0;
	}

	uint32 VertexArray::AddAttribute(const Accessor& pAccessor)
	{		
		uint32 CountPerAttrib = (uint32)pAccessor.DataType;
		uint32 Stride = CountPerAttrib *GetTypeSize(pAccessor.ComponentType);

		GL_CALL(glEnableVertexAttribArray(m_EnabledAttributes));
		GL_CALL(glVertexAttribPointer(m_EnabledAttributes, CountPerAttrib, (GLenum)pAccessor.ComponentType, false,  Stride, (void*)m_Offset));
		m_Offset += Stride*pAccessor.Count;
		m_EnabledAttributes++;
		return HYD_OK;
	}

	uint32 VertexArray::AddAttributes(Layouts& pAttributes)
	{
		for (auto& attrib : pAttributes)
		{
			GL_CALL(glEnableVertexAttribArray(m_EnabledAttributes));
			GL_CALL(glVertexAttribPointer(m_EnabledAttributes, attrib.Count, GetGLType(attrib.Type), attrib.Normalized, pAttributes.GetStride(), (void*)m_Offset));

			m_Offset += attrib.Count *  Layout::GetTypeSize(attrib.Type);
			m_EnabledAttributes++;
		}
		return HYD_OK;
	}


	uint32 VertexArray::DisableAttributes()
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


	GLenum VertexArray::GetGLType(DataType pType)
	{
		switch (pType)
		{
		case TYPE_FLOAT:
			return GL_FLOAT;
		case TYPE_UNSIGNED_SHORT:
			return GL_UNSIGNED_SHORT;
		case TYPE_SIGNED_SHORT:
			return GL_SHORT;
		case TYPE_UNSIGNED_BYTE:
			return GL_UNSIGNED_BYTE;
		case TYPE_SIGNED_BYTE:
			return GL_BYTE;
		case TYPE_UNSIGNED_INT:
			return GL_UNSIGNED_INT;
		case TYPE_SIGNED_INT:
			return GL_INT;
		}
		return GL_INVALID_ENUM;
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