#include "GLVertexBuffer.h"
#include "HydPch.h"


namespace Hydrogen 
{
	GLVertexBuffer::~GLVertexBuffer()
	{
		DeleteBuffer();
	}

	GLVertexBuffer::GLVertexBuffer(GLVertexBuffer && pMove)
	{
		m_Handler = pMove.m_Handler;
		m_SizeInBytes = pMove.m_SizeInBytes;

		pMove.m_Handler = 0;
		pMove.m_SizeInBytes = 0;
	}

	GLVertexBuffer& GLVertexBuffer::operator=(GLVertexBuffer&& pOther)
	{
		if (&pOther != this)
		{
			m_Handler     = pOther.m_Handler;
			m_SizeInBytes = pOther.m_SizeInBytes;

			pOther.m_Handler     = 0;
			pOther.m_SizeInBytes = 0;
		}

		return *this;
	}


	uint32 GLVertexBuffer::CreateBuffer()
	{
		GL_CALL(glCreateBuffers(1, &m_Handler));
		if (m_Handler == 0)
			return HYD_FAILED;
		return HYD_OK;
	}

	uint32 GLVertexBuffer::CreateBuffer(uint32 pSize, void* pData, uint32 pUsageHint)
	{
		GL_CALL(glCreateBuffers(1, &m_Handler));
		if (m_Handler == 0)
			return HYD_FAILED;

		if (pUsageHint == 0)
			pUsageHint = GL_STATIC_DRAW;
		GL_CALL(glNamedBufferData(m_Handler, pSize, pData, pUsageHint));
		return HYD_OK;
	}

	uint32 GLVertexBuffer::DeleteBuffer()
	{
		if (m_Handler == 0)
			return HYD_INVALID_TARGET;
		GL_CALL(glDeleteBuffers(1, &m_Handler));
		m_Handler = 0;
		return HYD_OK;
	}


	uint32 GLVertexBuffer::WriteBuffer(uint32 pSize, void* pData, uint32 pUsageHint)
	{
		if (m_Handler == 0)
			return HYD_INVALID_TARGET;

		if (pUsageHint == 0)
			pUsageHint = GL_STATIC_DRAW;
		GL_CALL(glNamedBufferData(m_Handler, pSize, pData, pUsageHint));
		return HYD_OK;
	}

	uint32 GLVertexBuffer::WriteChunk(uint32 pOffset, uint32 pSize, void* pData)
	{
		if (m_Handler == 0)
			return HYD_INVALID_TARGET;

		GL_CALL(glNamedBufferSubData(m_Handler, pOffset, pSize, pData));
		return HYD_OK;
	}

	uint32 GLVertexBuffer::ReadBuffer(uint32 pOffset, uint32 pBytesToRead, std::string& pData)
	{
		if (m_Handler == 0)
			return HYD_INVALID_TARGET;
		if (pBytesToRead > m_SizeInBytes)
			return HYD_INVALID_VALUE;

		void* Mapped = (char*)glMapNamedBufferRange(m_Handler, pOffset, pBytesToRead, GL_MAP_READ_BIT);

		pData.resize(pBytesToRead);
		memcpy((void*)&pData[0], Mapped, pBytesToRead);

		GL_CALL(glUnmapNamedBuffer(m_Handler));
	}

	uint32 GLVertexBuffer::ClearBuffer(uint32 pClearValue)
	{
		return uint32();
	}

	void GLVertexBuffer::Bind() const
	{
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_Handler));
	}

	void GLVertexBuffer::Unbind() const
	{
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
};