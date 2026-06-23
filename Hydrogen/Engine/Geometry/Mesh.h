#pragma once

#include "Common.h"
#include "Render/Platform/OpenGL/GLVertexArray.h"
#include "Primitive.h"
#include "VecMath/Transform/Transformation.h"
#include "Core/ResourcePool.h"

namespace Hydrogen
{


	struct Vertex
	{
		VecF3 Position;
		VecF3 Normal;
		VecF2 TexCoord;
	};

	class Mesh
	{
	public:



		HYD Mesh();
		HYD Mesh(const std::string& pName, std::vector<Primitive>&& pPrimitives);

		HYD Mesh(const Mesh& pOther) = delete;
		HYD Mesh(Mesh&& pOther);

		HYD Mesh& operator=(const Mesh& pOther) = delete;
		HYD Mesh& operator=(Mesh&& pOther);

		HYD uint32 SetMesh(const std::string& pName, std::vector<Primitive>&& pPrimitives);
		
		HYD uint32     PushPremitive(const Primitive& pPrimitive);
		HYD Primitive& GetPrimitve(uint32 pIndex);

		HYD inline const std::string GetName() { return m_Name; }

		HYD uint32 Render(const Transformation& m_Transform =Transformation());


	private:
		std::string				m_Name; 
		HYD_VEC<Primitive>      m_Primitives;
		Transformation			m_Transform;
	};

};