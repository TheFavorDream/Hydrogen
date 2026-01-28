#include "Buffer.h"

namespace Hydrogen 
{
	Buffer::~Buffer()
	{
		DestroyBuffer();
	}

	Buffer::Buffer(Buffer && pMove)
	{
		m_BufferID = pMove.m_BufferID;
		m_BufferSize = pMove.m_BufferSize;
		m_BufferTarget = pMove.m_BufferTarget;

		pMove.m_BufferID = 0;
		pMove.m_BufferSize = 0;
	}

	int Buffer::CreateBuffer(GLenum pBufferTarget, uint32 pSize, void* pData, uint32 pCount)
	{
		GL_CALL(glGenBuffers(1, &m_BufferID));
		if (m_BufferID == 0)
		{
			Log::SetError("Couldn't Create Buffer", HYD_OPENGL_VERTEX_BUFFER_FAILED, __FILE__, __LINE__);
			return HYD_OPENGL_VERTEX_BUFFER_FAILED;
		}
		GL_CALL(glBindBuffer(pBufferTarget, m_BufferID));
		GL_CALL(glBufferData(pBufferTarget, pSize, pData, GL_STATIC_DRAW));

		m_BufferTarget = pBufferTarget;
		m_BufferSize = pCount;
		return HYD_OK;
	}

	int Buffer::CopyDataChunk(uint32 pOffset, uint32 pSize, void* pData)
	{
		if (m_BufferID == 0)
		{
			Log::SetError("Trying to copy into an empty buffer",HYD_OPENGL_VERTEX_BUFFER_FAILED, __FILE__, __LINE__);
			return HYD_OPENGL_VERTEX_BUFFER_FAILED;
		}
		Bind();
		GL_CALL(glBufferSubData(m_BufferTarget, (GLintptr)pOffset, (GLsizeiptr)pSize ,pData));

		return HYD_OK;
	}

	int Buffer::DestroyBuffer()
	{
		GL_CALL(glBindBuffer(m_BufferTarget, 0));
		GL_CALL(glDeleteBuffers(1, &m_BufferID));
		m_BufferID = 0;
		return HYD_OK;
	}

	void Buffer::Bind() const
	{
		GL_CALL(glBindBuffer(m_BufferTarget, m_BufferID));
	}
	void Buffer::Unbind() const
	{
		GL_CALL(glBindBuffer(m_BufferTarget, 0));
	}
};