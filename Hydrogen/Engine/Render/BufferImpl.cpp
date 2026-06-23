#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "VertexArray.h"
#include "Renderer.h"

#include "Render/Platform/OpenGL/GLVertexBuffer.h"
#include "Render/Platform/OpenGL/GLElementBuffer.h"
#include "Render/Platform/OpenGL/GLVertexArray.h"

namespace Hydrogen
{
	//Vertex Buffer Interface Impl
	//These classes dispatchs the right function based on rendering api
	VertexBuffer::VertexBuffer()
		: m_BufferObj(nullptr)
	{
	}

	VertexBuffer::~VertexBuffer()
	{
		if (m_BufferObj)
		{

			switch (Renderer::GetRenderingAPI())
			{
			case API_OPENGL:
				Ptr<GLVertexBuffer> ptr_VBO = static_cast<Ptr<GLVertexBuffer>>(m_BufferObj);
				delete ptr_VBO;
				m_BufferObj = nullptr;
				break;
			}
		}
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& pOther)
	{
		m_BufferObj = pOther.m_BufferObj;
		pOther.m_BufferObj = nullptr;
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& pOther)
	{
		if (&pOther == this)
			return *this;

		m_BufferObj = pOther.m_BufferObj;
		pOther.m_BufferObj = nullptr;
		return *this;
	}

