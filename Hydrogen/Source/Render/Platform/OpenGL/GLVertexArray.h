#pragma once

#include "Common.h"
#include "Glew/glew.h"
#include "GLBuffers/GLBuffer.h"
#include <vector>
#include "Log/Log.h"

namespace Hydrogen
{

	enum Attributes
	{
		 POSITION     = 0,
		 NORMALS	  = 1,
		 TEXCOORDS_0  = 2,
		 TEXCOORDS_1  = 3,
		 TANGENT	  = 4,
		 COLOR_0	  = 5
	};


	enum DataType {
		TYPE_FLOAT,
		TYPE_UNSIGNED_INT,
		TYPE_SIGNED_INT,
		TYPE_UNSIGNED_SHORT,
		TYPE_SIGNED_SHORT,
		TYPE_UNSIGNED_BYTE,
		TYPE_SIGNED_BYTE
	};


	struct Layout
	{

		Layout() = default;

		Layout(DataType pType, uint32 pCount, uint32 pAttribute, bool pNormalized = false, uint32 pOffset =0, uint32 pStride=0)
			: Count(pCount), Offset(pOffset), Stride(pStride), Type(pType), Attribute(pAttribute) , Normalized(pNormalized)
		{

		}

		uint32     Count      = 0;
		uint32     Offset     = 0; 
		uint32     Stride     = 0; 
		uint32     Attribute  = 0;
		DataType   Type       = TYPE_FLOAT;
		bool       Normalized = false;

	};

	class VertexArray
	{
	public:

		HYD VertexArray() = default;
		HYD ~VertexArray();

		HYD VertexArray(const VertexArray& pOther) = delete;
		HYD VertexArray(VertexArray&& pOther);

		HYD VertexArray& operator=(const VertexArray& pOther) = delete;
		HYD VertexArray& operator=(VertexArray&& pOther);


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


		uint32										 m_Attributes = 0;
		GLuint										 m_VertexID = 0;
	};

};