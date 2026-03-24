#include "GLTF.h"
#include "HydPch.h"


namespace Hydrogen
{


	uint32				GLTFLoader::s_Version;
	std::string			GLTFLoader::s_RootPath;
	Load_Flags			GLTFLoader::s_Flags;
	Scene*              GLTFLoader::s_CurrentScene;

	Model* GLTFLoader::Load(const std::string& pPath, Scene* pScene ,Load_Flags pFlags)
	{

		if (pScene == nullptr)
			return nullptr;

		s_CurrentScene  = pScene;
		s_RootPath		= pPath.substr(0, pPath.find_last_of("\\")) + "\\";
		s_Flags			= pFlags;


		switch (GetFileFormat(pPath))
		{
		case GLTF:
			return LoadGLTF(pPath);
		case GLB:
			return LoadGLB(pPath);		
		}

		return nullptr;
	}

	Model* GLTFLoader::LoadGLTF(const std::string& pPath)
	{

		PROFILE_START("Parsing")
		std::ifstream SourceFile(pPath);
		if (SourceFile.fail())
		{
			Log::SetError(Log::FmtStr("Couldn't open model at: %s", pPath.c_str()));
			SourceFile.close();
			return nullptr;
		}

		json GLTF = json::parse(SourceFile);
		SourceFile.close();
		PROFILE_STOP


		uint32 Err = HYD_OK;

		PROFILE_START("Buffer Loading")
		std::vector<std::string> Buffers;
		Err = SetupBuffers(GLTF["buffers"], Buffers);
		if (Err != HYD_OK)
		{
			return nullptr;
		}
		PROFILE_STOP


		PROFILE_START("Buffer Views")
		std::vector<BufferView> BufferViews;
		Err = SetupBufferViews(GLTF["bufferViews"], Buffers, BufferViews);
		if (Err != HYD_OK)
		{
			return nullptr;
		}
		//We don't need Buffers anymore
		Buffers.clear();
		PROFILE_STOP



		//Process Materials:
		PROFILE_START("Material Loading")
		std::vector<Id> Materials;
		if (GLTF.find("materials") != GLTF.end() && s_Flags != NO_MATERIAL)
		{
			LoadMaterials(GLTF, Materials, BufferViews);
		}
		PROFILE_STOP

		PROFILE_START("Accessor Loading")
		std::vector<Accessor> Accessors;
		Err = SetupAccessors(GLTF["accessors"], BufferViews, Accessors);
		if (Err != HYD_OK)
		{
			return nullptr;
		}
		PROFILE_STOP
		//We won't clear bufferViews yet, because it might be needed in Material loading
		//BufferViews.clear();


		Model* model = ResourcePool<Model>::New();


		PROFILE_START("Setup Mesh")
		std::vector<Id> Meshes;
		Err = SetupMeshes(GLTF["meshes"], Accessors, Materials, Meshes, model);
		if (Err != HYD_OK)
		{
			delete model;
			return nullptr;
		}
		Materials.clear();
		PROFILE_STOP

		PROFILE_START("Process Nodes")
		std::vector<GeoNode> nodes;
		Err = ProcessNodes(GLTF["nodes"], nodes);
		if (Err != HYD_OK)
		{
			delete model;
			return nullptr;
		}
		PROFILE_STOP

		
		PROFILE_START("Process Scene")
		Err = ProcessScene(GLTF["scenes"][0], nodes, Meshes, model);
		nodes.clear();
		if (Err != HYD_OK)
		{
			delete model;
			return nullptr;
		}

		PROFILE_STOP


		//We've done using bufferview 
		BufferViews.clear();
		GLTF.clear();
		return model;
	}

	Model* GLTFLoader::LoadGLB(const std::string& pPath)
	{
		return nullptr;
	}

	uint32 GLTFLoader::Free(Model ** pModel)
	{
		uint32 Err = (*pModel)->DestroyModel();
		delete *pModel;
		*pModel = nullptr;

		return Err;
	}

