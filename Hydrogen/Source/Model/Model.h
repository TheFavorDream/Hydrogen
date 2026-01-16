#pragma once

#include "Common.h"
#include "Definitions.h"
#include "Loader.h"
#include "Parser/Json.h"
#include "Platform/OpenGL/Buffer.h"
#include "Platform/OpenGL/VertexArray.h"
#include "Model/Scene/Node.h"
#include "Model/Scene/Scene.h"
#include "Model/Geometry/Mesh.h"
#include "Model/Material/Texture/Texture.h"
#include "Shader/Shader.h"
#include "Math/Matrix/Matrix.h"


using json = nlohmann::json;

namespace Hydrogen
{
	enum FileExtension {FILE_INVALID, FILE_GLTF, FILE_GLB};
	class Model
	{
	public:


		int LoadModel(std::string& pPath);
		int Free();

		void RenderScene(Shader& pShader,  uint32 pTargetScene=0, Matrix* pModelTransformation = nullptr);
		inline uint32 GetDefaultScene() { return m_DefaultScene; }

	private:

		int LoadURI(json& pBuffer, std::string& pRootPath, std::vector<std::string>& pBuffers);
		int SetupBufferViews(json& pBufferView, std::vector<BufferView>& pBufferViews, std::vector<std::string>& pBuffers);
		int SetupAccessors(json& pAccessor, std::vector<Accessor>& pAccessors, std::vector<BufferView>& pBufferViews);
		int SetupMeshes(json& pMeshes, std::vector<Accessor>& pAccessors);
		int SetupMaterials(json& pMaterial);
		int SetupNodes(json& pNodes);
		int SetupScenes(json pScenes);


		int ProcessPrimitives(json& pPrimitive, std::vector<Accessor>& pAccessors,  std::vector<Primitive>& pPrimitives);
		int ProcessElementBuffer(Accessor& pAccessors, int32& pEboID);
		int ProcessAttributes(json& pAttribute, std::vector<Accessor>& pAccessors, Primitive & pPrimitiveRef);


		int ProcessGLTF(json& pGLTF, FileFormat pFormat, std::string* pGLBbinary=nullptr);

		int ParseGLTF(std::string& pPath);
		int ParseGLB(std::string& pPath);

		FileExtension GetFileExtension(std::string& pPath);

	private:
		json m_GLTF;
		std::string m_RootPath;
		
		std::vector<Buffer>		 m_Buffers;
		std::vector<VertexArray> m_VertexArrays;
		std::vector<Mesh>		 m_Meshes;
		std::vector<Node>        m_Nodes;
		std::vector<Scene>       m_Scenes;
		std::vector<Texture>     m_Textures;//This is temporary

		int32 m_DefaultScene = -1;

		friend class Mesh;
		friend class Scene;
		friend class Node;
	};
	typedef Model* ModelRef;
};

