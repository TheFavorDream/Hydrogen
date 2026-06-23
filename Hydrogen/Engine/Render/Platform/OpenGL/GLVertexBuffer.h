/*
	OpenGL Vertex Buffer Definitions.
*/

#pragma once


#include "Glew/glew.h"
#include "Common.h"
#include "Log/Log.h"

namespace Hydrogen
{

	class GLVertexBuffer
	{
	public:

		HYD  GLVertexBuffer() = default;
		HYD ~GLVertexBuffer();

		HYD GLVertexBuffer(const GLVertexBuffer& pOther) = delete;
		HYD GLVertexBuffer& operator=(const GLVertexBuffer& pOther) = delete;
		
		HYD GLVertexBuffer(GLVertexBuffer&& pMove);
		HYD GLVertexBuffer& operator=(GLVertexBuffer&& pOther);
		 
		uint32 CreateBuffer(); //Empty Buffer 
		uint32 CreateBuffer(uint32 pSize, void* pData, uint32 pUsageHint = 0);
		uint32 DeleteBuffer();

		uint32 WriteBuffer(uint32 pSize, void* pData, uint32 pUsageHint = 0);
		uint32 WriteChunk(uint32 pOffset, uint32 pSize, void* pData);
		uint32 ReadBuffer(uint32 pOffset, uint32 pBytesToRead, std::string& pData);

		uint32 ClearBuffer(uint32 pClearValue = 0);

		 //Binds the Current Vertex Buffer
		HYD void Bind() const;
		 // Binds the current buffer to zero
		HYD void Unbind() const;
		 
	private:

		uint32 m_Handler     = 0;
		uint32 m_SizeInBytes = 0;

	};

};