	FileFormat GLTFLoader::GetFileFormat(const std::string & pFilePath)
	{
		std::string FileExtention = pFilePath.substr(pFilePath.find_last_of("."));

		if (FileExtention == ".gltf" || FileExtention == ".GLTF")
			return GLTF;
		if (FileExtention == ".glb" || FileExtention == ".GLB")
			return GLB;

		return INVALID_FILE;
	}


	uint32 GLTFLoader::SetupBuffers(json & pBuffers, std::vector<std::string>& pBufferData)
	{
		if (pBuffers == nullptr)
			return HYD_CORRUPTED_GLTF;

		try
		{
			for (auto &i : pBuffers)
			{
				pBufferData.emplace_back(); //Push an empty string
				std::string* Data = &pBufferData.back();

				std::string Path = s_RootPath + std::string(i["uri"]);
				std::fstream Source(Path, std::ios::binary || std::ios::in);
				if (!Source.is_open())
				{
					Log::SetError("Failed to load external URI", HYD_INVALID_PATH);
					Source.close();
					pBufferData.clear();
					return HYD_INVALID_PATH;
				}

				//Retrive the File size;
				uint32 FileSize = 0;
				Source.seekg(0, Source.end);
				FileSize = (uint32)Source.tellg();
				Source.seekg(0, Source.beg);

				//Allocate memory:
				Data->resize(FileSize);
				Source.read(&(*Data)[0], FileSize);
				Source.close();
			}

		}

		catch (const json::exception& eInfo)
		{
			Log::SetError(Log::FmtStr("Unable to Parse GLTF file. Currupted json data Error:%s", eInfo.what()), HYD_INVALID_GLTF);
			return HYD_INVALID_GLTF;
		}

		return HYD_OK;
	}

	uint32 GLTFLoader::SetupBufferViews(json & pBufferViews, const std::vector<std::string>& pBufferData, std::vector<BufferView>& pBufferViewData)
	{
		if (pBufferViews == nullptr)
			return HYD_CORRUPTED_GLTF;

		try
		{

			for (auto &i : pBufferViews)
			{
				pBufferViewData.emplace_back();
				BufferView* bufferView = &pBufferViewData.back();

				int32 Target = i.value("target", -1);
				if (Target == -1)
				{
					Log::SetError("Invalid bufferView target", HYD_INVALID_TARGET, __FILE__, __LINE__);
					pBufferViewData.clear();
					return HYD_INVALID_TARGET;
				}

				int32 BufferIndex = i.value("buffer", -1);
				if (BufferIndex == -1)
				{
					Log::SetError("Invalid buffer index", HYD_INVALID_VALUE, __FILE__, __LINE__);
					pBufferViewData.clear();
					return HYD_INVALID_VALUE;
				}

				bufferView->Target = Target;

				uint32 ByteOffset = i.value("byteOffset", 0);
				uint32 ByteLength = i["byteLength"];
				uint32 ByteStride = i.value("byteStride", 0);
				bufferView->Data = pBufferData[BufferIndex].substr(ByteOffset, ByteLength);
				
			}

		}

		catch (const json::exception& eInfo)
		{
			Log::SetError(Log::FmtStr("Unable to Parse GLTF file. Currupted json data Error:%s", eInfo.what()), HYD_INVALID_GLTF);
			return HYD_INVALID_GLTF;
		}


		return HYD_OK;
	}

