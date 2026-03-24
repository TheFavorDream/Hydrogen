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
#include "Core/ResourcePool.h"
#include "Render/Platform/OpenGL/GLBuffers/GLBuffer.h"
#include "Render/Platform/OpenGL/GLBuffers/GLVertexBuffer.h"
#include "Render/Platform/OpenGL/GLBuffers/GLElementBuffer.h"


#include "Render/Platform/OpenGL/GLVertexArray.h"

namespace Hydrogen
{
	class BufferPool
	{
	public:
	
		HYD  uint32 InitPool();
		HYD  uint32 ShutdownPool();



		HYD  Id     CreateVertexArray();
		HYD  uint32 AddVertexAttribute(Id pVao,  const Layout& pLayout);
		HYD  uint32 AddVertexAttributes(Id pVao, const std::vector<Layout>& pLayout);
		HYD  uint32 DestroyVertexArray(Id* pVao);

		//Buffer Operations:
		HYD  Id      CreateVertexBuffer(uint32 pSize, void* pData = 0, uint32 pCount = 0, uint32 pComponentType = GL_UNSIGNED_SHORT);
		HYD  uint32  CopyVertexDataChunk(Id pBufferID, uint32 pOffset, uint32 pSize, void* pData);
		HYD  uint32  DestroyVertexBuffer(Id* pBufferID);

		HYD  Id      CreateElementBuffer(uint32 pCount , uint32 pComponentType, void* pData);
		HYD  uint32  CopyElementDataChunk(Id pBufferID, uint32 pOffset, uint32 pSize, void* pData);
		HYD  uint32  DestroyElementBuffer(Id* pBufferID);

			 
		HYD  uint32  BindVertexBuffer(Id pBufferID);
		HYD  uint32  UnbindVertexBuffer(Id pBufferID);
			 
		HYD  uint32  BindElementBuffer(Id pBufferID);
		HYD  uint32  UnbindElementBuffer(Id pBufferID);


		HYD  uint32 BindArray(Id pArray);
		HYD  uint32 UnbindArray(Id pArray);
			 
		HYD  uint32 BindCollection(Id pVAO, Id pVBO, Id pEBO);
		HYD  uint32 UnbindCollection(Id pVAO, Id pVBO, Id pEBO);
			 
		HYD        Buffer*		GetVertexBuffer(Id pBufferID);
		HYD        Buffer*		GetElementBuffer(Id pBufferID);
		HYD		   VertexArray& GetArray(Id pArrayID);
		
		
		HYD inline uint32		GetTotalBufferNumber() { return s_Buffers; }

	private:
		Id s_CurrentBindedVertexBuffer;
		Id s_CurrentBindedElementBuffer;
		Id s_CurrentBindedVertexArray;
		uint32 s_Buffers;

		ResourcePool<Buffer>		m_VBOs;
		ResourcePool<Buffer>		m_EBOs;
		ResourcePool<VertexArray>	m_VAOs;

	};
};