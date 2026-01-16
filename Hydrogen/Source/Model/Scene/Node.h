#pragma once

#include "Common.h"
#include "Math/Matrix/Matrix.h"
#include "Math/Vector/VectorDef.h"
#include "Shader/Shader.h"
#include "Model/Geometry/Mesh.h"
#include <vector>

namespace Hydrogen
{

	class Node
	{
	public:

		 Node();
		 Node(std::string pName, Mesh* pMeshRef);
		~Node();

		void RenderMesh(Shader& pShader, Model* pModel, Matrix* pSubMatrix=nullptr);

		inline Matrix& GetModelMatrix() { return m_LocalTransformation; };
		inline std::vector<uint32>& GetChildren() { return m_Children; };

	private:
		std::string m_Name;
		Mesh* m_MeshRef;
		std::vector<uint32>  m_Children;

		 Matrix m_LocalTransformation;

		//Transformation Properties:
		Vec3 m_Translation = Vec3(0.0f);
		Vec4 m_Rotation;
		Vec3 m_Scale = Vec3(1.0f);

		friend class Model;
	};

};