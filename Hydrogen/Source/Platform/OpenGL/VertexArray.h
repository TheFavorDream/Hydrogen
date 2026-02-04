#pragma once

#include "Common.h"
#include "Glew/glew.h"
#include "Buffer.h"
#include <vector>
#include "Log/Log.h"
#include "Model/Loader/Definitions.h"

namespace Hydrogen
{


	/*
		Each Primitive has a Attribute.
		Attribute Objects store the Index for the specific Vertex Array Attrib Pointer.
	*/
	struct Attribute
	{
		int32 POSITION = -1;
		int32 NORMALS = -1;
		int32 TANGENT = -1;
		int32 TEXCOORDS_0 = -1;
		int32 TEXCOORDS_1 = -1;
		int32 COLOR_0 = -1;
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


		HYD int CreateVertexArray();
		HYD int DestroyVertexArray();

		HYD int AddAttribute(const Accessor& pAccessor);
		HYD int DisableAttributes();

		HYD void Bind();
		HYD void Unbind();

	private:
		HYD uint32 GetTypeSize(uint32 pType);

	private:
		uint32 m_Offset = 0;
		uint32 m_EnabledAttributes = 0;
		GLuint m_VertexID = 0;
	};

};