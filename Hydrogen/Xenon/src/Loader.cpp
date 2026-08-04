#include "../include/Loader.h"

namespace Xenon
{
	/*

		Loader Implementation


	*/


	//static variable definitions:
	XE_VECTOR<Loader::GLTFNode>	    Loader::s_Nodes;
	XE_VECTOR<BufferView>			Loader::s_BufferViews;
	XE_VECTOR<Accessor>				Loader::s_Accessors;
	XE_VECTOR<Mesh>					Loader::s_Meshes;
	XE_VECTOR<ImageInfo>            Loader::s_ImagesInfo;
	XE_VECTOR<Sampler>              Loader::s_Samplers;
	XE_VECTOR<Loader::TextureInfo>  Loader::s_TexturesInfo;
	XE_VECTOR<Material>				Loader::s_Materials;
	XE_MAP<uint64_t, Texture>	    Loader::s_TextureLookUp;
	XE_VECTOR<Camera>				Loader::s_Cameras;

	uint64_t Loader::s_SceneCount = 0;
	uint64_t Loader::s_NodeCount = 0;
	uint64_t Loader::s_MeshCount  = 0;

	FILE_FORMAT Loader::s_CurrentFormat;
	BinaryData	Loader::s_GlbBinary;
	XE_STRING   Loader::s_RootPath;
	Model*      Loader::s_CurrentModel = nullptr;
	LoadFlags_  Loader::s_MaterialFlag;
	LoadFlags_  Loader::s_TransformFlag;


	Model Loader::Load(const XE_STRING&  pPath, uint32_t pFlags)
	{

		Loader::ExtractFilePath(pPath);
		Loader::RetriveFileFormat(pPath);

		s_MaterialFlag = LoadFlags_(pFlags % 10);
		pFlags -= (pFlags % 10);
		s_TransformFlag = LoadFlags_(pFlags);
		

		BinaryData data;
		data.Ptr = (uint8_t*)XE_STREAM_READ(pPath, data.ByteLength);

		Model model;

		//Global Pointer to model within loader
		s_CurrentModel = &model;


		switch (s_CurrentFormat)
		{
		case FORMAT_GLTF:
			 LoadJson(data);
			 break;
		case FORMAT_GLB:
			 LoadGLB(data);
			 break;
		case FORMAT_INVALID:
			return Model();
		}


		return std::move(model);
	}



