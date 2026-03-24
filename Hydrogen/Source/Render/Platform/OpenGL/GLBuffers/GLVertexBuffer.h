/*
	OpenGL Vertex Buffer Definitions.
*/

#pragma once


#include "Glew/glew.h"
#include "Common.h"
#include "Log/Log.h"
#include "GLBuffer.h"

namespace Hydrogen
{

	class GLVertexBuffer : public GLBuffer 
	{
	public:

		HYD  GLVertexBuffer() = default;
		HYD ~GLVertexBuffer();

		HYD GLVertexBuffer(const GLVertexBuffer& pOther) = delete;
		HYD GLVertexBuffer& operator=(const GLVertexBuffer& pOther) = delete;
		
		HYD GLVertexBuffer(GLVertexBuffer&& pMove);
		HYD GLVertexBuffer& operator=(GLVertexBuffer&& pOther);
		 
		 //Binds the Current Vertex Buffer
		HYD void Bind() const override;
		 // Binds the current buffer to zero
		HYD void Unbind() const override;
		 

	};

};
