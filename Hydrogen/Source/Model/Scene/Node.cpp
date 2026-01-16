#include "Node.h"

namespace Hydrogen
{
	Node::Node()
	{
	}

	Node::Node(std::string pName, Mesh* pMeshRef):
		m_MeshRef(pMeshRef), m_Name(pName)
	{
	}

	Node::~Node()
	{
	}

	void Node::RenderMesh(Shader & pShader, Model* pModel, Matrix* pSubMatrix)
	{

		Matrix Transform = ((pSubMatrix != nullptr) ?  m_LocalTransformation * (*pSubMatrix)  : m_LocalTransformation);
		//Transform.PrintMatrix();
		//std::cout << '\n';


		if (m_MeshRef != nullptr)
		{
			pShader.Bind();
			pShader.SetUniformMat4("Model", Transform.GetPtr());
			pShader.SetUniformInt1("TEX", 0);
			m_MeshRef->Render(pModel);
		}
		for (auto& i : m_Children)
		{
			pModel->m_Nodes[i].RenderMesh(pShader, pModel, &Transform);
		}
	}
};