#pragma once

#include "Common.h"
#include "Model/Geometry/Model.h"
#include "Definitions.h"
#include "Parser/Json.h"
#include "Core/ResourcePool.h"
#include "Render/RenderPool/BufferPool.h"
#include "Render/Material/image.h"
#include "Render/Material/Material.h"
#include "Timer/Timer.h"
#include "Log/Log.h"
#include "Core/Scene.h"

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

		HYD static Model* Load(const std::string& pPath, Scene* pScene, Load_Flags pFlags=DEFAULT);
		HYD static uint32 Free(Model** pModel);
		HYD static inline uint32 GetGLTFVersion() { return s_Version; }

	private:
		HYD static Model* LoadGLTF(const std::string& pPath);
		HYD static Model* LoadGLB(const std::string& pPath);



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
		HYD static uint32 SetupMeshes(json& pMeshes, const std::vector<Accessor>& pAccessorData, std::vector<Wraper<Material>>& pMaterials, std::vector<Id>& pMeshIDs, Model* pCurrentModel);
		HYD static uint32 SetupPrimitives(json& pPrimitives,  const std::vector<Accessor>& pAccessorData, std::vector<Wraper<Material>>& pMaterials,  Mesh* pCurrentMesh);
		HYD static uint32 ProcessAttributes(json& pAttributes, const std::vector<Accessor>& pAccessorData, Primitive& pCurrentPrimitive);
	
		HYD static uint32 ProcessNodes(json& pNodes, std::vector<GeoNode>& nodes);
		HYD static uint32 ProcessScene(json& pScene, const std::vector<GeoNode>& nodes, const std::vector<Id>& pMeshIDs, Model* pCurrentModel);


		//Material Loading:
		HYD static uint32 LoadMaterials(json& pGLTF, std::vector<Wraper<Material>>& pMaterials, const std::vector<BufferView>& pBufferViews);
		HYD static uint32 LoadImages(json& pImages, const std::string& pRootPath ,std::vector<Image>& pImageData, const std::vector<BufferView>& pBufferViews);
		HYD static uint32 SetupSamplers(json& pSamplers, std::vector<Sampler>& pSamplerData);
		HYD static uint32 SetupTextures(json& pTextures, std::vector<Wraper<TextureBase>>& pTextureData, const std::vector<Sampler>& pSamplerData, const std::vector<Image>& pImageData);
		HYD static uint32 SetupMaterials(json& pMaterials, std::vector<Wraper<Material>>& pMaterial, std::vector<Wraper<TextureBase>>& pTextures);

	private:
		static std::string s_RootPath; // the path where .gltf or .glb lives. needed to load external resources
		static uint32 s_Version;
		static Load_Flags s_Flags;
		static Scene* s_CurrentScene;
	};


};