	uint32 GLTFLoader::SetupAccessors(json& pAccessors, const std::vector<BufferView>& pBufferViewData, std::vector<Accessor>& pAccessorData)
	{

		if (pAccessors == nullptr)
			return HYD_CORRUPTED_GLTF;

		try
		{
			for (auto& i : pAccessors)
			{
				pAccessorData.emplace_back();
				Accessor* accessor = &pAccessorData.back();

				int32 BufferviewIndex = i.value("bufferView", -1);
				if (BufferviewIndex == -1 || BufferviewIndex >= pBufferViewData.size())
				{
					Log::SetError("Invalid buffer view index", HYD_INVALID_VALUE, __FILE__, __LINE__);
					pAccessorData.clear();
					return HYD_INVALID_VALUE;
				}

				uint32 byteOffset = i.value("byteOffset", 0);

				accessor->Count			= i["count"];
				accessor->ComponentType = VertexArray::GetHydType(i["componentType"]);
				accessor->VectorType    = Accessor::RetriveType(std::string(i["type"]));

				accessor->Data.Target = pBufferViewData[BufferviewIndex].Target;
				accessor->Data.Data  = pBufferViewData[BufferviewIndex].Data.substr(byteOffset);

			}
		}

		catch (const json::exception& eInfo)
		{
			Log::SetError(Log::FmtStr("Unable to Parse GLTF file. Currupted json data Error:%s", eInfo.what()), HYD_INVALID_GLTF);
			return HYD_INVALID_GLTF;
		}


		return HYD_OK;
	}

	uint32 GLTFLoader::SetupMeshes(json& pMeshes, const std::vector<Accessor>& pAccessorData, std::vector<Id>& pMaterials, std::vector<Id>& pMeshIDs, Model* pCurrentModel)
	{

		if (pMeshes == nullptr)
			return HYD_CORRUPTED_GLTF;

		try
		{
			
			for (auto& i : pMeshes)
			{
				Mesh* mesh = ResourcePool<Mesh>::New();

				mesh->m_Name = i.value("name", "unamed");
				if (SetupPrimitives(i["primitives"], pAccessorData, pMaterials, mesh) == HYD_INVALID_VALUE)
				{
					//Skip this mesh because it's empty
					continue;
				}

				pMeshIDs.push_back(pCurrentModel->AddMesh(&mesh));
			}

		}
		catch (const json::type_error& eInfo)
		{
			Log::SetError(Log::FmtStr("Unable to Parse GLTF file. Currupted json data Error:%s", eInfo.what()), HYD_INVALID_GLTF);
			return HYD_INVALID_GLTF;
		}
		return HYD_OK;
	}


	/*
	
		we construct buffers from accessors here
	
	*/
	uint32 GLTFLoader::SetupPrimitives(json& pPrimitives, const std::vector<Accessor>& pAccessorData,  std::vector<Id>& pMaterials, Mesh* pCurrentMesh)
	{
		if (pPrimitives == nullptr)
			return HYD_INVALID_VALUE; //this means this mesh doesn't have any primitive therefore it's an empty mesh.

		try 
		{
			for (auto& i : pPrimitives)
			{
				Primitive primitive;
				primitive.m_RenderingMode = i.value("mode", 4);

				int32 MaterialIndex = i.value("material", -1);
				if (s_Flags != NO_MATERIAL && MaterialIndex != -1)
					primitive.m_Material = pMaterials[MaterialIndex];

				//Creates Vertex Buffer
				ProcessAttributes(i["attributes"], pAccessorData, primitive);

				if (i.find("indices") != i.end())
				{

					uint32 indicies = i["indices"];

					//Create Element Buffer:
					primitive.Count = pAccessorData[indicies].Count;
					primitive.Type  = VertexArray::GetGLType(pAccessorData[indicies].ComponentType);
					
					primitive.m_ElementBuffer = s_CurrentScene->m_Buffers.CreateElementBuffer(
						pAccessorData[indicies].Count,
						VertexArray::GetGLType(pAccessorData[indicies].ComponentType),
						(void*)&pAccessorData[indicies].Data.Data[0]);
				
				}

				pCurrentMesh->m_Primitives.push_back(std::move(primitive));
			}
		}

		catch (const json::exception& eInfo)
		{
			Log::SetError(Log::FmtStr("Unable to Parse GLTF file. Currupted json data Error:%s", eInfo.what()), HYD_INVALID_GLTF);
			return HYD_INVALID_GLTF;
		}

		return HYD_OK;
	}

