#include "GLVertexArray.h"
#include "HydPch.h"


namespace Hydrogen
{
	VertexArray::VertexArray(VertexArray&& Other)
	{
		m_VertexID = Other.m_VertexID;
		m_Attributes = Other.m_Attributes;

		Other.m_VertexID   = 0;
		Other.m_Attributes = 0;
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
			this->m_Attributes = pOther.m_Attributes;

			pOther.m_VertexID   = 0;
			pOther.m_Attributes = 0;

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

	uint32 VertexArray::AddAttribute(const Layout& pLayout)
	{	
		GL_CALL(glEnableVertexAttribArray(pLayout.Attribute));
		GL_CALL(glVertexAttribPointer(pLayout.Attribute, pLayout.Count, GetGLType(pLayout.Type), false,  pLayout.Stride, (void*)pLayout.Offset));
		m_Attributes += 1;
		return HYD_OK;
	}

	uint32 VertexArray::AddAttributes(const std::vector<Layout>& pAttributes)
	{
		for (auto& attrib : pAttributes)
		{
			AddAttribute(attrib);
		}
		return HYD_OK;
	}


	uint32 VertexArray::EnableAttribute(uint32 pAttribute)
	{
		if (pAttribute >= m_Attributes)
			return HYD_INVALID_VALUE;
		GL_CALL(glEnableVertexArrayAttrib(m_VertexID, (uint32)pAttribute));
		return HYD_OK;
	}

	uint32 VertexArray::DisableAttributes(uint32 pAttribute)
	{
		if (pAttribute >= m_Attributes)
			return HYD_INVALID_VALUE;
		GL_CALL(glDisableVertexArrayAttrib(m_VertexID, (uint32)pAttribute));
		return HYD_OK;
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

	DataType VertexArray::GetHydType(GLenum pType)
	{
		switch (pType)
		{
		case GL_FLOAT:
			return TYPE_FLOAT;

		case GL_UNSIGNED_SHORT:
			return  TYPE_UNSIGNED_SHORT;

		case GL_SHORT:
			return TYPE_SIGNED_SHORT;
		
		case GL_UNSIGNED_BYTE:
			return TYPE_UNSIGNED_BYTE;
		
		case GL_BYTE:
			return TYPE_SIGNED_BYTE;

		case GL_UNSIGNED_INT:
			return TYPE_UNSIGNED_INT;
		case GL_INT:
			return TYPE_SIGNED_INT;
		}
		return DataType(0);
	}


	uint32 VertexArray::GetTypeSize(DataType pType)
	{

		switch (pType)
		{
		case TYPE_FLOAT:
			return 4;
		case TYPE_UNSIGNED_SHORT:
			return 2;
		case TYPE_SIGNED_SHORT:
			return 2;
		case TYPE_UNSIGNED_BYTE:
			return 1;
		case TYPE_SIGNED_BYTE:
			return 1;
		case TYPE_UNSIGNED_INT:
			return 4;
		case TYPE_SIGNED_INT:
			return 4;
		}
		return 0;
	}
};