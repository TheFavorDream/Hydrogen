#pragma once

#include "Common.h"

namespace Hydrogen
{

	enum Attributes
	{
		POSITION = 0,
		NORMALS = 1,
		TEXCOORDS_0 = 2,
		TEXCOORDS_1 = 3,
		TANGENT = 4,
		COLOR_0 = 5
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

		Layout(DataType pType, uint32 pCount, uint32 pAttribute, bool pNormalized = false, uint32 pOffset = 0, uint32 pStride = 0)
			: Count(pCount), Offset(pOffset), Stride(pStride), Type(pType), Attribute(pAttribute), Normalized(pNormalized)
		{

		}

		uint32     Count = 0;
		uint32     Offset = 0;
		uint32     Stride = 0;
		uint32     Attribute = 0;
		DataType   Type = TYPE_FLOAT;
		bool       Normalized = false;

	};

	class VertexArray
	{
	public:

		VertexArray();
	   ~VertexArray();

	   VertexArray(const VertexArray& pOther) = delete;
	   VertexArray(VertexArray&& pOther);

	   VertexArray& operator=(const VertexArray& pOther) = delete;
	   VertexArray& operator=(VertexArray&& pOther);

	   HYD uint32 CreateVertexArray();
	   HYD uint32 DestroyVertexArray();

	   HYD uint32 AddAttribute(const Layout& pLayout);
	   HYD uint32 AddAttributes(const std::vector<Layout>& pAttributes);

	   HYD uint32 EnableAttribute(uint32    pAttribute);
	   HYD uint32 DisableAttributes(uint32  pAttribute);

	   HYD void Bind();
	   HYD void Unbind();

	private:
		void* m_Object = nullptr;
	};

};