#include "Mesh.h"
#include "HydPch.h"


#include "Render/Renderer.h"

namespace Hydrogen 
{


	Mesh Mesh::CreateGLTFMesh(const Xenon::Mesh & pMesh)
	{
		Mesh NewMesh;


		return std::move(NewMesh);
	}


	Mesh::Mesh()
	{

	}


	Mesh::Mesh(const std::string& pName, std::vector<Primitive>&& pPrimitives)
	{
		m_Name			 = pName;
		m_Primitives	 = std::move(pPrimitives);
	}

	Mesh::Mesh(Mesh&& pOther)
	{
		m_Name		 = std::move(pOther.m_Name);
		m_Primitives = std::move(pOther.m_Primitives);
	}

	Mesh::Mesh(const Mesh& pOther)
	{
	
	}

	Mesh& Mesh::operator=(const Mesh& pOther)
	{
		return *this;
	}


	Mesh& Mesh::operator=(Mesh&& pOther)
	{
		if (this != &pOther)
		{
			m_Name = std::move(pOther.m_Name);
			m_Primitives = std::move(pOther.m_Primitives);
		}
		return *this;
	}

	uint32 Mesh::PushPremitive(const Primitive& pPrimitive)
	{
		m_Primitives.emplace_back(pPrimitive);
		return HYD_OK;
	}

	Primitive& Mesh::GetPrimitve(uint64 pIndex)
	{
		
		ASSERT((pIndex < m_Primitives.size()), "Index Out of Range");
		return m_Primitives[pIndex];
	}



	uint32 Mesh::Render(const Transformation& m_Transform)
	{

		//if (m_Transform.IsDirty)
		MatF4 Matrix = Transformation::CalculateMatrix(m_Transform);

		for (auto& pri : m_Primitives)
		{
			pri.m_Transform = Matrix;
			Renderer::Self().PushPrimitive(&pri);
		}
		return HYD_OK;
	}







	Primitive::Primitive()
	{
		//Asign the Shader to primitves:
		//m_Shader   = Renderer::GetDefaultShader();
		//m_Material = Renderer::GetDefaultMaterial();
	}

	Primitive::Primitive(const Primitive& pOther)
	{

	}

	Primitive::Primitive(Primitive&& pOther)
	{

		m_Transform     = std::move(pOther.m_Transform);
		m_Material		= std::move(pOther.m_Material);
		//m_Shader		= std::move(pOther.m_Shader);

		m_RenderingMode = pOther.m_RenderingMode;

	}


};