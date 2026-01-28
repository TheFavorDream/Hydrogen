#include "GLTF.h"

namespace Hydrogen
{


	uint32				GLTFLoader::s_Version;
	std::string			GLTFLoader::s_RootPath;

	Model* GLTFLoader::Load(const std::string& pPath)
	{
		s_RootPath = pPath.substr(0, pPath.find_last_of("\\")) + "\\";

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

		std::ifstream SourceFile(pPath);
		json GLTF = json::parse(SourceFile);
		SourceFile.close();


		uint32 Err = HYD_OK;

		std::vector<std::string> Buffers;
		Err = SetupBuffers(GLTF["buffers"], Buffers);
		if (Err != HYD_OK)
		{
			return nullptr;
		}


		std::vector<BufferView> BufferViews;
		Err = SetupBufferViews(GLTF["bufferViews"], Buffers, BufferViews);
		if (Err != HYD_OK)
		{
			return nullptr;
		}
		//We don't need Buffers anymore
		Buffers.clear();


		std::vector<Accessor> Accessors;
		Err = SetupAccessors(GLTF["accessors"], BufferViews, Accessors);
		if (Err != HYD_OK)
		{
			return nullptr;
		}
		BufferViews.clear();


		Model* model = new Model();

		Err = SetupMeshes(GLTF["meshes"], Accessors, model);
		if (Err != HYD_OK)
		{
			delete model;
			return nullptr;
		}


		std::vector<Node> nodes;
		Err = ProcessNodes(GLTF["nodes"], nodes);
		if (Err != HYD_OK)
		{
			delete model;
			return nullptr;
		}


		Err = ProcessScene(GLTF["scenes"][0], nodes, model);
		nodes.clear();

		if (Err != HYD_OK)
		{
			delete model;
			return nullptr;
		}

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
				pBufferData.push_back(std::string()); //Push an empty string
				std::string* Data = &pBufferData.at(pBufferData.size()-1);

				std::string Path = s_RootPath + std::string(i["uri"]);
				std::fstream Source(Path, std::ios::binary || std::ios::in);
				if (!Source.is_open())
				{
					Log::SetError("Failed to load external URI", HYD_INVALID_PATH, __FILE__, __LINE__);
					Source.close();
					pBufferData.clear();
					return HYD_INVALID_PATH;
				}

				//Retrive the File size;
				uint32 FileSize = 0;
				Source.seekg(0, Source.end);
				FileSize = Source.tellg();
				Source.seekg(0, Source.beg);

				//Allocate memory:
				Data->resize(FileSize);
				Source.read(&(*Data)[0], FileSize);
				Source.close();
			}

		}

		catch (...)
		{
			//replace with Assertion
			__debugbreak();
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
				pBufferViewData.push_back({});
				BufferView* bufferView = &pBufferViewData.at(pBufferViewData.size() - 1);

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

				uint32 ByteOffset = i["byteOffset"];
				uint32 ByteLength = i["byteLength"];
				bufferView->Data = pBufferData[BufferIndex].substr(ByteOffset, ByteLength);
				
			}

		}

		catch (...)
		{
			//replace with Assertion
			__debugbreak();
		}


		return HYD_OK;
	}

	uint32 GLTFLoader::SetupAccessors(json & pAccessors, const std::vector<BufferView>& pBufferViewData, std::vector<Accessor>& pAccessorData)
	{

		if (pAccessors == nullptr)
			return HYD_CORRUPTED_GLTF;

		try
		{
			for (auto& i : pAccessors)
			{
				pAccessorData.push_back({});
				Accessor* accessor = &pAccessorData.at(pAccessorData.size()-1);

				int32 BufferviewIndex = i.value("bufferView", -1);
				if (BufferviewIndex == -1 || BufferviewIndex >= pBufferViewData.size())
				{
					Log::SetError("Invalid buffer view index", HYD_INVALID_VALUE, __FILE__, __LINE__);
					pAccessorData.clear();
					return HYD_INVALID_VALUE;
				}

				uint32 byteOffset = i.value("byteOffset", 0);

				accessor->Count			= i["count"];
				accessor->ComponentType = i["componentType"];
				accessor->DataType		= Accessor::RetriveType(std::string(i["type"]));

				accessor->Data.Target = pBufferViewData[BufferviewIndex].Target;
				accessor->Data.Data  = pBufferViewData[BufferviewIndex].Data.substr(byteOffset);

			}
		}

		catch(...)
		{
			__debugbreak();
		}


		return HYD_OK;
	}

	uint32 GLTFLoader::SetupMeshes(json & pMeshes, const std::vector<Accessor>& pAccessorData, Model* pCurrentModel)
	{

		if (pMeshes == nullptr)
			return HYD_CORRUPTED_GLTF;

		try
		{
			for (auto& i : pMeshes)
			{
				pCurrentModel->m_Meshes.push_back({});
				Mesh* mesh = &pCurrentModel->m_Meshes.at(pCurrentModel->m_Meshes.size() - 1);

				mesh->m_Name = i.value("name", "unamed");
				if (SetupPrimitives(i["primitives"], pAccessorData, pCurrentModel, mesh) == HYD_INVALID_VALUE)
				{
					//Skip this mesh because it's empty
					pCurrentModel->m_Meshes.pop_back();
					continue;
				}


			}
		}

		catch (...)
		{
			__debugbreak();
		}

		return HYD_OK;
	}


	/*
	
		we construct buffers from accessors here
	
	*/
	uint32 GLTFLoader::SetupPrimitives(json& pPrimitives, const std::vector<Accessor>& pAccessorData, Model* pCurrentModel, Mesh* pCurrentMesh)
	{
		if (pPrimitives == nullptr)
			return HYD_INVALID_VALUE; //this means this mesh doesn't have any primitive therefore it's an empty mesh.


		try 
		{
			for (auto& i : pPrimitives)
			{
				pCurrentMesh->m_Primitives.push_back({});
				Primitive* primitive = &pCurrentMesh->m_Primitives.at(pCurrentMesh->m_Primitives.size()-1);

				primitive->RenderingMode = i.value("mode", 4);

				//Creates Vertex Buffer
				ProcessAttributes(i["attributes"], pAccessorData, primitive, pCurrentModel);

				if (i.find("indices") != i.end())
				{
					uint32 indicies = i["indices"];

					pCurrentModel->m_Arrays.at(primitive->VaoID).Bind();
					//Create Element Buffer:

					Buffer EBO;
					EBO.CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, pAccessorData[indicies].Data.Data.size(), (void*)&pAccessorData[indicies].Data.Data[0], pAccessorData[indicies].Count);
					
					
					pCurrentModel->m_Buffers.push_back(std::move(EBO));
					primitive->EboID = pCurrentModel->m_Buffers.size() - 1;
				}
			}
		}

		catch (...)
		{
			__debugbreak();
		}

		return HYD_OK;
	}

	uint32 GLTFLoader::ProcessAttributes(json & pAttributes, const std::vector<Accessor>& pAccessorData, Primitive *pCurrentPrimitive, Model* pCurrentModel)
	{
		if (pAttributes == nullptr)
			return HYD_CORRUPTED_GLTF;
		try
		{
			//Attribue = vertex buffer
			pCurrentPrimitive->Attributes.POSITION    = pAttributes.value("POSITION", -1);
			pCurrentPrimitive->Attributes.NORMALS     = pAttributes.value("NORMAL", -1);
			pCurrentPrimitive->Attributes.TANGENT     = pAttributes.value("TANGENT", -1);
			pCurrentPrimitive->Attributes.TEXCOORDS_0 = pAttributes.value("TEXCOORD_0", -1);
			pCurrentPrimitive->Attributes.TEXCOORDS_1 = pAttributes.value("TEXCOORD_1", -1);
			pCurrentPrimitive->Attributes.COLOR_0	  = pAttributes.value("COLOR_0", -1);

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
				VBOSize += pAccessorData[i].Data.Data.size();
			}

			VertexArray VAO;
			VAO.CreateVertexArray();
			VAO.Bind();

			//Vertex Buffer Setup:
			Buffer VBO;
			VBO.CreateBuffer(GL_ARRAY_BUFFER, VBOSize);
			uint32 Offset = 0;
			for (auto &i : Indecies)
			{
				if (i == -1)
					continue;//skip 
				VBO.CopyDataChunk(Offset, pAccessorData[i].Data.Data.size(), (void*)(&pAccessorData[i].Data.Data[0]));
				Offset += pAccessorData[i].Data.Data.size();

				VAO.AddAttribute(pAccessorData[i]);
			}


			

			pCurrentModel->m_Buffers.push_back(std::move(VBO)); //push an empty buffer
			pCurrentModel->m_Arrays.push_back(std::move(VAO));

			pCurrentPrimitive->VboID = pCurrentModel->m_Buffers.size() - 1;
			pCurrentPrimitive->VaoID = pCurrentModel->m_Arrays.size() - 1;

		}

		catch (...)
		{
			__debugbreak();
		}

		return  HYD_OK;
	}

	uint32 GLTFLoader::ProcessNodes(json& pNodes, std::vector<Node>& nodes)
	{

		if (pNodes == nullptr)
			return HYD_CORRUPTED_GLTF;

		try
		{
			for (auto &i : pNodes)
			{
				Node node;
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
						node.Transformation[I][J] = uint32(i);
						I++;

						if (I > 3)
						{
							I = 0;
							J++;
						}

					}
				}


				if (i.find("scale") != i.end())
				{
					float Scale[3];
					Scale[0] = (float)i["scale"][0];
					Scale[1] = (float)i["scale"][1];
					Scale[2] = (float)i["scale"][2];

					for (uint32 i = 0; i < 3; i++)
						node.Transformation[i][i] *= Scale[i];
					
				}

				if (i.find("rotation") != i.end())
				{
					Vec4 pRotation;
					pRotation.X = (float)i["rotation"][0];
					pRotation.Y = (float)i["rotation"][1];
					pRotation.Z = (float)i["rotation"][2];
					pRotation.W = (float)i["rotation"][3];

					glm::mat4 Result = glm::mat4(1.0f);

					Result[0][0] = 2.0f*((pRotation.W*pRotation.W) + (pRotation.X*pRotation.X)) - 1.0f;
					Result[1][0] = 2.0f*((pRotation.X*pRotation.Y) - (pRotation.W*pRotation.Z));
					Result[2][0] = 2.0f*((pRotation.X*pRotation.Z) + (pRotation.W*pRotation.Y));
					Result[0][1] = 2.0f*((pRotation.X*pRotation.Y) + (pRotation.W*pRotation.Z));
					Result[1][1] = 2.0f*((pRotation.W*pRotation.W) + (pRotation.Y*pRotation.Y)) - 1.0f;
					Result[2][1] = 2.0f*((pRotation.Y*pRotation.Z) - (pRotation.W*pRotation.X));
					Result[0][2] = 2.0f*((pRotation.X*pRotation.Z) - (pRotation.W*pRotation.Y));
					Result[1][2] = 2.0f*((pRotation.Y*pRotation.Z) + (pRotation.W*pRotation.X));
					Result[2][2] = 2.0f*((pRotation.W*pRotation.W) + (pRotation.Z*pRotation.Z)) - 1.0f;

					node.Transformation  = Result * node.Transformation;
				}

				if (i.find("translation") != i.end())
				{
					
					node.Transformation[3][0] = (float)i["translation"][0];
					node.Transformation[3][1] = (float)i["translation"][1];
					node.Transformation[3][2] = (float)i["translation"][2];

					//node.Transformation = glm::translate(node.Transformation, Translate);
				}

				nodes.push_back(node);
			}
		}

		catch (...)
		{
			__debugbreak();
		}

		return HYD_OK;
	}

	uint32 GLTFLoader::ProcessScene(json & pScene, const std::vector<Node>& nodes, Model* pCurrentModel)
	{
		if (pScene == nullptr)
			return HYD_CORRUPTED_GLTF;

		try
		{
			pCurrentModel->m_Name = pScene.value("name", "unamed");


			//Traversing the Tree in Preorder

			std::stack<Node> Stack;
			
			for (auto& i : pScene["nodes"])
			{
				Stack.push(nodes[uint32(i)]);
			}

			while (!Stack.empty())
			{
				Node Current = Stack.top();
				Stack.pop();

				if (Current.Mesh != -1)
				{
					pCurrentModel->m_Meshes[Current.Mesh].m_Transformation = Current.Transformation;
				}

				for (auto &i : Current.Children)
				{
					Node Child = nodes[i];
					Child.Transformation =  Current.Transformation * Child.Transformation;
					Stack.push(Child);
				}
			}

		}

		catch (...)
		{
			__debugbreak();
		}

		return HYD_OK;
	}
};