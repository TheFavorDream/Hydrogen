#pragma once

#include "Common.h"
#include "Glew/glew.h"
#include <vector>
#include "Log/Log.h"
#include "Render/VertexArray.h"

namespace Hydrogen
{



	class GLVertexArray
	{
	public:

		HYD GLVertexArray() = default;
		HYD ~GLVertexArray();

		HYD GLVertexArray(const GLVertexArray& pOther) = delete;
		HYD GLVertexArray(GLVertexArray&& pOther);

		HYD GLVertexArray& operator=(const GLVertexArray& pOther) = delete;
		HYD GLVertexArray& operator=(GLVertexArray&& pOther);


		HYD uint32 CreateVertexArray();
		HYD uint32 DestroyVertexArray();

		HYD uint32 AddAttribute(const Layout& pLayout);
		HYD uint32 AddAttributes(const std::vector<Layout>& pAttributes);

		HYD uint32 EnableAttribute(uint32    pAttribute);
		HYD uint32 DisableAttributes(uint32  pAttribute);

		HYD void Bind();
		HYD void Unbind();

	public:

		HYD static GLenum   GetGLType(DataType pType);
		HYD static DataType GetHydType(GLenum pType);
		HYD static uint32   GetTypeSize(DataType pType);

	private:


		uint32	m_Attributes = 0;
		GLuint	m_VertexID   = 0;
	};

};