	uint32_t Loader::LoadJson(BinaryData& pData)
	{
		auto Json = simdjson::padded_string((char*)pData.Ptr, pData.ByteLength);
		simdjson::ondemand::document Gltf = simdjson::ondemand::parser::get_parser().iterate(Json);


		pData.Free();

		s_CurrentModel->m_DefaultScene = XE_NO_DEFAULT_SCENE;

		//Retriving Asset
		LOG(LOG_INFO, "Loading Asset Info");

		if (Gltf["asset"]["generator"].has_value())
			s_CurrentModel->m_Generator = std::string_view(Gltf["asset"]["generator"]);

		if (Gltf["asset"]["copyright"].has_value())
			s_CurrentModel->m_CopyRight = std::string_view(Gltf["asset"]["copyright"]);

		if (Gltf["asset"]["version"].has_value())
			s_CurrentModel->m_Version = std::string_view(Gltf["asset"]["version"]);

		if (Gltf["scene"].has_value())
			s_CurrentModel->m_DefaultScene = int32_t(Gltf["scene"]);



		uint32_t ErrorCode = 0; //No Error

		/*
			Buffers
		*/
		LOG(LOG_INFO, "Loading External Buffers");
		if (ErrorCode = LoadBuffersFromJson(Gltf))
		{
			LOG(LOG_ERROR, "Could not be able to retrive buffers. ErrorCode:%i", ErrorCode);
			return ErrorCode;
		}


		/*
			Buffer Views:

		*/

		LOG(LOG_INFO, "Configuring BufferViews");
		if (ErrorCode = LoadBufViewFromJson(Gltf))
		{
			LOG(LOG_ERROR, "Failed to configure BufferViews. ErrorCode:%i", ErrorCode);
			return ErrorCode;
		}

		if (s_MaterialFlag != LF_NO_MATERIAL)
			LoadMaterials(Gltf);


		/*
			Accessors:
		*/

		LOG(LOG_INFO, "Loading Accessors");
		if (ErrorCode = LoadAccessorsFromJson(Gltf))
		{
			LOG(LOG_ERROR, "Falied to load accessors. ErrorCode:%i", ErrorCode);
			return ErrorCode;
		}

		Loader::s_BufferViews.clear();


		/*
			Meshes:
		*/

		LOG(LOG_INFO, "Loading Meshes");
		if (ErrorCode = LoadMeshesFromJson(Gltf))
		{
			LOG(LOG_ERROR, "Couldn't Retrive Meshes. ErrorCode:%i", ErrorCode);
			return ErrorCode;
		}
		Loader::s_Accessors.clear();



		/*
			Cameras:
		*/
		LOG(LOG_INFO, "Seting up Cameras");
		if (ErrorCode = LoadCamerasFromJson(Gltf))
		{
			LOG(LOG_ERROR, "Couln't Retrive Cameras");
			return ErrorCode;
		}

		/*

			node Loading:

		*/
		LOG(LOG_INFO, "Seting Up Nodes");
		if (ErrorCode = LoadNodesFromJson(Gltf))
		{
			LOG(LOG_ERROR, "Couln't Setup nodes. ErrorCode:%i", ErrorCode);
			return ErrorCode;
		}


		/*

			Scene Loading:

		*/
		LOG(LOG_INFO, "Constructing the Scene");
		if (ErrorCode = LoadSceneFromJson(Gltf))
		{
			LOG(LOG_ERROR, "Failed to construct scene. ErrorCode:%i", ErrorCode);
			return ErrorCode;
		}

		LOG(LOG_INFO, "Model Loading Done. Scenes:%i\tNodes:%i\tMeshes:%i",s_SceneCount, s_NodeCount, s_MeshCount);

		Loader::s_Meshes.clear();
		Loader::s_Nodes.clear();



		return 0;
	}

	uint32_t Loader::LoadGLB(BinaryData& pData)
	{

		//12 byte Header:
		BinaryData Header = pData.SubData(0, 12);
		
		uint32_t Magic   = 0;
		uint32_t Version = 0;
		uint32_t Length  = 0;

		memcpy((void*)&Magic,   (void*)Header[0], 4);
		memcpy((void*)&Version, (void*)Header[4], 4);
		memcpy((void*)&Length,  (void*)Header[8], 4);

		Header.Free();

		if (Magic != 0x46546C67)
			LOG(LOG_WARNING, "GLB Magic mismatch: invalid magic number. expected:%i, got:%i", 0x46546C67, Magic);
	
		//Json chunk:

		uint32_t Type   = 0;
		uint32_t JsonLength = 0;

		memcpy((void*)&JsonLength, (void*)pData[12], 4);
		memcpy((void*)&Type, (void*)pData[16], 4);

		if (Type != 0x4E4F534A)
		{
			LOG(LOG_ERROR, "GLB: Expected a Json Chunk");
			return 1;
		}

		BinaryData Json(JsonLength, pData[20]);


		//Binary Chunk:
		
		Type = 0;
		uint32_t BinaryLength = 0;

		memcpy((void*)&BinaryLength, (void*)pData[20 + JsonLength], 4);
		memcpy((void*)&Type, (void*)pData[24 + JsonLength], 4);

		if (Type != 0x004E4942)
		{
			LOG(LOG_ERROR, "GLB: Expected a Binary Chunk");
			return 1;
		}
		
		s_GlbBinary.Copy(pData, (28+JsonLength), BinaryLength);

		return LoadJson(Json);
	}





