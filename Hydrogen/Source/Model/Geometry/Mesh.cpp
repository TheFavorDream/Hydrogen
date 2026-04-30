#include "Mesh.h"
#include "HydPch.h"


#include "Render/Renderer.h"

namespace Hydrogen 
{
	Mesh::Mesh()
	{
		m_Matrix = Core::GetCurrentScene()->m_Matrices.GenerateMatrix();
	}

	Mesh::Mesh(const std::string& pName, std::vector<Primitive>&& pPrimitives, const Transformation& pTransform)
	{
		m_Name			 = pName;
		m_Primitives	 = std::move(pPrimitives);
		m_Transform		 = pTransform;
		m_Matrix		 = Core::GetCurrentScene()->m_Matrices.GenerateMatrix();
	}

	Mesh::Mesh(Mesh&& pOther)
	{
		m_Name		 = std::move(pOther.m_Name);
		m_Primitives = std::move(pOther.m_Primitives);
		m_Transform	 = std::move(pOther.m_Transform);
		m_Matrix	 = pOther.m_Matrix;

		pOther.m_Matrix = 0;
	}

	uint32 Mesh::SetMesh(const std::string& pName, std::vector<Primitive>&& pPrimitives, const Transformation& pTransform)
	{
		m_Name		 = pName;
		m_Primitives = std::move(pPrimitives);
		m_Transform  = pTransform;
		m_Matrix	 = Core::GetCurrentScene()->m_Matrices.GenerateMatrix();


		return HYD_OK;
	}

	uint32 Mesh::SetTransform(const Transformation& pTransform)
	{
		m_Transform = pTransform;
		return HYD_OK;
	}


	uint32 Mesh::PushPremitive(const Primitive& pPrimitive)
	{
		m_Primitives.emplace_back(pPrimitive);
		return HYD_OK;
	}

	Primitive& Mesh::GetPrimitve(uint32 pIndex)
	{
		
		ASSERT((pIndex >= m_Primitives.size()), "Index Out of Range");
		return m_Primitives[pIndex];
	}

	Mesh& Mesh::operator=(Mesh&& pOther)
	{
		if (this != &pOther)
		{
			m_Name		 = std::move(pOther.m_Name);
			m_Primitives = std::move(pOther.m_Primitives);
			m_Matrix	 = pOther.m_Matrix;

			pOther.m_Matrix = 0;
		}
		return *this;
	}

	uint32 Mesh::Render(Transformation& pTransform)
	{

		Transformation Result = (pTransform*m_Transform);
		
		if (Result.IsDirty)
		{
			Core::GetCurrentScene()->m_Matrices.CalculateMatrix(m_Matrix, Result);
			m_Transform.IsDirty = false;


#ifndef DIST
			std::string scale      = "S:("     + std::to_string(m_Transform.t_Scale.X) + "," + std::to_string(m_Transform.t_Scale.Y) + "," + std::to_string(m_Transform.t_Scale.Z) + ") ";
			std::string rotate     = "R:("    + std::to_string(m_Transform.t_Rotate.X) + "," + std::to_string(m_Transform.t_Rotate.Y) + "," + std::to_string(m_Transform.t_Rotate.Z) + "," + std::to_string(m_Transform.t_Rotate.W) +  ") ";
			std::string translate  = "T:(" + std::to_string(m_Transform.t_Translate.X) + "," + std::to_string(m_Transform.t_Translate.Y) + "," + std::to_string(m_Transform.t_Translate.Z) + ") ";

			Log::SetInfo("Calculating " + m_Name + ":" + scale + rotate + translate);
#endif
		}

		for (auto& pri : m_Primitives)
		{
			pri.m_Matrix = m_Matrix;
			Renderer::PushPrimitive(&pri);
		}
		return HYD_OK;
	}


	Primitive::Primitive()
	{
		//Asign the Shader to primitves:
		m_Shader   = Renderer::GetDefaultShader();
		m_Material = Renderer::GetDefaultMaterial();
	}

};