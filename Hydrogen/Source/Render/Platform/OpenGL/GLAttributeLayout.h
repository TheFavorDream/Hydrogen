#pragma once

#include "Common.h"
#include "GLVertexArray.h"

namespace Hydrogen
{

	enum DataType {
		TYPE_FLOAT		,
		TYPE_UNSIGNED_INT,
		TYPE_SIGNED_INT	,
		TYPE_UNSIGNED_SHORT,
		TYPE_SIGNED_SHORT  ,
		TYPE_UNSIGNED_BYTE ,
		TYPE_SIGNED_BYTE   
	};

	struct Layout
	{
		DataType Type;
		uint32   Count;
		bool     Normalized;


		static uint32 GetTypeSize(DataType pType)
		{

			switch (pType)
			{
			case TYPE_FLOAT:
				return 4;
			case TYPE_UNSIGNED_SHORT:
				return 2;
			case TYPE_SIGNED_SHORT:
				return 2;
			case TYPE_UNSIGNED_BYTE:
				return 1;
			case TYPE_SIGNED_BYTE:
				return 1;
			case TYPE_UNSIGNED_INT:
				return 4;
			case TYPE_SIGNED_INT:
				return 4;
			}
			return 0;
		}
	};

	class Layouts
	{
	public:


		template <typename T>
		uint32 PushAttribute(uint32 pCount, bool pNormalized)
		{

			return HYD_OK;
		}

		template <>
		uint32 PushAttribute<float>(uint32 pCount, bool pNormalized)
		{
			m_Layouts.push_back({TYPE_FLOAT, pCount, pNormalized});
			m_Stride += pCount * Layout::GetTypeSize(TYPE_FLOAT);
			return HYD_OK;
		}

		inline std::vector<Layout>::iterator begin() { return m_Layouts.begin(); }
		inline std::vector<Layout>::iterator end() { return m_Layouts.end(); }
	
		inline uint32 GetStride() const { return m_Stride; }

	private:
		uint32              m_Stride = 0;
		std::vector<Layout> m_Layouts;
	};


};