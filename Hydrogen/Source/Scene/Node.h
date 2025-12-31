#pragma once

#include "../Common.h"
#include "../Math/Matrix/Matrix.h"
#include "../Math/Vector/VectorDef.h"
#include "../Shader/Shader.h"
#include "../Geometry/Mesh.h"
#include <vector>

namespace Hydrogen
{

	class Node
	{
	public:

		 Node();
		 Node(std::string pName, Mesh* pMeshRef);
		~Node();

		void RenderMesh(Shader& pShader, Model* pModel);

		const Matrix& GetModelMatrix();
		inline std::vector<Node*>& GetChildren() { return m_Children; };

	private:
		std::string m_Name;
		Mesh* m_MeshRef;
		std::vector<Node*>  m_Children;

		//Transformation Properties:
		Vec3 m_Translation;
		Vec4 m_Rotation;
		Vec3 m_Scale;

		friend class Model;
	};

};