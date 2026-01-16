#include "Model.h"

namespace Hydrogen
{


	int Model::LoadModel(std::string & pPath)
	{
		FileExtension File = GetFileExtension(pPath);

		switch (File)
		{
		case FILE_GLTF:
			//Load GLTF
			return ParseGLTF(pPath);
		case FILE_GLB:
			//Load GLB
			return ParseGLB(pPath);
		}
		return HYD_INVALID_FILE;
	}

	int Model::Free()
	{
		m_Meshes.clear();
		m_Buffers.clear();
		m_VertexArrays.clear();
		return 0;
	}

	void Model::RenderScene(Shader& pShader, uint32 pTargetScene, Matrix* pModelTransformation)
	{
		m_Scenes[pTargetScene].RenderScene(pShader, this, pModelTransformation);
	}



	int Model::ParseGLTF(std::string& pPath)
	{
		//Open The File And Parse it
		std::ifstream Source(pPath);
		if (Source.fail())
		{
			Log::SetError(HYD_INVALID_PATH, __FILE__, __LINE__);
			return HYD_INVALID_PATH;
		}
		m_GLTF = json::parse(Source);
		Source.close();

		m_RootPath = pPath.substr(0, pPath.find_last_of("\\") + 1);
		ProcessGLTF(m_GLTF, GLTF);

		//Clear the Json File
		m_GLTF.clear();
		return HYD_OK;
	}


	int Model::ParseGLB(std::string & pPath)
	{

		m_RootPath = pPath.substr(0, pPath.find_last_of("\\") + 1);

		std::ifstream SourceFile(pPath, std::ios::in | std::ios::binary);
		if (SourceFile.fail())
		{
			Log::SetError(HYD_INVALID_PATH, __FILE__, __LINE__);
			return HYD_INVALID_PATH;
		}

		//Retrive the File Size in bytes:
		SourceFile.seekg(0, SourceFile.end);
		uint32 FileSize = SourceFile.tellg();
		SourceFile.seekg(0, SourceFile.beg);


		std::string GLBContent;
		GLBContent.resize(FileSize);
		SourceFile.read(&GLBContent[0], FileSize);

		SourceFile.close();
		
		std::string Header = GLBContent.substr(0, 12);

		//Header Properties:
		uint32 Magic = 0;
		uint32 Version = 0;
		uint32 Length = 0;

		//Retrive the Properties:
		memcpy((void*)&Magic, (void*)&(Header.substr(0, 4)),   sizeof(uint32));
		memcpy((void*)&Version, (void*)&(Header.substr(4, 8)), sizeof(uint32));
		memcpy((void*)&Length, (void*)&(Header.substr(8, 12)), sizeof(uint32));

		//Check the Magic Code:
		if (Magic != GLB_MAGIC)
		{
			Log::SetError(HYD_INVALID_GLB, __FILE__, __LINE__);
			return HYD_INVALID_GLB;
		}

		// we check to make sure the file is NOT Currepted
		if (Length > FileSize)
		{
			Log::SetError(HYD_CORRUPTED_GLB, __FILE__, __LINE__);
			return HYD_CORRUPTED_GLB;
		}


		std::string Chunks;
		Chunks.resize(Length - 12);
		memcpy((void*)&Chunks[0], (void*)&GLBContent.substr(12)[0], Length - 12);

		std::string JsonData;
		std::string BinData;

		uint32 Offset = 0;
		while (Offset < Length-12)
		{
			uint32 ChunkType = 0;
			uint32 ChunkLength = 0;

			memcpy((void*)&ChunkLength, (void*)&(Chunks.substr(Offset, Offset + 4))[0], 4);
			memcpy((void*)&ChunkType, (void*)&(Chunks.substr(Offset+4, Offset + 8))[0], 4);

			Offset += 8;

			switch (ChunkType)
			{
			case GLB_JSON_TYPE:
				JsonData.resize(ChunkLength);
				memcpy((void*)&JsonData[0], (void*)&(Chunks.substr(Offset, Offset + ChunkLength))[0], ChunkLength);
				break;
			case GLB_BIN_TYPE:
				BinData.resize(ChunkLength);
				memcpy((void*)&BinData[0], (void*)&(Chunks.substr(Offset, Offset + ChunkLength))[0], ChunkLength);
				break;
			}

			Offset += (ChunkLength);
		}


		m_GLTF = json::parse(JsonData);
		int Err = ProcessGLTF(m_GLTF, GLB, &BinData);
		m_GLTF.clear();

		return Err;
	}

