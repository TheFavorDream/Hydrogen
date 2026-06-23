#pragma once

#include "Common.h"
#include "Glew/glew.h"
#include "Log/Log.h"

namespace Hydrogen
{

	class GLElementBuffer
	{
	public:

		HYD GLElementBuffer();

		HYD GLElementBuffer(const GLElementBuffer& pOther) = delete;
		HYD GLElementBuffer(GLElementBuffer&& pOther);

		HYD GLElementBuffer& operator=(const GLElementBuffer* pOther) = delete;
		HYD GLElementBuffer& operator=(GLElementBuffer&& pOther);

		HYD uint32 CreateElementBuffer();
		HYD uint32 CreateElementBuffer(uint32 pCount, GLenum pType, void* pData);
		HYD uint32 DeleteBuffer();
		HYD uint32 InitElementBuffer(uint32 pCount, GLenum pType, void* pData);

		HYD uint32 WriteChunk(uint32 pOffset, uint32 pSize, void* pData);
		HYD uint32 ReadBuffer(uint32 pOffset, uint32 pBytesToRead, std::string& pData);

		HYD void Bind()    const;
		HYD void Unbind()  const;

		HYD inline uint32 GetCount() const { return m_Count; }
		HYD inline GLenum GetType()  const { return m_Type; }

	public:

		HYD static uint32 GetTypeSize(GLenum pType);

	private:
		uint32  m_Handler = 0;
		uint32  m_SizeInBytes = 0;
		uint32  m_Count   = 0;
		GLenum  m_Type    = 0;
	};

};