	uint32 GLTFLoader::ProcessAttributes(json & pAttributes, const std::vector<Accessor>& pAccessorData, Primitive& pCurrentPrimitive)
	{
		if (pAttributes == nullptr)
			return HYD_CORRUPTED_GLTF;
		try
		{

			std::vector<int32> Indecies = {
				pAttributes.value("POSITION", -1),
				pAttributes.value("NORMAL", -1),
				pAttributes.value("TANGENT", -1),
				pAttributes.value("TEXCOORD_0", -1),
				pAttributes.value("TEXCOORD_1", -1),
				pAttributes.value("COLOR_0", -1)
			};

			//Calculate the buffer size:
			uint32 VBOSize = 0;
			for (auto &i : Indecies)
			{
				if (i == -1)
					continue;
				VBOSize += (uint32)pAccessorData[i].Data.Data.size();
			}

			
			pCurrentPrimitive.m_VertexArrays = s_CurrentScene->m_Buffers.CreateVertexArray();
			s_CurrentScene->m_Buffers.BindArray(pCurrentPrimitive.m_VertexArrays);


			//Vertex Buffer Setup:
			
			pCurrentPrimitive.m_VertexBuffer = s_CurrentScene->m_Buffers.CreateVertexBuffer(VBOSize);
			s_CurrentScene->m_Buffers.BindVertexBuffer(pCurrentPrimitive.m_VertexBuffer);

			uint32 Offset = 0;
			uint32 AttribCount = 0;
			uint32 Stride = 0;

			for (auto &i : Indecies)
			{
				if (i == -1)
					continue;//skip
				const Accessor& accessor = pAccessorData[i];

				s_CurrentScene->m_Buffers.CopyVertexDataChunk(pCurrentPrimitive.m_VertexBuffer,
					Offset, (uint32)accessor.Data.Data.size(), (void*)(&accessor.Data.Data[0]));

				Stride =  (uint32)accessor.VectorType * VertexArray::GetTypeSize(accessor.ComponentType);


				const Layout layout(accessor.ComponentType, (uint32)accessor.VectorType, AttribCount, false, Offset, Stride);
				s_CurrentScene->m_Buffers.AddVertexAttribute(pCurrentPrimitive.m_VertexArrays, layout);

				Offset += (uint32)pAccessorData[i].Data.Data.size();
				AttribCount++;
			}
		}

		catch (const json::exception& eInfo)
		{
			Log::SetError(Log::FmtStr("Unable to Parse GLTF file. Currupted json data Error:%s", eInfo.what()), HYD_INVALID_GLTF);
			return HYD_INVALID_GLTF;
		}

		return  HYD_OK;
	}



	uint32 GLTFLoader::ProcessNodes(json& pNodes, std::vector<GeoNode>& nodes)
	{

		if (pNodes == nullptr)
			return HYD_CORRUPTED_GLTF;

		try
		{
			for (auto &i : pNodes)
			{
				GeoNode node;
				node.Mesh = i.value("mesh", -1);
				
				for (auto &j : i["children"])
				{
					node.Children.push_back(uint32(j));
				}

				if (i.find("matrix") != i.end())
				{


					Log::SetInfo("Loading Matrix into memory...");

					//Read the matrix;
					uint32 I = 0, J = 0;
					for (auto &j : i["matrix"])
					{
						//node.Transformation[I][J] = uint32(i);
						I++;

						if (I > 3)
						{
							I = 0;
							J++;
						}

					}
				}

				else
				{
					if (i.find("scale") != i.end())
					{
						node.Transformation.Scale = Vec3(
							(float)i["scale"].at(0),
							(float)i["scale"].at(1),
							(float)i["scale"].at(2)
						);
					}

					if (i.find("rotation") != i.end())
					{
						node.Transformation.Rotation.Vector = Vec3(
							(float)i["rotation"].at(0),
							(float)i["rotation"].at(1),
							(float)i["rotation"].at(2)
						);
						node.Transformation.Rotation.Scaler = (float)i["rotation"].at(3);
					}

					if (i.find("translation") != i.end())
					{

						node.Transformation.Translation.X = (float)i["translation"].at(0);
						node.Transformation.Translation.Y = (float)i["translation"].at(1);
						node.Transformation.Translation.Z = (float)i["translation"].at(2);
					}
					node.ModelMatrix = CalculateMatrix(node.Transformation);
				}

				nodes.push_back(node);
			}
		}

		catch (const json::exception& eInfo)
		{
			Log::SetError(Log::FmtStr("Unable to Parse GLTF file. Currupted json data Error:%s", eInfo.what()), HYD_INVALID_GLTF);
			return HYD_INVALID_GLTF;
		}



		return HYD_OK;
	}

