#pragma once

#include "Common.h"
#include "Model/Definitions.h"
#include "Platform/OpenGL/VertexArray.h"
#include "Platform/OpenGL/Buffer.h"
#include "Model/Model.h"

namespace Hydrogen
{

	class Mesh
	{
	public:

		Mesh()=default;
		Mesh(std::string pName, std::vector<Primitive>& pPrimitives, uint32 pDefaultPrimitive=0);
		Mesh(Mesh&& Other);
		~Mesh();

		void Render(Model* pModel);

		inline const std::string& GetMeshName() const { return m_Name; }
		inline const uint32 GetDefaultPrimitive() const { return m_DefaultPrimitive; }

	private:
		std::string m_Name;
		std::vector<Primitive> m_Primitives;
		uint32 m_DefaultPrimitive;
	};

};