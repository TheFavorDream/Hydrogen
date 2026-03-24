#include "GLBuffer.h"

namespace Hydrogen
{

	uint32 GLBuffer::CreateBuffer()
	{
		GL_CALL(glCreateBuffers(1, &m_BufferID));
		if (m_BufferID == 0)
			return HYD_FAILED;
		return HYD_OK;
	}

	uint32 GLBuffer::CreateBuffer(uint32 pSize, void* pData, uint32 pUsageHint)
	{
		GL_CALL(glCreateBuffers(1, &m_BufferID));
		if (m_BufferID == 0)
			return HYD_FAILED;

		if (pUsageHint == 0)
			pUsageHint = GL_STATIC_DRAW;
		GL_CALL(glNamedBufferData(m_BufferID, pSize, pData, pUsageHint));
		return HYD_OK;
	}

	uint32 GLBuffer::DeleteBuffer()
	{
		if (m_BufferID == 0)
			return HYD_INVALID_TARGET;
		GL_CALL(glDeleteBuffers(1, &m_BufferID));
		m_BufferID = 0;
		return HYD_OK;
	}

	uint32 GLBuffer::WriteBuffer(uint32 pSize, void* pData, uint32 pUsageHint)
	{
		if (m_BufferID == 0)
			return HYD_INVALID_TARGET;

		if (pUsageHint == 0)
			pUsageHint = GL_STATIC_DRAW;
		GL_CALL(glNamedBufferData(m_BufferID, pSize, pData, pUsageHint));
		return HYD_OK;
	}

	uint32 GLBuffer::WriteChunk(uint32 pOffset, uint32 pSize, void* pData)
	{
		if (m_BufferID == 0)
			return HYD_INVALID_TARGET;

		GL_CALL(glNamedBufferSubData(m_BufferID, pOffset, pSize, pData));
		return HYD_OK;
	}

	uint32 GLBuffer::ReadBuffer(uint32 pOffset, uint32 pBytesToRead, std::string& pData)
	{
		if (m_BufferID == 0)
			return HYD_INVALID_TARGET;
		if (pBytesToRead > m_SizeInBytes)
			return HYD_INVALID_VALUE;

		void* Mapped = (char*)glMapNamedBufferRange(m_BufferID, pOffset, pBytesToRead, GL_MAP_READ_BIT);

		pData.resize(pBytesToRead);
		memcpy((void*)&pData[0], Mapped, pBytesToRead);

		GL_CALL(glUnmapNamedBuffer(m_BufferID));
		return HYD_OK;
	}

	uint32 GLBuffer::ClearBuffer(uint32 pClearValue)
	{
		return HYD_OK;
	}
};