#include "Mesh.h"
#include "HydPch.h"


#include "Render/Renderer.h"

namespace Hydrogen 
{
	Mesh::Mesh()
	{
	}

	Mesh::Mesh(const std::string& pName, std::vector<Primitive>&& pPrimitives, Transform pTransform)
	{
		m_Name			 = pName;
		m_Primitives	 = std::move(pPrimitives);
		m_ModelMatrix = CalculateMatrix(pTransform);
	}

	Mesh::Mesh(Mesh && pOther)
	{
		m_Primitives = std::move(pOther.m_Primitives);
		m_Name = std::move(pOther.m_Name);
	}

	uint32 Mesh::SetMesh(const std::string& pName, std::vector<Primitive>&& pPrimitives, Transform pTransform)
	{
		m_Name = pName;
		m_Primitives = std::move(pPrimitives);
		m_ModelMatrix = CalculateMatrix(pTransform);

		return HYD_OK;
	}

	uint32 Mesh::SetTransform(const Transform& pTransform) 
	{
		m_ModelMatrix = m_ModelMatrix * CalculateMatrix(pTransform);
		return HYD_OK;
	}

	HYD uint32 Mesh::SetTransform(const Mat4& pTransform)
	{
		m_ModelMatrix = m_ModelMatrix * pTransform;
		return HYD_OK;
	}

	uint32 Mesh::PushPremitive(const Primitive& pPrimitive)
	{
		m_Primitives.emplace_back(pPrimitive);
		return HYD_OK;
	}

	Primitive& Mesh::GetPrimitve(uint32 pIndex)
	{
		if (pIndex >= m_Primitives.size())
		{
			ASSERT("Index Out of Range");
		}
		return m_Primitives[pIndex];
	}

	Mesh& Mesh::operator=(Mesh&& pOther)
	{
		if (this != &pOther)
		{
			m_Primitives = std::move(pOther.m_Primitives);
			m_Name = std::move(pOther.m_Name);
		}
		return *this;
	}

	uint32 Mesh::Render(const Mat4& pTransform)
	{
		for (auto& pri : m_Primitives)
		{

			pri.m_ModelMatrix = (pTransform*m_ModelMatrix).Transpose();
			Renderer::PushPrimitive(&pri);
		}
		return HYD_OK;
	}


	Primitive::Primitive()
	{
		//Asign the Shader to primitves:
		m_Shader = Renderer::GetDefaultShader();
	}

};