	uint32 VertexBuffer::CreateBuffer()
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			Ptr<GLVertexBuffer> VBO = Memory::AllocateRaw<GLVertexBuffer>();
			m_BufferObj = static_cast<void*>(VBO);
			return VBO->CreateBuffer();
		}

		return HYD_FAILED;
	}

	uint32 VertexBuffer::CreateBuffer(uint32 pSize, void* pData, uint32 pUsageHint)
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			Ptr<GLVertexBuffer> VBO = Memory::AllocateRaw<GLVertexBuffer>();
			m_BufferObj = static_cast<void*>(VBO);
			return VBO->CreateBuffer(pSize, pData, pUsageHint);
		}

		return HYD_FAILED;
	}


	uint32 VertexBuffer::DeleteBuffer()
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLVertexBuffer>>(m_BufferObj)->DeleteBuffer();
		}
		return HYD_FAILED;
	}

	uint32 VertexBuffer::WriteBuffer(uint32 pSize, void* pData, uint32 pUsageHint)
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLVertexBuffer>>(m_BufferObj)->WriteBuffer(pSize, pData, pUsageHint);
		}
		return HYD_FAILED;
	}

	uint32 VertexBuffer::WriteChunk(uint32  pOffset, uint32 pSize, void* pData)
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLVertexBuffer>>(m_BufferObj)->WriteChunk(pOffset, pSize, pData);
		}
		return HYD_FAILED;
	}

	uint32 VertexBuffer::ReadBuffer(uint32  pOffset, uint32 pBytesToRead, std::string& pData)
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLVertexBuffer>>(m_BufferObj)->ReadBuffer(pOffset, pBytesToRead, pData);
		}
		return HYD_FAILED;
	}

	uint32 VertexBuffer::ClearBuffer(uint32 pClearValue)
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLVertexBuffer>>(m_BufferObj)->ClearBuffer();
		}
		return HYD_FAILED;
	}

	void VertexBuffer::Bind()
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			static_cast<Ptr<GLVertexBuffer>>(m_BufferObj)->Bind();
			break;
		}
	}

	void VertexBuffer::Unbind()
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			static_cast<Ptr<GLVertexBuffer>>(m_BufferObj)->Unbind();
			break;
		}
	}


	//Element Buffer Interface Impl

	ElementBuffer::ElementBuffer()
	{

	}

	ElementBuffer::~ElementBuffer()
	{
		if (m_Object)
		{

			switch (Renderer::GetRenderingAPI())
			{
			case API_OPENGL:
				delete static_cast<Ptr<GLElementBuffer>>(m_Object);
				m_Object = nullptr;
				break;
			}
		}
	}

	ElementBuffer::ElementBuffer(ElementBuffer&& pOther)
	{
		m_Object = pOther.m_Object;
		pOther.m_Object = nullptr;
	}


	ElementBuffer& ElementBuffer::operator=(ElementBuffer&& pOther)
	{
		if (&pOther == this)
			return *this;

		m_Object = pOther.m_Object;
		pOther.m_Object = nullptr;

		return *this;
	}

	uint32 ElementBuffer::CreateElementBuffer()
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			Ptr<GLElementBuffer> EBO = Memory::AllocateRaw<GLElementBuffer>();
			m_Object = static_cast<void*>(EBO);
			return EBO->CreateElementBuffer();
		}
		return HYD_FAILED;
	}

	uint32 ElementBuffer::CreateElementBuffer(uint32 pCount, GLenum pType, void* pData)
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			Ptr<GLElementBuffer> EBO = Memory::AllocateRaw<GLElementBuffer>();
			m_Object = static_cast<void*>(EBO);
			return EBO->CreateElementBuffer(pCount, pType, pData);
		}
		return HYD_FAILED;
	}

	uint32 ElementBuffer::DeleteBuffer()
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLElementBuffer>>(m_Object)->DeleteBuffer();
		}
		return HYD_FAILED;
	}

	uint32 ElementBuffer::InitElementBuffer(uint32 pCount, GLenum pType, void* pData)
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLElementBuffer>>(m_Object)->InitElementBuffer(pCount, pType, pData);
		}
		return HYD_FAILED;
	}

	uint32 ElementBuffer::WriteChunk(uint32 pOffset, uint32 pSize, void* pData)
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLElementBuffer>>(m_Object)->WriteChunk(pOffset, pSize, pData);
		}
		return HYD_FAILED;
	}

	uint32 ElementBuffer::ReadBuffer(uint32 pOffset, uint32 pBytesToRead, std::string& pData)
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLElementBuffer>>(m_Object)->ReadBuffer(pOffset, pBytesToRead, pData);
		}
		return HYD_FAILED;
	}

	void ElementBuffer::Bind() const
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			static_cast<Ptr<GLElementBuffer>>(m_Object)->Bind();
			break;
		}
	}

	void ElementBuffer::Unbind() const
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			static_cast<Ptr<GLElementBuffer>>(m_Object)->Unbind();
			break;
		}
	}

	uint32 ElementBuffer::GetCount() const
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLElementBuffer>>(m_Object)->GetCount();
		}
		return 0;
	}

	GLenum ElementBuffer::GetType() const
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLElementBuffer>>(m_Object)->GetType();
		}
		return HYD_FAILED;
	}



	//Vertex Array Interface:

	VertexArray::VertexArray()
	{

	}

	VertexArray::~VertexArray()
	{
		if (m_Object)
		{

			switch (Renderer::GetRenderingAPI())
			{
			case API_OPENGL:
				delete static_cast<Ptr<GLVertexArray>>(m_Object);
				m_Object = nullptr;
				break;
			}
		}
	}

	VertexArray::VertexArray(VertexArray&& pOther)
	{
		m_Object = pOther.m_Object;
		pOther.m_Object = nullptr;
	}


	VertexArray& VertexArray::operator=(VertexArray&& pOther)
	{
		if (&pOther == nullptr)
			return *this;

		m_Object = pOther.m_Object;
		pOther.m_Object = nullptr;
		return *this;
	}

	uint32 VertexArray::CreateVertexArray()
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			Ptr<GLVertexArray> VAO = Memory::AllocateRaw<GLVertexArray>();
			m_Object = static_cast<void*>(VAO);
			return VAO->CreateVertexArray();
		}
		return HYD_FAILED;
	}

	uint32 VertexArray::DestroyVertexArray()
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLVertexArray>>(m_Object)->DestroyVertexArray();
		}
		return HYD_FAILED;
	}

	uint32 VertexArray::AddAttribute(const Layout& pLayout)
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLVertexArray>>(m_Object)->AddAttribute(pLayout);
		}
		return HYD_FAILED;
	}
	uint32 VertexArray::AddAttributes(const std::vector<Layout>& pAttributes)
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLVertexArray>>(m_Object)->AddAttributes(pAttributes);
		}
		return HYD_FAILED;
	}

	uint32 VertexArray::EnableAttribute(uint32    pAttribute)
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLVertexArray>>(m_Object)->EnableAttribute(pAttribute);
		}
		return HYD_FAILED;
	}
	uint32 VertexArray::DisableAttributes(uint32  pAttribute)
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLVertexArray>>(m_Object)->DisableAttributes(pAttribute);
		}
		return HYD_FAILED;
	}

	void VertexArray::Bind() 
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			static_cast<Ptr<GLVertexArray>>(m_Object)->Bind();
			break;
		}
	}
	void VertexArray::Unbind()
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			static_cast<Ptr<GLVertexArray>>(m_Object)->Unbind();
			break;
		}
	}

};