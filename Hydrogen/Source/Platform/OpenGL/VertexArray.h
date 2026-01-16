#pragma once

#include "Common.h"
#include "Glew/glew.h"
#include "Buffer.h"
#include "Model/Definitions.h"
#include <vector>

namespace Hydrogen
{


	class VertexArray
	{
	public:

		 VertexArray() = default;
		 VertexArray(VertexArray&& Other);
		 ~VertexArray();

		int CreateVertexArray();
		int DestroyVertexArray();

		int AddAttribute(Accessor& pAccessor);
		int DisableAttributes();

		void Bind();
		void Unbind();

	private:
		uint32 GetTypeSize(uint32 pType);

	private:
		uint32 m_Offset = 0;
		uint32 m_EnabledAttributes = 0;
		GLuint m_VertexID = 0;
	};

};