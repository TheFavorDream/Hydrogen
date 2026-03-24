#include "GLElementBuffer.h"

namespace Hydrogen
{




	GLElementBuffer::GLElementBuffer()
	{
		Unbind();
		DeleteBuffer();
	}

	GLElementBuffer::GLElementBuffer(GLElementBuffer&& pOther)
	{
		m_BufferID    = pOther.m_BufferID;
		m_SizeInBytes = pOther.m_SizeInBytes;
		m_Count		  = pOther.m_Count;
		m_Type		  = pOther.m_Type;


		pOther.m_BufferID    = 0;
		pOther.m_SizeInBytes = 0;
		pOther.m_Count	     = 0;
	}

	GLElementBuffer& GLElementBuffer::operator=(GLElementBuffer && pOther)
	{
		if (this != &pOther)
		{
			m_BufferID = pOther.m_BufferID;
			m_SizeInBytes = pOther.m_SizeInBytes;
			m_Count = pOther.m_Count;
			m_Type = pOther.m_Type;


			pOther.m_BufferID = 0;
			pOther.m_SizeInBytes = 0;
			pOther.m_Count = 0;
		}
		return *this;
	}

	uint32 GLElementBuffer::CreateElementBuffer()
	{
		m_Count = 0;
		m_Type  = GL_UNSIGNED_SHORT;
		return CreateBuffer();
	}

	uint32 GLElementBuffer::CreateElementBuffer(uint32 pCount, GLenum pType, void* pData)
	{
		m_Count = pCount;
		m_Type  = pType;
		return CreateBuffer(pCount*GetTypeSize(pType), pData, GL_STATIC_DRAW);
	}

	uint32 GLElementBuffer::InitElementBuffer(uint32 pCount, GLenum pType, void* pData)
	{
		m_Count = pCount;
		m_Type = pType;
		return WriteBuffer(pCount*GetTypeSize(pType), pData, GL_STATIC_DRAW);
	}


	void GLElementBuffer::Bind() const
	{
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID));
	}

	void GLElementBuffer::Unbind() const
	{
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	
	}
	uint32 GLElementBuffer::GetTypeSize(GLenum pType)
	{

		switch (pType)
		{
		case GL_UNSIGNED_SHORT:
			return sizeof(uint16);
		case GL_UNSIGNED_BYTE:
			return sizeof(uint8);
		case GL_UNSIGNED_INT:
			return sizeof(uint32);
		case GL_BYTE:
			return sizeof(int8);
		case GL_SHORT:
			return sizeof(int16);
		case GL_INT:
			return sizeof(int32);
		}

		return 0;
	}
};