	int Model::ProcessGLTF(json & pGLTF, FileFormat pFormat, std::string* pGLBbinary)
	{
		//Load URIs:
		std::vector<std::string> URIBuffers;

		int Err = 0;

		if (pFormat == GLTF)
		{
			Err = LoadURI(m_GLTF["buffers"], m_RootPath, URIBuffers);
			if (Err != HYD_OK)
			{
				std::cout << "Can not access the uris\n";
				return Err;
			}
		}

		else if (pFormat == GLB)
		{
			URIBuffers.push_back(*pGLBbinary);
		}

		//Construct Buffer Views
		std::vector<BufferView> BufferViews;
		Err = SetupBufferViews(m_GLTF["bufferViews"], BufferViews, URIBuffers);
		if (Err != HYD_OK)
		{
			return Err;
		}
		//Free the Uris cached data:
		URIBuffers.clear();


		//Construct Accessors:
		std::vector<Accessor> Accessors;
		Err = SetupAccessors(m_GLTF["accessors"], Accessors, BufferViews);
		if (Err != HYD_OK)
		{
			return Err;
		}
		//Free Buffer View Cache:
		BufferViews.clear();

		Err = SetupMeshes(m_GLTF["meshes"], Accessors);
		if (Err != HYD_OK)
		{
			return Err;
		}
		Accessors.clear();

		Err = SetupNodes(m_GLTF["nodes"]);
		if (Err != HYD_OK)
		{
			return Err;
		}

		Err = SetupScenes(m_GLTF["scenes"]);
		if (Err != HYD_OK)
		{
			return Err;
		}

		m_DefaultScene = m_GLTF.value("scene", -1);
		return HYD_OK;
	}

	FileExtension Model::GetFileExtension(std::string & pPath)
	{
		std::string Prefix = pPath.substr(pPath.find_last_of("."));

		if(Prefix == ".gltf")
			return FILE_GLTF;
		else if (Prefix == ".glb")
			return FILE_GLB;
		
		return FILE_INVALID;
	}

	int Model::LoadURI(json & pBuffer, std::string & pRootPath, std::vector<std::string>& pBuffers)
	{
		if (pBuffer == nullptr)
		{
			Log::SetError(HYD_CORRUPTED_GLTF, __FILE__, __LINE__);
			return HYD_CORRUPTED_GLTF;
		}

		std::ifstream Uri;
		for (auto &i : pBuffer)
		{
			Uri.open(pRootPath + i.value("uri", ""), std::ios::binary);
			if (Uri.fail())
			{
				Log::SetError(HYD_URI_FAILED, __FILE__, __LINE__);
				return HYD_URI_FAILED;
			}

			std::string Data;
			Data.resize(i.value("byteLength", 0));
			Uri.read(&Data[0], Data.size());
			pBuffers.push_back(std::move(Data));
			Uri.close();
		}
		return HYD_OK;
	}

	int Model::SetupBufferViews(json & pBufferView, std::vector<BufferView>& pBufferViews, std::vector<std::string>& pBuffers)
	{
		if (pBufferView == nullptr)
		{
			Log::SetError(HYD_CORRUPTED_GLTF, __FILE__, __LINE__);
			return HYD_CORRUPTED_GLTF;
		}

		for (auto &i : pBufferView)
		{
			pBufferViews.push_back({});
			pBufferViews[pBufferViews.size() - 1].Target = i["target"];
			pBufferViews[pBufferViews.size() - 1].Data = pBuffers.at((size_t)i.value("buffer", 0)).substr((size_t)i.value("byteOffset", 0), (size_t)i.value("byteLength", -1));

		}

		return HYD_OK;
	}

