#pragma once

#include "Common.h"
#include "Platform/OpenGL/VertexArray.h"
#include "Platform/OpenGL/Buffer.h"
#include "Math/Matrix/Matrix.h"
#include "Shader/Shader.h"
#include "Model/Model.h"
#include "3rdParty/glm/gtc/type_ptr.hpp"

namespace Hydrogen
{

	struct Primitive
	{
		int32 VboID = -1;//-1 means no buffer is setted
		int32 EboID = -1;//-1 means no buffer is setted
		int32 VaoID = -1;//-1 means no buffer is setted
		int32 MaterialID;
		int32 RenderingMode = 4;
		Attribute Attributes;
	};

	class Mesh
	{
	public:

		HYD  Mesh() = default;
		HYD  Mesh(std::string pName, std::vector<Primitive>& pPrimitives);
		HYD ~Mesh();


		HYD void Render(const Shader& pShader, glm::mat4* pTransform, Model* pCaller);
		HYD inline const std::string& GetMeshName()	const { return m_Name; }
	

	private:
		std::string m_Name;
		std::vector<Primitive> m_Primitives;
		//Matrix m_Transformation;
		glm::mat4 m_Transformation = glm::mat4(1.0f); //Temp

		friend class GLTFLoader;
	};

};