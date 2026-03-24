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
		m_BufferID = pMove.m_BufferID;
		m_SizeInBytes = pMove.m_SizeInBytes;

		pMove.m_BufferID = 0;
		pMove.m_SizeInBytes = 0;
	}

	GLVertexBuffer& GLVertexBuffer::operator=(GLVertexBuffer&& pOther)
	{
		if (&pOther != this)
		{
			m_BufferID    = pOther.m_BufferID;
			m_SizeInBytes = pOther.m_SizeInBytes;

			pOther.m_BufferID    = 0;
			pOther.m_SizeInBytes = 0;
		}

		return *this;
	}


	void GLVertexBuffer::Bind() const
	{
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
	}
	void GLVertexBuffer::Unbind() const
	{
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
};