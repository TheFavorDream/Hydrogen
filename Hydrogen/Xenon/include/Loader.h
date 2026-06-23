#pragma once

#include "Xenon.h"
#include "../3rdParty/SimdJson/simdjson.h"

namespace Xenon
{

	class Loader
	{
	public:


		static Model Load(const XE_STRING&  pPath, uint32_t pFlags = LF_DEFAULT);


	private:

		static uint32_t LoadJson(BinaryData& pJson);
		static uint32_t LoadGLB(BinaryData& pData);

		static XE_STRING RetriveExtension(const XE_STRING&  pPath);
		static void		 RetriveFileFormat(const XE_STRING& pPath);
		static void		 ExtractFilePath(const XE_STRING&   pPath);
		static MimeType  RetriveMimeType(const XE_STRING& pType);

		static uint32_t    LoadSceneFromJson(simdjson::ondemand::document_reference pGLTF);
		static uint32_t    LoadNodesFromJson(simdjson::ondemand::document_reference pGLTF);
		static uint32_t    LoadCamerasFromJson(simdjson::ondemand::document_reference pGLTF);
		static uint32_t    LoadBuffersFromJson(simdjson::ondemand::document_reference pGLTF);
		static uint32_t    LoadBufViewFromJson(simdjson::ondemand::document_reference pGLTF);
		static uint32_t    LoadAccessorsFromJson(simdjson::ondemand::document_reference pGLTF);
		static uint32_t    LoadMeshesFromJson(simdjson::ondemand::document_reference pGLTF);
		static uint32_t    LoadPrimitivesFromJson(simdjson::ondemand::array pPrimitives, XE_VECTOR<Primitive>& pGroup);


		static AlphaMode   RetriveAlphaModeFromString(const std::string_view& pOther);

		static uint32_t    LoadMaterials(simdjson::ondemand::document_reference pGLTF);

		static uint32_t    SetMetallicRoughness(simdjson::ondemand::object pMetallicRoughness, Material& pMaterial);

		static Texture	   LookUpTexture(simdjson::ondemand::object pTextureInfo);

		static uint32_t    LoadImageRefsFromJson(simdjson::ondemand::document_reference pGLTF);
		static uint32_t    LoadSamplersFromJson(simdjson::ondemand::document_reference pGLTF);
		static uint32_t    LoadTexturesFromJson(simdjson::ondemand::document_reference pGLTF);



	public:


		struct GLTFNode
		{
			GLTFNode() = default;

			XE_VECTOR<size_t> Children;
			XE_VECTOR<Node>*  PointerToParent = nullptr;
			uint64_t          ParentDepth = 0;
			Node			  MainNode;

		};


		struct TextureInfo
		{
			ImageInfo Image;
			Sampler   Sampler;
		};

		//Temporary Buffers
		static XE_VECTOR<GLTFNode>          s_Nodes;
		static XE_VECTOR<BufferView>	    s_BufferViews;
		static XE_VECTOR<Accessor>          s_Accessors;
		static XE_VECTOR<Mesh>		        s_Meshes;
		static XE_VECTOR<ImageInfo>         s_ImagesInfo;
		static XE_VECTOR<Sampler>           s_Samplers;
		static XE_VECTOR<TextureInfo>       s_TexturesInfo;
		static XE_VECTOR<Material>			s_Materials;
		static XE_VECTOR<Camera>			s_Cameras;

		static XE_MAP<uint64_t, Texture>   s_TextureLookUp;

		static BinaryData  s_GlbBinary;

		static FILE_FORMAT s_CurrentFormat;

		static LoadFlags_  s_MaterialFlag;
		static LoadFlags_  s_TransformFlag;

		static Model*      s_CurrentModel;

		static uint64_t s_SceneCount;
		static uint64_t s_NodeCount;
		static uint64_t s_MeshCount;


		static XE_STRING   s_RootPath;
	};



};