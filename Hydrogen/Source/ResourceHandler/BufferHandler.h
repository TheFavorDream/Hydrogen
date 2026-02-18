/*
	Purpose:
		Handles the Buffers (Vertex Buffer And Element Buffer) as well as Vertex Array Buffers.
		this class provides an interface for engine to create and destroy buffers.
		the main advantage of having a buffer manager (or resource manager in general) is the ability of different 
		primitives to use a common buffer without needing to duplicate.
		Resource Handlers also Encapsulats Buffers and controls the usage of them as well.

*/
#pragma once

#include "Common.h"
#include "Log/Log.h"
#include "ResourceHandler.h"
#include "Platform/OpenGL/Buffer.h"
#include "Platform/OpenGL/VertexArray.h"

namespace Hydrogen
{
	class BufferHandler 
	{
	public:
	
		HYD static uint32 InitHandler();
		HYD static uint32 ShutdownHandler();



		HYD static Id     PushVertexArray(VertexArray* pVAO);
		HYD static uint32 DestroyVertexArray(Id* pVao);
		HYD static Id     CreateVertexArray();
		HYD static uint32 AddVertexAttribute(Id pVao, const Accessor& pAccessor);

		//Buffer Operations:
		HYD static Id	   PushBuffer(Buffer* pBuffer);
		HYD static uint32  DestroyBuffer(Id* pBufferID);
		HYD static Id      CreateBuffer(GLenum pBufferTarget, uint32 pSize, void* pData = 0, uint32 pCount = 0, uint32 pComponentType = GL_UNSIGNED_SHORT);
		HYD static uint32  CopyDataChunk(Id pBufferID, uint32 pOffset, uint32 pSize, void* pData);



		HYD static uint32  BindBuffer(Id pBufferID);
		HYD static uint32  UnbindBuffer(Id pBufferID);

		HYD static uint32 BindArray(Id pArray);
		HYD static uint32 UnbindArray(Id pArray);

		HYD static uint32 BindCollection(Id pVAO, Id pVBO, Id pEBO);
		HYD static uint32 UnbindCollection(Id pVAO, Id pVBO, Id pEBO);

		HYD static Buffer&		GetBuffer(Id pBufferID);
		HYD static VertexArray& GetArray(Id pArrayID);
		HYD inline uint32 GetTotalBufferNumber() { return s_Buffers; }

	private:
		static Id s_CurrentBindedVertexBuffer;
		static Id s_CurrentBindedElementBuffer;
		static Id s_CurrentBindedVertexArray;
		static uint32 s_Buffers;

		HYD static std::vector<Id> s_BufferID;
		HYD static std::vector<Id> s_ArrayID;
	};
};