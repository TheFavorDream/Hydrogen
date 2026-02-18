#include "BufferHandler.h"

namespace Hydrogen
{

	Id	   BufferHandler::s_CurrentBindedVertexBuffer  = 0;
	Id	   BufferHandler::s_CurrentBindedElementBuffer = 0;
	Id	   BufferHandler::s_CurrentBindedVertexArray   = 0;
	uint32 BufferHandler::s_Buffers = 0;

	std::vector<Id> BufferHandler::s_BufferID;
	std::vector<Id> BufferHandler::s_ArrayID;

	uint32 BufferHandler::InitHandler()
	{
		return HYD_OK;
	}

	uint32 BufferHandler::ShutdownHandler()
	{
		//free Arrays:
		for (auto& id : s_ArrayID)
		{
			UnbindArray(id);
			ResourceHandler::PopResource(&id);
		}

		for (auto& id : s_BufferID)
		{
			UnbindBuffer(id);
			ResourceHandler::PopResource(&id);
		}
		return HYD_OK;
	}


	Id BufferHandler::PushVertexArray(VertexArray* pVAO)
	{
		Id ID = ResourceHandler::PushResource(pVAO);
		s_ArrayID.push_back(ID);
		return ID;
	}

	uint32 BufferHandler::DestroyVertexArray(Id* pVao)
	{
		UnbindArray(*pVao);
		ResourceHandler::PopResource(pVao);
		return HYD_OK;
	}

	Id BufferHandler::CreateVertexArray()
	{
		VertexArray* VAO = new VertexArray();
		VAO->CreateVertexArray();
		Id ID = ResourceHandler::PushResource(VAO);
		s_ArrayID.push_back(ID);
		return ID;
	}

	uint32 BufferHandler::AddVertexAttribute(Id pVao, const Accessor & pAccessor)
	{
		GetArray(pVao).AddAttribute(pAccessor);
		return HYD_OK;
	}



	Id BufferHandler::PushBuffer(Buffer* pBuffer)
	{
		return ResourceHandler::PushResource(pBuffer);
	}

	uint32 BufferHandler::DestroyBuffer(Id * pBufferID)
	{
		ResourceHandler::PopResource(pBufferID);
		return HYD_OK;
	}

	Id BufferHandler::CreateBuffer(GLenum pBufferTarget, uint32 pSize, void * pData, uint32 pCount, uint32 pComponentType)
	{
		Buffer* NewBuffer = new Buffer();
		NewBuffer->CreateBuffer(pBufferTarget, pSize, pData, pCount, pComponentType);
		Id ID = ResourceHandler::PushResource(NewBuffer);
		s_BufferID.push_back(ID);
		return ID;
	}

	uint32 BufferHandler::CopyDataChunk(Id pBufferID, uint32 pOffset, uint32 pSize, void * pData)
	{
		GetBuffer(pBufferID).CopyDataChunk(pOffset, pSize, pData);
		return HYD_OK;
	}


	Buffer& BufferHandler::GetBuffer(Id pBufferID)
	{
		if (pBufferID == 0)
			__debugbreak();
		return *dynamic_cast<Buffer*>(ResourceHandler::GetResource(pBufferID));
	}

	VertexArray & BufferHandler::GetArray(Id pArrayID)
	{
		if (pArrayID == 0)
			__debugbreak();
		return *dynamic_cast<VertexArray*>(ResourceHandler::GetResource(pArrayID));
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