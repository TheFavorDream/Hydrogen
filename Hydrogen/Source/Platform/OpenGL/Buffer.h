/*
	OpenGL Vertex Buffer Definitions.
*/

#pragma once


#include "Glew/glew.h"
#include "Common.h"
#include "Log/Log.h"
#include "ResourceHandler/ResourceHandler.h"

namespace Hydrogen
{

	class Buffer : public Resource
	{
	public:

		HYD Buffer() = default;
		HYD ~Buffer();

		
		HYD Buffer(const Buffer& pOther) = delete;
		HYD Buffer& operator=(const Buffer& pOther) = delete;
		
		HYD Buffer(Buffer&& pMove);
		HYD Buffer& operator=(Buffer&& pOther);

		 //Creates an OpenGL Vertex Buffer and fills it with Data
		HYD int CreateBuffer(GLenum pBufferTarget, uint32 pSize, void* pData=0, uint32 pCount=0, uint32 pComponentType= GL_UNSIGNED_SHORT);
		 //Copies the data to a specific offset
		HYD int CopyDataChunk(uint32 pOffset, uint32 pSize, void* pData);
		 //Free's the Buffer
		HYD int DestroyBuffer();
		 
		 //Binds the Current Vertex Buffer
		HYD void Bind() const;
		 // Binds the current buffer to zero
		HYD void Unbind() const;
		 
		HYD inline GLuint GetVertexID() { return m_BufferID; }
		HYD inline GLuint GetBufferSize() { return m_BufferSize; }
		HYD inline GLenum GetBufferTarget() const { return m_BufferTarget; }
		HYD inline uint32 GetCount() const { return m_BufferSize; }
		HYD inline uint32 GetComponentType() { return m_EBO_ComponentType; }

	private:
		GLenum m_BufferTarget;
		GLuint m_BufferID = 0;
		GLuint m_BufferSize = 0; // Size of the Buffer in bytes

		uint32 m_EBO_ComponentType = GL_UNSIGNED_SHORT;
	};

};