	int Model::SetupAccessors(json & pAccessor, std::vector<Accessor>& pAccessors, std::vector<BufferView>& pBufferViews)
	{
		if (pAccessor == nullptr)
		{
			Log::SetError(HYD_CORRUPTED_GLTF, __FILE__, __LINE__);
			return HYD_CORRUPTED_GLTF;
		}
		for (auto &i : pAccessor)
		{
			pAccessors.push_back({});
			pAccessors.at(pAccessors.size() - 1).Data = pBufferViews.at(i["bufferView"]);
			pAccessors.at(pAccessors.size() - 1).ComponentType = i.value("componentType", GL_INVALID_ENUM);
			pAccessors.at(pAccessors.size() - 1).Count = i.value("count", 0);
			pAccessors.at(pAccessors.size() - 1).DataType = Accessor::RetriveType(std::string(i["type"]));
		}

		return HYD_OK;
	}

	int Model::SetupMeshes(json & pMeshes, std::vector<Accessor>& pAccessors)
	{
		if (pMeshes == nullptr)
		{
			Log::SetError(HYD_CORRUPTED_GLTF, __FILE__, __LINE__);
			return HYD_CORRUPTED_GLTF;
		}

		for (auto &i : pMeshes)
		{
			std::vector<Primitive> Primitives;
			ProcessPrimitives(i["primitives"], pAccessors, Primitives);

			//Consturct Mesh Object
			//----------------
			m_Meshes.push_back(Mesh(i.value("name", "Unknown"), Primitives));
			//std::cout << "Mesh Constructed. Name:" << m_Meshes[m_Meshes.size() - 1].GetMeshName() << '\n';
		}

		return HYD_OK;
	}

	int Model::SetupMaterials(json & pMaterial)
	{
		if (pMaterial == nullptr)
			return HYD_CORRUPTED_GLTF;



		return HYD_OK;
	}

	int Model::SetupNodes(json & pNodes)
	{
		if (pNodes == nullptr)
		{
			Log::SetError(HYD_CORRUPTED_GLTF, __FILE__, __LINE__);
			return HYD_CORRUPTED_GLTF;
		}

		for (auto& i : pNodes)
		{
			Node node(i.value("name", "unnamed"), 
			((i.value("mesh", -1) != -1)?&m_Meshes[i["mesh"]] : nullptr));


			if (i.find("scale") != i.end())
			{
				node.m_Scale.X = i["scale"][0];
				node.m_Scale.Y = i["scale"][1];
				node.m_Scale.Z = i["scale"][2];

				node.GetModelMatrix() = Scale(node.GetModelMatrix(), node.m_Scale);
			}

			if (i.find("rotation") != i.end())
			{
				node.m_Rotation.X = i["rotation"][0];
				node.m_Rotation.Y = i["rotation"][1];
				node.m_Rotation.Z = i["rotation"][2];
				node.m_Rotation.W = i["rotation"][3];
				node.GetModelMatrix() = RotateQuaternion(node.GetModelMatrix(), node.m_Rotation);
			}

			//TRS Properties:
			if (i.find("translation") != i.end())
			{
				node.m_Translation.X = i["translation"][0];
				node.m_Translation.Y = i["translation"][1];
				node.m_Translation.Z = i["translation"][2];
				node.GetModelMatrix() = Translation(node.GetModelMatrix(), node.m_Translation);
			}
			//Matrix (if Present):


			if (i.find("matrix") != i.end())
			{
				json& Matrix = i["matrix"];
				std::vector<float> Values;
				for (auto& j : Matrix)
				{
					Values.push_back(float(j));
				}
				node.m_LocalTransformation.CopyDataToMatrix(Values);
			}


			for (auto& child : i["children"])
			{
				node.m_Children.push_back(child);
			}

			//node.m_LocalTransformation.PrintMatrix();
			//std::cout << '\n';
			m_Nodes.push_back(node);
		}
		
		return HYD_OK;
	}

	int Model::SetupScenes(json pScenes)
	{
		if (pScenes == nullptr)
		{
			Log::SetError(HYD_CORRUPTED_GLTF, __FILE__, __LINE__);
			return HYD_CORRUPTED_GLTF;
		}
		for (auto &i : pScenes)
		{
			Scene scene(i.value("name", "unnamed"));
			for (auto &node: i["nodes"])
			{
				scene.m_Nodes.push_back(&m_Nodes[node]);
			}
			m_Scenes.push_back(scene);
		}

		return HYD_OK;
	}

