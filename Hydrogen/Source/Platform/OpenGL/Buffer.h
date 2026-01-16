/*
	OpenGL Vertex Buffer Definitions.
*/

#pragma once


#include "Glew/glew.h"
#include "Common.h"


namespace Hydrogen
{

	class Buffer
	{
	public:

		Buffer() = default;
		~Buffer();


		//Move Constructor
		Buffer(Buffer&& pMove);

		//Creates an OpenGL Vertex Buffer and fills it with Data
		int CreateBuffer(GLenum pBufferTarget, uint32 pSize, int8* pData=0, uint32 pCount=0);
		//Copies the data to a specific offset
		int CopyDataChunk(uint32 pOffset, uint32 pSize, int8* pData);
		//Free's the Buffer
		int DestroyBuffer();

		//Binds the Current Vertex Buffer
		void Bind() const;
		// Binds the current buffer to zero
		void Unbind() const;

		inline GLuint GetVertexID() { return m_BufferID; }
		inline GLuint GetBufferSize() { return m_BufferSize; }
		inline GLenum GetBufferTarget() const { return m_BufferTarget; }
		inline uint32 GetCount() const { return m_BufferSize; }

	private:
		GLenum m_BufferTarget;
		GLuint m_BufferID = 0;
		GLuint m_BufferSize = 0; // Size of the Buffer in bytes
	};

};
