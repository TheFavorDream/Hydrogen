#pragma once

#include "Common.h"
#include "Log/Log.h"
#include "Glew/glew.h"
#include "Render/Buffer.h"

namespace Hydrogen
{
	class GLBuffer : public Buffer
	{
	public:

		GLBuffer() = default;
		virtual ~GLBuffer() = default;

		uint32 CreateBuffer() override; //Empty Buffer 
		uint32 CreateBuffer(uint32 pSize, void* pData, uint32 pUsageHint = 0) override;
		uint32 DeleteBuffer() override;

		uint32 WriteBuffer(uint32 pSize, void* pData, uint32 pUsageHint = 0) override;
		uint32 WriteChunk(uint32 pOffset, uint32 pSize, void* pData) override;
		uint32 ReadBuffer(uint32 pOffset, uint32 pBytesToRead, std::string& pData) override;

		virtual uint32 ClearBuffer(uint32 pClearValue = 0) override;

		virtual void Bind()    const = 0;
		virtual void Unbind()  const = 0;

	
	};
};