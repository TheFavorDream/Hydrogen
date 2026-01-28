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


	void Mesh::Render(const Shader & pShader, glm::mat4 * pTransform, Model* pCaller)
	{
		glm::mat4 Transformation = m_Transformation;
		if (pTransform != nullptr)
			Transformation *= (*pTransform);

		pShader.Bind();
		pShader.SetUniformMat4("Model", glm::value_ptr(Transformation));
		for (auto& i : m_Primitives)
		{
			//Rendering Process:

			pCaller->m_Arrays[i.VaoID].Bind();
			pCaller->m_Buffers[i.VboID].Bind();
			pCaller->m_Buffers[i.EboID].Bind();

			GL_CALL(glDrawElements(i.RenderingMode, pCaller->m_Buffers[i.EboID].GetBufferSize(), GL_UNSIGNED_SHORT, 0));

			pCaller->m_Arrays[i.VaoID].Unbind();
			pCaller->m_Buffers[i.VboID].Unbind();
			pCaller->m_Buffers[i.EboID].Unbind();

		}
		pShader.Unbind();
	}


};