	uint32 GLTFLoader::ProcessScene(json& pScene, const std::vector<GeoNode>& nodes, const std::vector<Id>& pMeshIDs, Model* pCurrentModel)
	{
		if (pScene == nullptr)
			return HYD_CORRUPTED_GLTF;

		try
		{
			pCurrentModel->SetName(pScene.value("name", "unamed"));


			//Traversing the Tree in Preorder

			std::stack<GeoNode> Stack;
			
			for (auto& i : pScene["nodes"])
			{
				Stack.push(nodes[uint32(i)]);
			}

			

			while (!Stack.empty())
			{
				GeoNode Current = Stack.top();
				Stack.pop();

				if (Current.Mesh != -1)
				{
					pCurrentModel->GetMesh(pMeshIDs[Current.Mesh]).SetTransform(Current.ModelMatrix);
				}

				for (auto &i : Current.Children)
				{
					GeoNode Child = nodes[i];
					Child.ModelMatrix =  Current.ModelMatrix * Child.ModelMatrix;
					Stack.push(Child);
				}
			}

		}

		catch (const json::exception& eInfo)
		{
			Log::SetError(Log::FmtStr("Unable to Parse GLTF file. Currupted json data Error:%s", eInfo.what()), HYD_INVALID_GLTF);
			return HYD_INVALID_GLTF;
		}

		return HYD_OK;
	}

	uint32 GLTFLoader::LoadMaterials(json & pGLTF, std::vector<Id>& pMaterials, const std::vector<BufferView>& pBufferViews)
	{

		uint32 Err = HYD_OK;
		
		
		PROFILE_START("Image Loading")
		//Load Images
		std::vector<Image> Images;
		Err = LoadImages(pGLTF["images"], s_RootPath, Images, pBufferViews);
		
		if (Err != HYD_OK)
		{
			Images.clear();
			return Err;
		}

		PROFILE_STOP

		PROFILE_START("Samplers Loading")
		std::vector<Sampler> Samplers;
		Err = SetupSamplers(pGLTF["samplers"], Samplers);

		if (Err != HYD_OK)
		{
			Images.clear();
			Samplers.clear();
			return Err;
		}
		PROFILE_STOP


		PROFILE_START("Loading Texture")
		std::vector<Id> Textures;
		Err = SetupTextures(pGLTF["textures"], Textures, Samplers, Images);

		if (Err != HYD_OK)
		{
			Images.clear();
			Samplers.clear();
			Textures.clear();
			return Err;
		}
		PROFILE_STOP

		PROFILE_START("Setup Materials")
		Err = SetupMaterials(pGLTF["materials"], pMaterials, Textures);
		PROFILE_STOP

		Images.clear();
		Samplers.clear();

		return Err;
	}

	//======================Material Loading================================
	uint32 GLTFLoader::LoadImages(json & pImages, const std::string & pRootPath, std::vector<Image>& pImageData, const std::vector<BufferView>& pBufferViews)
	{
		if (pImages == nullptr)
			return HYD_CORRUPTED_GLTF; //We don't have any images 

		try
		{
			for (auto& image : pImages)
			{
				pImageData.emplace_back();
				Image& CurrentImage = pImageData.back();
				//If URI was defined:
				if (image.find("uri") != image.end())
				{
					std::string URI = image["uri"];
					
					if (CurrentImage.LoadImage((pRootPath + URI).c_str()) == HYD_IMAGE_FAILED)
						Log::SetError(Log::FmtStr("Failed to Load Image at %s", (s_RootPath+URI).c_str()));
				}
				// if bufferView was defined
				else if (image.find("bufferView") != image.end())
				{
					uint32 Index = (uint32)image["bufferView"];

					CurrentImage.LoadImage(&pBufferViews[Index].Data[0]);
				}
				//File curreupted
				else
				{
					Log::SetError(Log::FmtStr("Unable to Load Image: %s", image.value("name", "unamed")));
					continue;
				}
			}
		}

		catch (const json::exception& eInfo)
		{
			Log::SetError(Log::FmtStr("Unable to Parse GLTF file. Currupted json data Error:%s", eInfo.what()), HYD_INVALID_GLTF);
			return HYD_CORRUPTED_GLTF;
		}

		return HYD_OK;
	}

