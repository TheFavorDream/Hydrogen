#pragma once

#include "Common.h"


#include "VecMath/Transform/Transformation.h"
#include "VecMath/Math.h"
#include "Render/Platform/OpenGL/GLVertexArray.h"
#include "Core/ResourcePool.h"

namespace Hydrogen
{


	struct Vertex
	{
		VecF3 Position;
		VecF3 Normal;
		VecF2 TexCoord;
	};

	struct Primitive
	{
		Id		   m_VertexArrays  = 0;
		Id	       m_VertexBuffer  = 0;
		Id         m_ElementBuffer = 0;
		Id         m_Material      = 0;
		Id         m_Shader		   = 0;
		Id         m_Matrix		   = 0;
		uint16     m_RenderingMode = 4;


		HYD Primitive();

	};

	class Mesh
	{
	public:



		HYD Mesh();
		HYD Mesh(const std::string& pName, std::vector<Primitive>&& pPrimitives, const Transformation& pTransform= Transformation());

		HYD Mesh(const Mesh& pOther) = delete;
		HYD Mesh(Mesh&& pOther);

		HYD Mesh& operator=(const Mesh& pOther) = delete;
		HYD Mesh& operator=(Mesh&& pOther);

		HYD uint32 SetMesh(const std::string& pName, std::vector<Primitive>&& pPrimitives, const Transformation& pTransform = Transformation());
		HYD uint32 SetTransform(const Transformation& pTransform);

		HYD inline Transformation& GetTransform() { return m_Transform; }
		
		HYD uint32     PushPremitive(const Primitive& pPrimitive);
		HYD Primitive& GetPrimitve(uint32 pIndex);

		HYD inline const std::string GetName() { return m_Name; }

		HYD uint32 Render(Transformation& pTransform = Transformation());

	private:

		std::string				m_Name; 
		std::vector<Primitive>  m_Primitives;
		Transformation			m_Transform;
		Id					    m_Matrix = 0;

		friend class Model;
		friend class GLTFLoader;
	};

};