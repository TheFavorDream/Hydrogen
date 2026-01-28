#pragma once

#include "Common.h"
#include "Model/Model.h"
#include "Definitions.h"
#include "3rdParty/Parser/Json.h"
#include <fstream>
#include <vector>
#include <stack>

using json = nlohmann::json;

namespace Hydrogen
{



	enum FileFormat {INVALID_FILE=-1, GLTF, GLB };
	class GLTFLoader
	{
	public:

		HYD static Model* Load(const std::string& pPath);

		HYD static Model* LoadGLTF(const std::string& pPath);
		HYD static Model* LoadGLB(const std::string& pPath);

		HYD static uint32 Free(Model** pModel);

		HYD static inline uint32 GetGLTFVersion() { return s_Version; }

	private:

		//Retrives the File format of the gltf file (.gltf or glb)
		HYD static FileFormat GetFileFormat(const std::string& pFilePath);

		//Loads the external buffers to memory
		HYD static uint32 SetupBuffers(json& pBuffers, std::vector<std::string>& pBufferData);

		//Gets buffers as input and divides them into bufferviews
		HYD static uint32 SetupBufferViews(json& pBufferViews, const std::vector<std::string>& pBufferData, std::vector<BufferView>& pBufferViewData);
		
		//Gets bufferviews as input and divides them into accessors
		HYD static uint32 SetupAccessors(json& pAccessors, const std::vector<BufferView>& pBufferViewData, std::vector<Accessor>& pAccessorData);
	
		//loads the meshes into the model file
		HYD static uint32 SetupMeshes(json& pMeshes, const std::vector<Accessor>& pAccessorData, Model* pCurrentModel);
		HYD static uint32 SetupPrimitives(json& pPrimitives, const std::vector<Accessor>& pAccessorData, Model* pCurrentModel, Mesh* pCurrentMesh);
	
		HYD static uint32 ProcessAttributes(json& pAttributes, const std::vector<Accessor>& pAccessorData, Primitive* pCurrentPrimitive, Model* pCurrentModel);
	
		HYD static uint32 ProcessNodes(json& pNodes, std::vector<Node>& nodes);
	
		HYD static uint32 ProcessScene(json& pScene, const std::vector<Node>& nodes, Model* pCurrentModel);

	private:
		static std::string s_RootPath; // the path where .gltf or .glb lives. needed to load external resources
		static uint32 s_Version;
	};


};