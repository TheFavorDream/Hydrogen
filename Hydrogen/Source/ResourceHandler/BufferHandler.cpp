#include "BufferHandler.h"

namespace Hydrogen
{

	Id BufferHandler::s_CurrentBindedVertexBuffer  = 0;
	Id BufferHandler::s_CurrentBindedElementBuffer = 0;
	Id BufferHandler::s_CurrentBindedVertexArray   = 0;

	LinkedList<Buffer>      BufferHandler::s_Buffers;
	LinkedList<VertexArray> BufferHandler::s_Arrays;

	uint32 BufferHandler::InitHandler()
	{
		return HYD_OK;
	}

	uint32 BufferHandler::ShutdownHandler()
	{
		s_Arrays.Clear();
		s_Buffers.Clear();
		return HYD_OK;
	}


	Id BufferHandler::PushVertexArray(VertexArray && pVAO)
	{
		return HYD_OK;
	}

	uint32 BufferHandler::DestroyVertexArray(Id * pVao)
	{
		GetArray(*pVao).Unbind();
		s_Arrays.Remove(pVao);
		return HYD_OK;
	}

	Id BufferHandler::CreateVertexArray()
	{
		VertexArray VAO;
		VAO.CreateVertexArray();
		return s_Arrays.AddTail(std::move(VAO));
	}

	uint32 BufferHandler::AddVertexAttribute(Id pVao, const Accessor & pAccessor)
	{
		GetArray(pVao).AddAttribute(pAccessor);
		return HYD_OK;
	}



	Id BufferHandler::PushBuffer(Buffer&& pBuffer)
	{
		return 0;
	}

	uint32 BufferHandler::DestroyBuffer(Id * pBufferID)
	{
		GetBuffer(*pBufferID).Unbind();
		s_Buffers.Remove(pBufferID);
		return HYD_OK;
	}

	Id BufferHandler::CreateBuffer(GLenum pBufferTarget, uint32 pSize, void * pData, uint32 pCount, uint32 pComponentType)
	{
		Buffer NewBuffer;
		NewBuffer.CreateBuffer(pBufferTarget, pSize, pData, pCount, pComponentType);
		return s_Buffers.AddTail(std::move(NewBuffer));
	}

	uint32 BufferHandler::CopyDataChunk(Id pBufferID, uint32 pOffset, uint32 pSize, void * pData)
	{
		s_Buffers.GetDataByID(pBufferID).CopyDataChunk(pOffset, pSize, pData);
		return HYD_OK;
	}


	Buffer& BufferHandler::GetBuffer(Id pBufferID)
	{
		if (pBufferID == 0)
			__debugbreak();
		return s_Buffers.GetDataByID(pBufferID);
	}

	VertexArray & BufferHandler::GetArray(Id pArrayID)
	{
		if (pArrayID == 0)
			__debugbreak();
		return s_Arrays.GetDataByID(pArrayID);
	}



//-----------------Binding Operations-----------------------------------

	uint32 BufferHandler::BindBuffer(Id pBufferID)
	{
		GetBuffer(pBufferID).Bind();
		return HYD_OK;
	}

	uint32 BufferHandler::UnbindBuffer(Id pBufferID)
	{
		GetBuffer(pBufferID).Unbind();
		return HYD_OK;
	}

	uint32 BufferHandler::BindArray(Id pArray)
	{
		GetArray(pArray).Bind();
		return HYD_OK;
	}

	uint32 BufferHandler::UnbindArray(Id pArray)
	{
		GetArray(pArray).Unbind();
		return HYD_OK;
	}

	HYD uint32 BufferHandler::BindCollection(Id pVAO, Id pVBO, Id pEBO)
	{
		GetArray(pVAO).Bind();
		GetArray(pVBO).Bind();		
		GetArray(pEBO).Bind();
		return HYD_OK;
	}
	HYD uint32 BufferHandler::UnbindCollection(Id pVAO, Id pVBO, Id pEBO)
	{
		GetArray(pVAO).Unbind();
		GetArray(pVBO).Unbind();
		GetArray(pEBO).Unbind();
		return HYD_OK;
	}
};