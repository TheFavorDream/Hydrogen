#pragma once

#include "Common.h"
#include "Glew/glew.h"
#include "GLBuffer.h"

namespace Hydrogen
{

	class GLElementBuffer : public GLBuffer
	{
	public:

		HYD GLElementBuffer();

		HYD GLElementBuffer(const GLElementBuffer& pOther) = delete;
		HYD GLElementBuffer(GLElementBuffer&& pOther);

		HYD GLElementBuffer& operator=(const GLElementBuffer* pOther) = delete;
		HYD GLElementBuffer& operator=(GLElementBuffer&& pOther);

		HYD uint32 CreateElementBuffer();
		HYD uint32 CreateElementBuffer(uint32 pCount, GLenum pType, void* pData);

		HYD uint32 InitElementBuffer(uint32 pCount, GLenum pType, void* pData);

		HYD void Bind()    const override;
		HYD void Unbind()  const override;

		HYD inline uint32 GetCount() const { return m_Count; }
		HYD inline GLenum GetType()  const { return m_Type; }

	public:

		HYD static uint32 GetTypeSize(GLenum pType);

	private:
		uint32  m_Count = 0;
		GLenum  m_Type  = 0;
	};

};