	int Model::ProcessPrimitives(json& pPrimitive, std::vector<Accessor>& pAccessors, std::vector<Primitive>& pPrimitives)
	{
		if (pPrimitive == nullptr)
		{
			Log::SetError(HYD_CORRUPTED_GLTF, __FILE__, __LINE__);
			return HYD_CORRUPTED_GLTF;
		}
		for (auto &i : pPrimitive)
		{
			Primitive primitive;
			
			ProcessAttributes(i["attributes"], pAccessors, primitive);

			//Take care of ElementBuffer
			if (i.value("indices", -1) != -1)
			{
				ProcessElementBuffer(pAccessors[i["indices"]], primitive.EboID);
			}
			primitive.MaterialID = i.value("material", -1);
			pPrimitives.push_back(primitive);
		}
		return HYD_OK;
	}

	int Model::ProcessElementBuffer(Accessor & pAccessors, int32 & pEboID)
	{
		Buffer Ebo;
		Ebo.CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, pAccessors.Data.Data.length(), &pAccessors.Data.Data[0], pAccessors.Count);
		m_Buffers.push_back(std::move(Ebo));
		pEboID = m_Buffers.size() - 1;

		return HYD_OK;
	}


	int Model::ProcessAttributes(json& pAttribute, std::vector<Accessor>& pAccessors, Primitive &pPrimitiveRef)
	{

		int32 BufferSize = 0;

		VertexArray VAO;
		VAO.CreateVertexArray();
		VAO.Bind();

		// A Temp cache for storing Attribs Index
		std::vector<int32> AttributeStorage;

		//Check The Supported Attributes:
		int32 Index = -1;
		if ((Index=pAttribute.value("POSITION", -1)) != -1)
		{
			BufferSize += pAccessors[Index].Data.Data.length();
			pPrimitiveRef.Attributes.POSITION = Index;
			AttributeStorage.push_back(Index);
		}
		if ((Index = pAttribute.value("NORMAL", -1)) != -1)
		{
			BufferSize += pAccessors[Index].Data.Data.length();
			pPrimitiveRef.Attributes.NORMALS = Index;
			AttributeStorage.push_back(Index);
		}
		if ((Index = pAttribute.value("TANGENT", -1)) != -1)
		{
			BufferSize += pAccessors[Index].Data.Data.length();
			pPrimitiveRef.Attributes.TANGENT = Index;
			AttributeStorage.push_back(Index);
		}
		if ((Index = pAttribute.value("TEXCOORD_0", -1)) != -1)
		{
			BufferSize += pAccessors[Index].Data.Data.length();
			pPrimitiveRef.Attributes.TEXCOORDS_0 = Index;
			AttributeStorage.push_back(Index);
		}
		if ((Index = pAttribute.value("TEXCOORD_1", -1)) != -1)
		{
			BufferSize += pAccessors[Index].Data.Data.length();
			pPrimitiveRef.Attributes.TEXCOORDS_1 = Index;
			AttributeStorage.push_back(Index);
		}
		if ((Index = pAttribute.value("COLOR_0", -1)) != -1)
		{
			BufferSize += pAccessors[Index].Data.Data.length();
			pPrimitiveRef.Attributes.COLOR_0 = Index;
		}

		Buffer VBO;
		VBO.CreateBuffer(GL_ARRAY_BUFFER, BufferSize, nullptr);

		int32 Offset = 0;
		for (auto &i : AttributeStorage)
		{
			VBO.CopyDataChunk(Offset, pAccessors[i].Data.Data.length(), &pAccessors[i].Data.Data[0]);
			VAO.AddAttribute(pAccessors[i]);
			Offset += pAccessors[i].Data.Data.length();
		}

		m_Buffers.push_back(std::move(VBO));
		pPrimitiveRef.VboID = m_Buffers.size() - 1;
		m_VertexArrays.push_back(std::move(VAO));
		pPrimitiveRef.VaoID = m_VertexArrays.size() - 1;

		return HYD_OK;
	}

	Type Accessor::RetriveType(std::string & pType)
	{
		if (pType == "SCALAR")
			return SCALER;
		else if (pType == "VEC2")
			return VEC2;
		else if (pType == "VEC3")
			return VEC3;
		else if (pType == "VEC4")
			return VEC4;
		else if (pType == "MAT2")
			return MAT2;
		else if (pType == "MAT3")
			return MAT3;
		else if (pType == "MAT4")
			return MAT4;
		return INVALID;
	}

};