	XE_STRING Loader::RetriveExtension(const XE_STRING& pPath)
	{
		/*
		Because it's not guaranteed that User Defined string has Substr() and find_last_of() functions,
		We have to Retrive the extension manaully
		*/
		uint64_t Index = pPath.size() - 1;

		for (; Index >= 0; Index--)
		{
			if (pPath[Index] == '.')
				break;
		}

		XE_STRING Extension;
		for (size_t i = Index+1; i < pPath.size(); i++)
		{
			Extension = Extension + pPath[i];
		}
		return Extension;
	}

	void Loader::RetriveFileFormat(const XE_STRING& pPath)
	{


		XE_STRING Extension = RetriveExtension(pPath);


		if (Extension == "gltf")
		{
			s_CurrentFormat = FORMAT_GLTF;
			return;
		}

		else if (Extension == "glb")
		{
			s_CurrentFormat = FORMAT_GLB;
			return;
		}

		s_CurrentFormat = FORMAT_INVALID;
	}


	void Loader::ExtractFilePath(const XE_STRING& pPath)
	{
		//just have to remove the file name from the rest

		for (size_t i = 0, j = pPath.size() - 1; i <= j; i++)
		{
			if (pPath[j] != '\\' && pPath[j] != '/')
				j--;

			s_RootPath += pPath[i];
		}
		return;
	}


	MimeType Loader::RetriveMimeType(const XE_STRING& pType)
	{
		if (pType == "image/png")
			return XE_IMG_PNG;
		else if (pType == "image/jpeg")
			return XE_IMG_JPG;
		else if (pType == "image/gif")
			return XE_IMG_GIF;
		else if (pType == "image/bmp")
			return XE_IMG_BMP;
		return XE_IMG_UNKNOWN;
	}


	uint32_t Loader::LoadSceneFromJson(simdjson::ondemand::document_reference pGLTF)
	{

		if (!pGLTF["scenes"].has_value())
			return 1;

		//Scene Loading:
		for (auto scene : pGLTF["scenes"])
		{
			std::shared_ptr<Scene> NewScene = std::make_shared<Scene>();

			NewScene->m_Name = (scene["name"].has_value()) ? std::string_view(scene["name"]) : "Unnamed Scene";

			//Tree Construction:

			std::stack<GLTFNode> Stack;


			for (uint64_t NodeIndex : scene["nodes"])
			{
				GLTFNode& Current = Loader::s_Nodes[NodeIndex];
				Current.PointerToParent = &NewScene->m_Nodes;
				Current.ParentDepth = 0;
				Stack.push(std::move(Current));
			}

			XE_VECTOR<Node>* CurrentNodes = nullptr;

			std::set<uint64_t> MeshIndices;


			while (!Stack.empty())
			{
				GLTFNode Current = std::move(Stack.top());
				Stack.pop();

				CurrentNodes = Current.PointerToParent;

				uint64_t Depth = Current.ParentDepth + 1;

				Current.MainNode.m_Depth         = Depth;
				Current.MainNode.m_PointerToRoot = NewScene;


				if (Current.MainNode.m_Mesh >= 0)
				{
					const Mesh& mesh = Loader::s_Meshes[Current.MainNode.m_Mesh];
					
					for (auto& pri : mesh)
					{
						for (auto& texIndex : pri.m_Material)
						{
							if (!texIndex.has_value())
								continue;
							NewScene->m_Textures[texIndex.value()] = Loader::s_TextureLookUp[texIndex.value()];
						}
					}

					MeshIndices.emplace(Current.MainNode.m_Mesh);
				}

				CurrentNodes->push_back(std::move(Current.MainNode));

				for (uint64_t Children : Current.Children)
				{
					GLTFNode& Child = Loader::s_Nodes[Children];
					Child.PointerToParent = &(*CurrentNodes)[CurrentNodes->size()-1].m_Childern;
					Child.ParentDepth = Depth;
					Stack.push(std::move(Child));
				}
			}

			for (auto& index : MeshIndices)
				NewScene->m_Meshes.push_back(Loader::s_Meshes[index]);

			Loader::s_CurrentModel->m_Scenes.push_back(std::move(NewScene));
			Loader::s_SceneCount += 1;
		}
		return 0;
	}

