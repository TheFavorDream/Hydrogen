#include "Mesh.h"

namespace Hydrogen
{

	Mesh::Mesh(std::string pName, std::vector<Primitive>& pPrimitives)
	{
		m_Name = pName;
		m_Primitives = std::move(pPrimitives);
	}

	Mesh::~Mesh()
	{
	}

	Mesh::Mesh(Mesh && pOther)
	{
		m_Primitives = std::move(pOther.m_Primitives);
		m_Name = std::move(pOther.m_Name);
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

	void Mesh::Render(const Shader & pShader, glm::mat4 * pTransform, Model* pCaller)
	{

		pShader.Bind();
		pShader.SetUniformMat4("Model", glm::value_ptr(m_Transformation));
		pShader.SetUniformInt1("BaseColor", 0);
		for (auto& i : m_Primitives)
		{
			//Rendering Process:

			i.m_VertexArrays.Bind();
			i.m_VertexBuffer.Bind();
			i.m_Material.BindBaseColor();

			if (i.m_ElementBuffer.GetBufferSize() != 0)
			{
				i.m_ElementBuffer.Bind();
				GL_CALL(glDrawElements(i.m_RenderingMode, i.m_ElementBuffer.GetCount(), i.m_ElementBuffer.GetComponentType(), 0));
				i.m_ElementBuffer.Unbind();
			}

			i.m_Material.UnbindBaseColor();
			i.m_VertexBuffer.Unbind();
			i.m_VertexArrays.Unbind();

		}
		pShader.Unbind();
	}


};