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
		m_Handler     = pOther.m_Handler;
		m_SizeInBytes = pOther.m_SizeInBytes;
		m_Count		  = pOther.m_Count;
		m_Type		  = pOther.m_Type;


		pOther.m_Handler     = 0;
		pOther.m_SizeInBytes = 0;
		pOther.m_Count	     = 0;
	}

	GLElementBuffer& GLElementBuffer::operator=(GLElementBuffer && pOther)
	{
		if (this == &pOther)
			return *this;

		m_Handler     = pOther.m_Handler;
		m_SizeInBytes = pOther.m_SizeInBytes;
		m_Count       = pOther.m_Count;
		m_Type        = pOther.m_Type;


		pOther.m_Handler     = 0;
		pOther.m_SizeInBytes = 0;
		pOther.m_Count       = 0;
		
		return *this;
	}

	uint32 GLElementBuffer::CreateElementBuffer()
	{
		m_Count = 0;
		m_Type  = GL_UNSIGNED_SHORT;
		GL_CALL(glCreateBuffers(1, &m_Handler));
		if (m_Handler == 0)
			return HYD_FAILED;
		return HYD_OK;
	}

	uint32 GLElementBuffer::CreateElementBuffer(uint32 pCount, GLenum pType, void* pData)
	{
		m_Count = pCount;
		m_Type  = pType;
		GL_CALL(glCreateBuffers(1, &m_Handler));
		if (m_Handler == 0)
			return HYD_FAILED;


		GL_CALL(glNamedBufferData(m_Handler, m_Count*GetTypeSize(m_Type), pData, GL_STATIC_DRAW));
		return HYD_OK;
	}

	uint32 GLElementBuffer::DeleteBuffer()
	{
		if (m_Handler == 0)
			return HYD_INVALID_TARGET;
		GL_CALL(glDeleteBuffers(1, &m_Handler));
		m_Handler     = 0;
		m_SizeInBytes = 0;
		m_Count		  = 0;
		return HYD_OK;
	}

	uint32 GLElementBuffer::InitElementBuffer(uint32 pCount, GLenum pType, void* pData)
	{
		m_Count = pCount;
		m_Type  = pType;
		if (m_Handler == 0)
			return HYD_INVALID_TARGET;

		GL_CALL(glNamedBufferData(m_Handler, m_Count*GetTypeSize(m_Type), pData, GL_STATIC_DRAW));
		return HYD_OK;
	}

	uint32 GLElementBuffer::WriteChunk(uint32 pOffset, uint32 pSize, void * pData)
	{
		if (m_Handler == 0)
			return HYD_INVALID_TARGET;

		GL_CALL(glNamedBufferSubData(m_Handler, pOffset, pSize, pData));
		return HYD_OK;
	}

	uint32 GLElementBuffer::ReadBuffer(uint32 pOffset, uint32 pBytesToRead, std::string & pData)
	{
		if (m_Handler == 0)
			return HYD_INVALID_TARGET;
		if (pBytesToRead > m_SizeInBytes)
			return HYD_INVALID_VALUE;

		void* Mapped = (char*)glMapNamedBufferRange(m_Handler, pOffset, pBytesToRead, GL_MAP_READ_BIT);

		pData.resize(pBytesToRead);
		memcpy((void*)&pData[0], Mapped, pBytesToRead);

		GL_CALL(glUnmapNamedBuffer(m_Handler));
		return HYD_OK;
	}


	void GLElementBuffer::Bind() const
	{
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handler));
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