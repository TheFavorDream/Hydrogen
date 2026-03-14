#include "BufferPool.h"

namespace Hydrogen
{


	uint32 BufferPool::InitPool()
	{
		return HYD_OK;
	}

	uint32 BufferPool::ShutdownPool()
	{
		//free Arrays:
		m_VAOs.Clear();
		m_VBOs.Clear();
		m_EBOs.Clear();
		return HYD_OK;
	}


//----------------Vertex Array--------------------

	Id BufferPool::CreateVertexArray(const Attribute& pAttribute)
	{
		VertexArray* VAO = ResourcePool<VertexArray>::New();
		VAO->CreateVertexArray();
		//VAO->AddAttribute(pAttribute);
		Id ID = m_VAOs.Push(VAO);
		return ID;
	}

	uint32 BufferPool::AddVertexAttribute(Id pVao, const Accessor & pAccessor)
	{
		GetArray(pVao).AddAttribute(pAccessor);
		return HYD_OK;
	}

	uint32 BufferPool::AddVertexAttributes(Id pVao, Layouts& pAttributes)
	{
		return GetArray(pVao).AddAttributes(pAttributes);
	}

	uint32 BufferPool::DestroyVertexArray(Id* pVao)
	{
		UnbindArray(*pVao);
		m_VAOs.Pop(pVao);
		return HYD_OK;
	}

//------------------------ Array Buffer ----------------------------------

	Id BufferPool::CreateVertexBuffer(uint32 pSize, void * pData, uint32 pCount, uint32 pComponentType)
	{
		Buffer* NewBuffer = ResourcePool<Buffer>::New();
		NewBuffer->CreateBuffer(GL_ARRAY_BUFFER, pSize, pData, pCount, pComponentType);
		Id ID = m_VBOs.Push(NewBuffer);
		return ID;
	}

	uint32 BufferPool::CopyVertexDataChunk(Id pBufferID, uint32 pOffset, uint32 pSize, void * pData)
	{
		return GetVertexBuffer(pBufferID).CopyDataChunk(pOffset, pSize, pData);
	}

	uint32 BufferPool::DestroyVertexBuffer(Id * pBufferID)
	{
		m_VBOs.Pop(pBufferID);
		return HYD_OK;
	}



	Id BufferPool::CreateElementBuffer(uint32 pSize, void* pData, uint32 pCount, uint32 pComponentType)
	{
		Buffer* NewBuffer = ResourcePool<Buffer>::New();
		NewBuffer->CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, pSize, pData, pCount, pComponentType);
		Id ID = m_EBOs.Push(NewBuffer);
		return ID;
	}

	uint32 BufferPool::CopyElementDataChunk(Id pBufferID, uint32 pOffset, uint32 pSize, void * pData)
	{
		return GetElementBuffer(pBufferID).CopyDataChunk(pOffset, pSize, pData);

	}

	HYD uint32 BufferPool::DestroyElementBuffer(Id * pBufferID)
	{
		return m_EBOs.Pop(pBufferID);
	}




	Buffer& BufferPool::GetVertexBuffer(Id pBufferID)
	{
		if (pBufferID == 0)
			__debugbreak();
		return m_VBOs.GetResource(pBufferID);
	}

	Buffer& BufferPool::GetElementBuffer(Id pBufferID)
	{

		if (pBufferID == 0)
			__debugbreak();
		return m_EBOs.GetResource(pBufferID);
	}

	VertexArray& BufferPool::GetArray(Id pArrayID)
	{
		if (pArrayID == 0)
			__debugbreak();
		return m_VAOs.GetResource(pArrayID);
	}



//-----------------Binding Operations-----------------------------------

	uint32 BufferPool::BindVertexBuffer(Id pBufferID)
	{
		GetVertexBuffer(pBufferID).Bind();
		s_CurrentBindedVertexBuffer = pBufferID;
		return HYD_OK;
	}

	uint32 BufferPool::UnbindVertexBuffer(Id pBufferID)
	{
		GetVertexBuffer(pBufferID).Unbind();
		s_CurrentBindedVertexBuffer = 0;
		return HYD_OK;
	}

	uint32 BufferPool::BindElementBuffer(Id pBufferID)
	{
		GetElementBuffer(pBufferID).Bind();
		s_CurrentBindedElementBuffer  = pBufferID;
		return HYD_OK;
	}

	uint32 BufferPool::UnbindElementBuffer(Id pBufferID)
	{

		GetElementBuffer(pBufferID).Unbind();
		s_CurrentBindedElementBuffer = 0;
		return HYD_OK;
	}

	uint32 BufferPool::BindArray(Id pArray)
	{
		GetArray(pArray).Bind();
		return HYD_OK;
	}

	uint32 BufferPool::UnbindArray(Id pArray)
	{
		GetArray(pArray).Unbind();
		return HYD_OK;
	}

	HYD uint32 BufferPool::BindCollection(Id pVAO, Id pVBO, Id pEBO)
	{
		GetArray(pVAO).Bind();
		GetArray(pVBO).Bind();		
		GetArray(pEBO).Bind();
		return HYD_OK;
	}
	HYD uint32 BufferPool::UnbindCollection(Id pVAO, Id pVBO, Id pEBO)
	{
		GetArray(pVAO).Unbind();
		GetArray(pVBO).Unbind();
		GetArray(pEBO).Unbind();
		return HYD_OK;
	}
};