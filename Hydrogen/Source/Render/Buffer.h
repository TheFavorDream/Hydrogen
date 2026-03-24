#pragma once

#include "Common.h"
#include "Glew/glew.h"
#include "Log/Log.h"

namespace Hydrogen {

	class Buffer
	{
	public:

		Buffer();
		virtual ~Buffer();

		virtual uint32 CreateBuffer() = 0; //Empty Buffer 
		virtual uint32 CreateBuffer(uint32 pSize, void* pData, uint32 pUsageHint=0) = 0;
		virtual uint32 DeleteBuffer() = 0;

		virtual uint32 WriteBuffer(uint32 pSize, void* pData, uint32 pUsageHint = 0) = 0;
		virtual uint32 WriteChunk(uint32 pOffset, uint32 pSize, void* pData) = 0;
		virtual uint32 ReadBuffer(uint32 pOffset, uint32 pBytesToRead, std::string& pData) = 0;

		virtual uint32 ClearBuffer(uint32 pClearValue=0) = 0;


		virtual void Bind()    const = 0;
		virtual void Unbind()  const  = 0;

	protected:

		uint32 m_BufferID = 0;
		uint32 m_SizeInBytes = 0;

	};
}