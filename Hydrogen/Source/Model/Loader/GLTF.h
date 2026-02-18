#pragma once

#include "Common.h"
#include "Model/Model.h"
#include "Definitions.h"
#include "3rdParty/Parser/Json.h"
#include <fstream>
#include <vector>
#include <stack>
#include "ResourceHandler/BufferHandler.h"
#include "Model/Material/Texture/Image.h"
#include "Model/Material/Texture/Texture.h"
#include "Model/Material/Material.h"
#include "Timer/Timer.h"
#include "Log/Log.h"

using json = nlohmann::json;

namespace Hydrogen
{



	enum FileFormat {INVALID_FILE=-1, GLTF, GLB };
	enum Load_Flags {
		DEFAULT,
		NO_MATERIAL,
		NO_TRANSFORM
	};

	class GLTFLoader
	{
	public:

		HYD static Model* Load(const std::string& pPath, Load_Flags pFlags=DEFAULT);

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
		HYD static uint32 SetupMeshes(json& pMeshes, const std::vector<Accessor>& pAccessorData, std::vector<Id>& pMaterials, Model* pCurrentModel);
		HYD static uint32 SetupPrimitives(json& pPrimitives, const std::vector<Accessor>& pAccessorData, std::vector<Id>& pMaterials,  Mesh& pCurrentMesh);
		HYD static uint32 ProcessAttributes(json& pAttributes, const std::vector<Accessor>& pAccessorData, Primitive& pCurrentPrimitive);
	
		HYD static uint32 ProcessNodes(json& pNodes, std::vector<GeoNode>& nodes);
		HYD static uint32 ProcessScene(json& pScene, const std::vector<GeoNode>& nodes, Model* pCurrentModel);


		//Material Loading:
		HYD static uint32 LoadMaterials(json& pGLTF, std::vector<Id>& pMaterials, const std::vector<BufferView>& pBufferViews);
		HYD static uint32 LoadImages(json& pImages, const std::string& pRootPath ,std::vector<Image>& pImageData, const std::vector<BufferView>& pBufferViews);
		HYD static uint32 SetupSamplers(json& pSamplers, std::vector<Sampler>& pSamplerData);
		HYD static uint32 SetupTextures(json& pTextures, std::vector<Id>& pTextureIds, const std::vector<Sampler>& pSamplerData, const std::vector<Image>& pImageData);
		HYD static uint32 SetupMaterials(json& pMaterials, std::vector<Id>& pMaterial, std::vector<Id>& pTextureIds);

	private:
		static std::string s_RootPath; // the path where .gltf or .glb lives. needed to load external resources
		static uint32 s_Version;
		static Load_Flags s_Flags;
	};


};