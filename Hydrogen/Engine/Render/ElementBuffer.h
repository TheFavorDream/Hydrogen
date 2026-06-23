#pragma once

#include "Common.h"
#include "Glew/glew.h"

namespace Hydrogen
{
	//Main Interface for Element Buffers
	class ElementBuffer
	{
	public:

		ElementBuffer();
	   ~ElementBuffer();

	   ElementBuffer(const ElementBuffer& pOther) = delete;
	   ElementBuffer(ElementBuffer&& pOther);

	   ElementBuffer& operator=(const ElementBuffer& pOther) = delete;
	   ElementBuffer& operator=(ElementBuffer&& pOther);

	   HYD uint32 CreateElementBuffer();
	   HYD uint32 CreateElementBuffer(uint32 pCount, GLenum pType, void* pData);
	   HYD uint32 DeleteBuffer();
	   HYD uint32 InitElementBuffer(uint32 pCount, GLenum pType, void* pData);

	   HYD uint32 WriteChunk(uint32 pOffset, uint32 pSize, void* pData);
	   HYD uint32 ReadBuffer(uint32 pOffset, uint32 pBytesToRead, std::string& pData);

	   HYD void Bind()    const;
	   HYD void Unbind()  const;

	   HYD uint32 GetCount() const; 
	   HYD GLenum GetType()  const;


	private:
		void* m_Object = nullptr;
	};

};