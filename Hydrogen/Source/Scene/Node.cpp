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

	void Node::RenderMesh(Shader & pShader, Model * pModel)
	{
		pShader.Bind();
		pShader.SetUniformMat4("Model", GetModelMatrix().GetPtr());
		m_MeshRef->Render(pModel);
	}

	const Matrix& Node::GetModelMatrix()
	{
		Matrix Mat(4, 4);

		float* Translation = (float*)&m_Translation;
		float* Scale = (float*)&m_Scale;

		for (int j = 0; j < 3; j++)
		{
			Mat.At(3, j) = Translation[j];	
			Mat.At(j, j) = Scale[j];

		}
		
		return Mat;
	}
};