	uint32 GLTFLoader::SetupSamplers(json & pSamplers, std::vector<Sampler>& pSamplerData)
	{
		if (pSamplers == nullptr)
			return HYD_CORRUPTED_GLTF;

		try
		{
			for (auto& sampler : pSamplers)
			{
				pSamplerData.emplace_back();
				Sampler& CurrentSampler = pSamplerData.at(pSamplerData.size() - 1);

				CurrentSampler.Mag   = (Filter)sampler.value("magFilter", (uint32)LINEAR);
				CurrentSampler.Min   = (Filter)sampler.value("minFilter", (uint32)LINEAR);
				CurrentSampler.WrapS = (Wrap)sampler.value("wrapS", (uint32)REPEAT);
				CurrentSampler.WrapT = (Wrap)sampler.value("wrapT", (uint32)REPEAT);
			}
		}
		catch (const json::exception& eInfo)
		{
			Log::SetError(Log::FmtStr("Unable to Parse GLTF file. Currupted json data Error:%s", eInfo.what()), HYD_INVALID_GLTF);
			return HYD_INVALID_GLTF;
		}

		return HYD_OK;
	}


	uint32 GLTFLoader::SetupTextures(json& pTextures, std::vector<Id>& pTextureIds, const std::vector<Sampler>& pSamplerData, const std::vector<Image>& pImageData)
	{

		if (pTextures == nullptr)
			return HYD_CORRUPTED_GLTF;

		try
		{
			for (auto& texture : pTextures)
			{
				uint32 ImageIndex = texture["source"];
				Sampler sampler;
				
				if (texture.find("sampler") != texture.end())
					sampler = pSamplerData.at(texture["sampler"]);

				//pTextureIds.push_back(Create2DTexture(pImageData[ImageIndex], sampler));

			}
		}

		catch (const json::exception& eInfo)
		{
			Log::SetError(Log::FmtStr("Unable to Parse GLTF file. Currupted json data Error:%s", eInfo.what()), HYD_INVALID_GLTF);
			return HYD_INVALID_GLTF;
		}

		return HYD_OK;
	}


	uint32 GLTFLoader::SetupMaterials(json& pMaterials, std::vector<Id>& pMaterial, std::vector<Id>& pTextureIds)
	{
		if (pMaterials == nullptr)
			return HYD_CORRUPTED_GLTF;
		try
		{
			for (auto& material : pMaterials)
			{
				Material* Current =  ResourcePool<Material>::New();

				//Current->m_Name = material.value("name", "unamed");

				json& pbrMetal = material["pbrMetallicRoughness"];

				//Current->m_MetalicnessFactor = pbrMetal.value("metallicFactor", 1.0f);
				//Current->m_RoughnessFactor   = pbrMetal.value("roughnessFactor", 1.0f);

				//Textures:
				if (pbrMetal.find("baseColorTexture") != pbrMetal.end())
				{
					json& BaseColor = pbrMetal["baseColorTexture"];

					uint32 Index = BaseColor["index"];
					//Current->m_BaseColorTexture = pTextureIds[Index];
				}

				
			}
		}

		catch (const json::exception& eInfo)
		{
			Log::SetError(Log::FmtStr("Unable to Parse GLTF file. Currupted json data Error:%s", eInfo.what()), HYD_INVALID_GLTF);
			return HYD_INVALID_GLTF;
		}

		return HYD_OK;
	}
};