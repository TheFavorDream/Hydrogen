#include "Mesh.h"

namespace Hydrogen
{

	Mesh::Mesh(std::string pName, std::vector<Primitive>& pPrimitives)
	{
		m_Name		 = pName;
		m_Primitives = std::move(pPrimitives);
	}

	Mesh::~Mesh()
	{
		m_Primitives.clear();
	}

	Mesh::Mesh(Mesh && pOther)
	{
		m_Primitives = std::move(pOther.m_Primitives);
		m_Name       = std::move(pOther.m_Name);
	}

	Mesh& Mesh::operator=(Mesh&& pOther)
	{
		if (this != &pOther)
		{
			m_Primitives = std::move(pOther.m_Primitives);
			m_Name		 = std::move(pOther.m_Name);
		}
		return *this;
	}

	void Mesh::Render(const Shader & pShader, glm::mat4 * pTransform, Model* pCaller)
	{

		pShader.Bind();
		pShader.SetUniformMat4("Model", glm::value_ptr(m_Transformation));
		pShader.SetUniformInt1("BaseColor", 0);
		for (auto& i : m_Primitives)
		{
			//Rendering Process:

			BufferHandler::BindArray(i.m_VertexArrays);
			BufferHandler::BindBuffer(i.m_VertexBuffer);

			if (i.m_Material != 0)
				MaterialHandler::GetMaterial(i.m_Material).BindBaseColor();

			if (BufferHandler::GetBuffer(i.m_ElementBuffer).GetBufferSize() != 0)
			{
				BufferHandler::BindBuffer(i.m_ElementBuffer);
				GL_CALL(glDrawElements(i.m_RenderingMode, BufferHandler::GetBuffer(i.m_ElementBuffer).GetCount(), BufferHandler::GetBuffer(i.m_ElementBuffer).GetComponentType(), 0));
				BufferHandler::UnbindBuffer(i.m_ElementBuffer);
			}

			if (i.m_Material != 0)
				MaterialHandler::GetMaterial(i.m_Material).UnbindBaseColor();
			BufferHandler::UnbindBuffer(i.m_VertexBuffer);
			BufferHandler::UnbindArray(i.m_VertexArrays);


		}
		pShader.Unbind();
	}


};