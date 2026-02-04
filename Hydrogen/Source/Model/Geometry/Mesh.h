#pragma once

#include "Common.h"


#include "Math/Matrix/Matrix.h"
#include "Shader/Shader.h"
#include "Model/Model.h"
#include "3rdParty/glm/gtc/type_ptr.hpp"
#include "Primitive.h"

namespace Hydrogen
{



	class Mesh
	{
	public:

		HYD  Mesh() = default;
		HYD  Mesh(std::string pName, std::vector<Primitive>& pPrimitives);
		HYD ~Mesh();


		HYD Mesh(const Mesh& pOther) = delete;
		HYD Mesh(Mesh&& pOther);

		HYD Mesh& operator=(const Mesh& pOther) = delete;
		HYD Mesh& operator=(Mesh&& pOther);

		HYD void Render(const Shader& pShader, glm::mat4* pTransform, Model* pCaller);
		HYD inline const std::string& GetMeshName()	const { return m_Name; }
	

	private:
		std::string m_Name;
		std::vector<Primitive> m_Primitives;
		//Matrix m_Transformation;
		glm::mat4 m_Transformation = glm::mat4(1.0f); //Temp


		friend class GLTFLoader;
		friend class Model;
	};

};