	/*
		Loads the nodes from json file to memory for the purpose of tree construction
	*/
	uint32_t Loader::LoadNodesFromJson(simdjson::ondemand::document_reference pGLTF)
	{
		if (!pGLTF["nodes"].has_value())
			return 1;

		for (auto node: pGLTF["nodes"])
		{

			Loader::GLTFNode NewNode;

			//Child Nodes:
			if (node["children"].has_value())
			{
				for (uint64_t nodeIndex : node["children"])
				{
					NewNode.Children.push_back(nodeIndex);
				}
			}


			//Node Name:
			NewNode.MainNode.m_Name   = (node["name"].has_value()) ? std::string_view(node["name"]) : "UnNamed";

			//Node's Mesh
			NewNode.MainNode.m_Mesh   = (node["mesh"].has_value()) ? int64_t(node["mesh"]) : -1;

			
			//Node's Camera
			int64_t CameraIndex = (node["camera"].has_value()) ? int64_t(node["camera"]) : -1;
			if (CameraIndex != -1)
				NewNode.MainNode.m_Camera = Loader::s_Cameras[CameraIndex];


			Loader::s_NodeCount += 1;
			
			if (node["matrix"].has_value())
			{

				float Values[16];

				size_t index = 0;
				for (double Val : node["matrix"])
				{
					Values[index] = (float)Val;
					index += 1;
				}

				NewNode.MainNode.m_LocalTransform = XE_MAT4(Values);
				Loader::s_Nodes.push_back(std::move(NewNode));

				continue;
			}


			//Node's Translation
			if (node["translation"].has_value())
			{
				float Translate[3] = { 0.0f, 0.0f, 0.0f };
				size_t index = 0;
				for (double Val : node["translation"])
				{
					Translate[index] = (float)Val;
					index += 1;
				}
				NewNode.MainNode.m_Translation = XE_VEC3(Translate);
			}

			//Node's Rotation
			if (node["rotation"].has_value())
			{
				float Rotation[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
				size_t index = 0;
				for (double Val : node["rotation"])
				{
					Rotation[index] = (float)Val;
					index += 1;
				}
				NewNode.MainNode.m_Rotation = XE_VEC4(Rotation);
			}

			//Node's Scale
			if (node["scale"].has_value())
			{
				float Scale[3] = { 1.0f, 1.0f, 1.0f };
				size_t index = 0;
				for (double Val : node["scale"])
				{
					Scale[index] = (float)Val;
					index += 1;
				}
				NewNode.MainNode.m_Scale = XE_VEC3(Scale);
			}
			

			Loader::s_Nodes.push_back(std::move(NewNode));
		}

		return 0;
	}

	uint32_t Loader::LoadCamerasFromJson(simdjson::ondemand::document_reference pGLTF)
	{
		if (!pGLTF["cameras"].has_value())
			return XE_OK;

		for (auto camera : pGLTF["cameras"])
		{
			Camera NewCamera;


			XE_STRING TypeStr;
			camera["type"].get_string(TypeStr);


			if (TypeStr == "orthographic")
			{
				simdjson::ondemand::object Ortho = camera["orthographic"];

				NewCamera.Xmag  =  (float)double(Ortho["xmag"]);
				NewCamera.Ymag  =  (float)double(Ortho["ymag"]);
				NewCamera.Zfar  =  (float)double(Ortho["zfar"]);
				NewCamera.Znear = (float)double(Ortho["znear"]);
				NewCamera.Type  = XE_CAMERA_ORTHOGONAL;
			}
			else if (TypeStr == "perspective")
			{
				simdjson::ondemand::object Pers = camera["perspective"];

				NewCamera.AspectRatio = (float)double(Pers["aspectRatio"]);
				NewCamera.Yfov        = (float)double(Pers["yfov"]);
				NewCamera.Zfar        = Pers["zfar"].has_value() ? (float)double(Pers["zfar"]) : INFINITY;
				NewCamera.Znear       = (float)double(Pers["znear"]);
				NewCamera.Type		  = XE_CAMERA_PRESPECTIVE;
			}

			Loader::s_Cameras.push_back(NewCamera);
		}


		return 0;
	}

	uint32_t Loader::LoadBuffersFromJson(simdjson::ondemand::document_reference pGLTF)
	{

		if (!pGLTF["buffers"].has_value())
			return 1;

		for (auto buffer : pGLTF["buffers"])
		{


			if (buffer["uri"].has_value())
			{
				XE_STRING URI;
				buffer["uri"].get_string(URI);

				std::shared_ptr<BinaryData> buffer;
				buffer.reset(new BinaryData());

				buffer->Ptr = (uint8_t*)XE_STREAM_READ(s_RootPath + URI, buffer->ByteLength);

				LOG(LOG_INFO, "Loaded %i bytes of data to memory", buffer->ByteLength);

				if (!buffer->Ptr)
					LOG(LOG_ERROR, "Cannot Load Buffer");
				Loader::s_CurrentModel->m_Buffers.push_back(std::move(buffer));
			}
			else
			{
				//Use GLB data:
				std::shared_ptr<BinaryData> buffer;
				buffer.reset(new BinaryData());
				buffer->Ptr		   = s_GlbBinary.Ptr;
				buffer->ByteLength = s_GlbBinary.ByteLength;

				s_GlbBinary.Ptr        = nullptr;
				s_GlbBinary.ByteLength = 0;

				Loader::s_CurrentModel->m_Buffers.push_back(std::move(buffer));
				
			}

			s_CurrentModel->m_BuffersCount += 1;
		}
		return 0;
	}

	uint32_t Loader::LoadBufViewFromJson(simdjson::ondemand::document_reference pGLTF)
	{

		if (!pGLTF["bufferViews"].has_value())
			return 1;

		for (auto bufferview : pGLTF["bufferViews"])
		{
			BufferView NewBV;

			uint64_t BufferIndex  = bufferview["buffer"].has_value()? size_t(bufferview["buffer"]) : 0;

			NewBV.Buffer = Loader::s_CurrentModel->m_Buffers.at(BufferIndex);

			NewBV.ByteLength = uint64_t(bufferview["byteLength"]);
			NewBV.ByteOffset = bufferview["byteOffset"].has_value() ? uint64_t(bufferview["byteOffset"]) : 0;
			NewBV.ByteStride = bufferview["byteStride"].has_value() ? uint64_t(bufferview["byteStride"]) : 0;

			Loader::s_BufferViews.push_back(NewBV);

		}

		return 0;
	}

	uint32_t Loader::LoadAccessorsFromJson(simdjson::ondemand::document_reference pGLTF)
	{

		if (!pGLTF["accessors"].has_value())
			return 1;

		for (auto accessor : pGLTF["accessors"])
		{
			XE_ASSERT("Currently Xenon does Not support sparse accessors", !accessor["bufferView"].has_value());

			Accessor NewAccessor;

			BufferView& BVRef = Loader::s_BufferViews[uint64_t(accessor["bufferView"])];
			BVRef.Target = XE_BIN_GEOMETRY;

			NewAccessor.Data		  = BVRef;
			NewAccessor.Count         = (uint64_t)accessor["count"];
			NewAccessor.ComponentType = (DataType)(uint64_t)accessor["componentType"];
			NewAccessor.Type		  = Accessor::RetriveArrayTypeFromString(accessor["type"]);
			NewAccessor.Normalized    = (accessor["normalized"].has_value()) ? bool(accessor["normalized"]) : false;
			NewAccessor.Offset        = (accessor["byteOffset"].has_value()) ? uint64_t(accessor["byteOffset"]) : 0;

			Loader::s_Accessors.push_back(NewAccessor);

		}

		return 0;
	}



	uint32_t Loader::LoadMeshesFromJson(simdjson::ondemand::document_reference pGLTF)
	{

		if (!pGLTF["meshes"].has_value())
			return 1;

		for (auto mesh : pGLTF["meshes"])
		{

			Mesh NewMesh;

			//Read Primitve indices
			LoadPrimitivesFromJson(mesh["primitives"], NewMesh.m_Primitives);

			//Read Name of the Mesh:
			if (mesh["name"].has_value())
				mesh["name"].get_string(NewMesh.m_Name);


			Loader::s_Meshes.push_back(std::move(NewMesh));
			Loader::s_MeshCount += 1;
		}
		return 0;
	}

	uint32_t Loader::LoadPrimitivesFromJson(simdjson::ondemand::array pPrimitives, XE_VECTOR<Primitive>& pGroup)
	{

		for (auto primitive : pPrimitives)
		{
			Primitive NewPrimtive;

			if (primitive["indices"].has_value())
			{
				Accessor& access	  = Loader::s_Accessors[size_t(primitive["indices"])];
				access.Kind			  = XE_INDEX;
				NewPrimtive.m_Indices = access;
			}

			if (primitive["mode"].has_value())
				NewPrimtive.m_RenderMode = int32_t(primitive["mode"]);

			if (primitive["material"].has_value() && s_Materials.size() > 0)
				NewPrimtive.m_Material = Loader::s_Materials[size_t(primitive["material"])];

			simdjson::ondemand::object Attribute = primitive["attributes"];

			if (Attribute["POSITION"].has_value())
			{
				Accessor& Access = Loader::s_Accessors[size_t(Attribute["POSITION"])];
				Access.Kind = XE_POSITION;
				NewPrimtive.m_Positions = Access;
			}
			if (Attribute["NORMAL"].has_value())
			{
				Accessor& Access      = Loader::s_Accessors[size_t(Attribute["NORMAL"])];
				Access.Kind           = XE_NORMALS;
				NewPrimtive.m_Normals = Access;
			}
			if (Attribute["TANGENT"].has_value())
			{
				Accessor& Access		= Loader::s_Accessors[size_t(Attribute["TANGENT"])];
				Access.Kind				= XE_TANGENT;
				NewPrimtive.m_Tangents  = Access;
			}

			if (Attribute["TEXCOORD_0"].has_value())
			{
				Accessor& Access		  = Loader::s_Accessors[size_t(Attribute["TEXCOORD_0"])];
				Access.Kind				  = XE_TEXCOORDS_0;
				NewPrimtive.m_TexCoord_0  = Access;
			}

			if (Attribute["TEXCOORD_1"].has_value())
			{
				Accessor& Access		 = Loader::s_Accessors[size_t(Attribute["TEXCOORD_1"])];
				Access.Kind				 = XE_TEXCOORDS_1;
				NewPrimtive.m_TexCoord_1 = Access;
			}

			if (Attribute["COLOR_0"].has_value())
			{
				Accessor& Access		 = Loader::s_Accessors[size_t(Attribute["COLOR_0"])];
				Access.Kind			     = XE_COLOR_0;
				NewPrimtive.m_Color_0    = Access;
			}

			pGroup.push_back(std::move(NewPrimtive));
		}

		return 0;
	}



	/*
		Material Loading:
	*/

	
	AlphaMode Loader::RetriveAlphaModeFromString(const std::string_view& pOther)
	{

		if (pOther == "MASK")
			return XE_MASK;
		else if (pOther == "BLEND")
			return XE_BLEND;
		return XE_OPAQUE;
	}

	uint32_t Loader::LoadMaterials(simdjson::ondemand::document_reference pGLTF)
	{
		if (!pGLTF["materials"].has_value())
			return 1;

		if (LoadImageRefsFromJson(pGLTF))
		{
			return 1;
		}

		LoadSamplersFromJson(pGLTF);

		if (LoadTexturesFromJson(pGLTF))
		{
			return 1;
		}


		s_Samplers.clear();
		s_ImagesInfo.clear();



		for (auto material : pGLTF["materials"])
		{
			Material NewMaterial;

			NewMaterial.m_Name		  = (material["name"].has_value())               ? std::string_view(material["name"])				 : "Unnamed Material";
			NewMaterial.m_Alpha       = (material["alphaMode"].has_value())          ? RetriveAlphaModeFromString(material["alphaMode"]) : XE_OPAQUE;
			NewMaterial.m_AlphaCutoff = (float)((material["alphaCutoff"].has_value()) ? double(material["alphaCutoff"])				     : 0.5f);
			NewMaterial.m_DoubleSided = (material["doubleSided"].has_value())		 ? bool(material["doubleSided"])					 : false;

			if (material["emissiveFactor"].has_value())
			{
				float EmissiveFactor[3];
				uint16_t Index = 0;
				for (double Val : material["emissiveFactor"])
				{
					EmissiveFactor[Index] = float(Val);
					Index++;
				}
				NewMaterial.m_EmissiveFactor = XE_VEC3(EmissiveFactor);
			}


			//Metallic-Roughness part of material
			if (material["pbrMetallicRoughness"].has_value())
			{
				SetMetallicRoughness(material["pbrMetallicRoughness"], NewMaterial);
			}

			//Normal part of material
			if (material["normalTexture"].has_value() && s_MaterialFlag != LF_BASE_COLOR_ONLY)
			{
				NewMaterial.m_Normal = LoadTextureIfHasnt(material["normalTexture"]);
			}
		

			//Occlusion part of material
		
			if (material["occlusionTexture"].has_value() && s_MaterialFlag == LF_DEFAULT_MATERIAL)
			{
				NewMaterial.m_Occlusion         = LoadTextureIfHasnt(material["occlusionTexture"]);
				NewMaterial.m_OcclusionStrength = float((material["occlusionTexture"]["strength"].has_value()) ? double(material["occlusionTexture"]["strength"]) : 1.0f);
			}
		
			//Emissive part:
		
			if (material["emissiveTexture"].has_value() && s_MaterialFlag == LF_DEFAULT_MATERIAL)
			{
				NewMaterial.m_Emissive = LoadTextureIfHasnt(material["emissiveTexture"]);
			}


			Loader::s_Materials.push_back(NewMaterial);
		}

		return 0;
	}



	uint32_t Loader::SetMetallicRoughness(simdjson::ondemand::object pMetallicRoughness, Material& pMaterial)
	{
		pMaterial.m_MetallicFactor  = (float)(pMetallicRoughness["metallicFactor"].has_value()  ?  double(pMetallicRoughness["metallicFactor"]) : 1.0f);
		pMaterial.m_RoughnessFactor = (float)(pMetallicRoughness["roughnessFactor"].has_value() ? double(pMetallicRoughness["roughnessFactor"]) : 1.0f);

		if (pMetallicRoughness["baseColorFactor"].has_value())
		{
			float BaseColorFactor[4];
			uint16_t Index = 0;
			for (double Val : pMetallicRoughness["baseColorFactor"])
			{
				BaseColorFactor[Index] = float(Val);
				Index++;
			}
			pMaterial.m_BaseColorFactor = XE_VEC4(BaseColorFactor);
		}


		if (pMetallicRoughness["baseColorTexture"].has_value())
		{
			pMaterial.m_BaseColorTexture = LoadTextureIfHasnt(pMetallicRoughness["baseColorTexture"]);

		}

		if (pMetallicRoughness["metallicRoughnessTexture"].has_value() && s_MaterialFlag == LF_DEFAULT_MATERIAL)
		{
			pMaterial.m_MetallicRoughnessTexture = LoadTextureIfHasnt(pMetallicRoughness["metallicRoughnessTexture"]);
		}

		return 0;
	}




	uint64_t Loader::LoadTextureIfHasnt(simdjson::ondemand::object pTextureInfo)
	{

		uint64_t TextureIndex = pTextureInfo["index"];

		if (Loader::s_TextureLookUp.find(TextureIndex) != Loader::s_TextureLookUp.end())
		{
			return TextureIndex;
		}

		//Load Texture:
		const Loader::TextureInfo& textureInfo = Loader::s_TexturesInfo[TextureIndex];

		Texture NewTexture;
		
		//Image Data
		if (textureInfo.Image.ImageBV.Target == XE_BIN_IMAGE)
		{
			NewTexture.ImageData = textureInfo.Image.ImageBV;
		}

		else if (textureInfo.Image.URI.length() > 0)
		{
			BinaryData* Image = new BinaryData();
			Image->Ptr = (uint8_t*)XE_STREAM_READ((s_RootPath + textureInfo.Image.URI), Image->ByteLength);
			
			BufferView View;
			View.ByteLength = Image->ByteLength;
			View.Target		= XE_BIN_IMAGE;

			Loader::s_CurrentModel->m_Buffers.push_back(std::move(std::shared_ptr<BinaryData>(Image)));
			s_CurrentModel->m_BuffersCount += 1;

			View.Buffer			 = Loader::s_CurrentModel->m_Buffers.at(Loader::s_CurrentModel->m_Buffers.size()-1);
			NewTexture.ImageData = View;
		}

		NewTexture.TextureSample = textureInfo.TextureSample;
		NewTexture.TexCoordSet   = uint16_t((pTextureInfo["texCoord"].has_value())? uint64_t(pTextureInfo["texCoord"]) : 0);
		NewTexture.ImageMime     = textureInfo.Image.ImageMime;

		Loader::s_TextureLookUp[TextureIndex] = NewTexture;

		return TextureIndex;
	}

	uint32_t Loader::LoadImageRefsFromJson(simdjson::ondemand::document_reference pGLTF)
	{
		if (!pGLTF["images"].has_value())
			return 1;

		for (auto image : pGLTF["images"])
		{
			ImageInfo NewImage;

		

			if (image["bufferView"].has_value())
			{
				BufferView& BVRef   = Loader::s_BufferViews[(size_t)image["bufferView"]];
				BVRef.Target        = XE_BIN_IMAGE;
				NewImage.ImageBV    = BVRef;
				NewImage.ImageMime   = RetriveMimeType(std::string(std::string_view(image["mimeType"])));
			}
			else
			{
				std::string_view Path;
				image["uri"].get_string(Path);
				NewImage.ImageMime = RetriveMimeType("image/" + RetriveExtension(std::string(Path)));
				NewImage.URI = Path;
			}

			Loader::s_ImagesInfo.push_back(NewImage);
		}

		return 0;
	}

	uint32_t Loader::LoadSamplersFromJson(simdjson::ondemand::document_reference pGLTF)
	{
		if (!pGLTF["samplers"].has_value())
			return 1;

		for (auto sampler : pGLTF["samplers"])
		{
			Sampler NewSampler;


			NewSampler.Mag      = TextureFilter((sampler["magFilter"].has_value()) ? uint32_t(sampler["magFilter"]) : LINEAR);
			NewSampler.Min      = TextureFilter((sampler["minFilter"].has_value()) ? uint32_t(sampler["minFilter"]) : LINEAR);

			NewSampler.WrapS    = TextureWrap((sampler["wrapS"].has_value()) ? uint32_t(sampler["wrapS"]) : REPEAT);
			NewSampler.WrapT    = TextureWrap((sampler["wrapT"].has_value()) ? uint32_t(sampler["wrapT"]) : REPEAT);

			Loader::s_Samplers.push_back(NewSampler);
		}

		return 0;
	}

	uint32_t Loader::LoadTexturesFromJson(simdjson::ondemand::document_reference pGLTF)
	{
		if (!pGLTF["textures"].has_value())
			return 1;

		for (auto texture : pGLTF["textures"])
		{
			Loader::TextureInfo NewTexture;

			if (Loader::s_Samplers.size() != 0)
				NewTexture.TextureSample = Loader::s_Samplers[(uint64_t)texture["sampler"]];

			if (Loader::s_ImagesInfo.size() != 0)
				NewTexture.Image   = Loader::s_ImagesInfo[(uint64_t)texture["source"]];

			Loader::s_TexturesInfo.push_back(NewTexture);
		}

		return 0;
	}
};