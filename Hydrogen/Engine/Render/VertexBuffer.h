#pragma once
#include "Common.h"


namespace Hydrogen
{
	
	class VertexBuffer
	{
	public:

		VertexBuffer();
	   ~VertexBuffer();

	   VertexBuffer(const VertexBuffer& pOther) = delete;
	   VertexBuffer(VertexBuffer&& pOther);

	   VertexBuffer& operator=(const VertexBuffer& pOther) = delete;
	   VertexBuffer& operator=(VertexBuffer&& pOther);

	   uint32 CreateBuffer(); //Empty Buffer 
	   uint32 CreateBuffer(uint32 pSize, void* pData, uint32 pUsageHint = 0);
	   uint32 DeleteBuffer();

	   uint32 WriteBuffer(uint32 pSize, void* pData, uint32 pUsageHint = 0);
	   uint32 WriteChunk(uint32  pOffset, uint32 pSize, void* pData);
	   uint32 ReadBuffer(uint32  pOffset, uint32 pBytesToRead, std::string& pData);

	   uint32 ClearBuffer(uint32 pClearValue = 0);

	   void Bind();
	   void Unbind();

	private:
		void* m_BufferObj = nullptr;
	};

};