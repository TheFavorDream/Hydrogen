#pragma once

#include "Common.h"


#include "VecMath/Transformation.h"
#include "VecMath/Math.h"
#include "Render/Platform/OpenGL/GLVertexArray.h"
#include "Core/ResourcePool.h"

namespace Hydrogen
{


	struct Vertex
	{
		Vec3 Position;
		Vec3 Normal;
		Vec2 TexCoord;
	};

	struct Primitive
	{
		Mat4       m_ModelMatrix;

		Id		   m_VertexArrays  = 0;
		Id	       m_VertexBuffer  = 0;
		Id         m_ElementBuffer = 0;
		Id         m_Material      = 0;
		Id         m_Shader		   = 0;
		uint32     m_RenderingMode = 4;
		//Indices count
		uint32 Count = 0;
		uint32 Type  = GL_UNSIGNED_BYTE;

		HYD Primitive();
	};

	class Mesh
	{
	public:



		HYD Mesh();
		HYD Mesh(const std::string& pName, std::vector<Primitive>&& pPrimitives, Transform pTransform=Transform());

		HYD Mesh(const Mesh& pOther) = delete;
		HYD Mesh(Mesh&& pOther);

		HYD Mesh& operator=(const Mesh& pOther) = delete;
		HYD Mesh& operator=(Mesh&& pOther);

		HYD uint32 SetMesh(const std::string& pName, std::vector<Primitive>&& pPrimitives, Transform pTransform=Transform());
		HYD uint32 SetTransform(const Transform& pTransform);
		HYD uint32 SetTransform(const Mat4&      pTransform);


		HYD uint32     PushPremitive(const Primitive& pPrimitive);
		HYD Primitive& GetPrimitve(uint32 pIndex);

		HYD uint32 Render(const Mat4& pTransform=Mat4());

	private:

		std::string				m_Name; 
		std::vector<Primitive>  m_Primitives;
		Mat4					m_ModelMatrix;

		friend class Model;
		friend class GLTFLoader;
	};

};