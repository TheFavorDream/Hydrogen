#include "../include/Xenon.h"


/*
	Xenon Vector Definition:
*/
namespace Xenon 
{

/*


	Model Class Implementation

*/



	Model::Model()
	{
		m_Buffers.reserve(3);
	}

	Model::~Model()
	{
		m_Buffers.clear();
	}


	Model::Model(const Model& pOther)
	{
		m_CopyRight    = pOther.m_CopyRight;
		m_Generator    = pOther.m_Generator;
		m_Version      = pOther.m_Version;


		//m_Buffers	   = pOther.m_Buffers;
		m_Scenes       = pOther.m_Scenes;
		m_DefaultScene = pOther.m_DefaultScene;
	}

	Model::Model(Model&& pOther)
	{
		m_CopyRight    = std::move(pOther.m_CopyRight);
		m_Generator    = std::move(pOther.m_Generator);
		m_Version      = std::move(pOther.m_Version);

		m_Scenes       = std::move(pOther.m_Scenes);
		m_Buffers	   = std::move(pOther.m_Buffers);
		m_DefaultScene = pOther.m_DefaultScene;

	}


	Model& Model::operator=(const Model& pOther)
	{
		if (&pOther != this)
		{
			m_CopyRight    = pOther.m_CopyRight;
			m_Generator    = pOther.m_Generator;
			m_Version      = pOther.m_Version;

			m_Scenes       = pOther.m_Scenes;
		//	m_Buffers      = pOther.m_Buffers;
			m_DefaultScene = pOther.m_DefaultScene;

		}
		return *this;
	}

	Model& Model::operator=(Model&& pOther)
	{
		if (&pOther != this)
		{
			m_CopyRight	   = std::move(pOther.m_CopyRight);
			m_Generator	   = std::move(pOther.m_Generator);
			m_Version	   = std::move(pOther.m_Version);

			m_Buffers      = std::move(pOther.m_Buffers);
			m_Scenes       = std::move(pOther.m_Scenes);
			m_DefaultScene = pOther.m_DefaultScene;
		}
		return *this;
	}

	uint32_t Model::Free()
	{
		m_Buffers.clear();
		m_Scenes.clear();

		int32_t      m_DefaultScene = 0;
		uint64_t	 m_BuffersCount = 0;

		m_Generator.clear();
		m_CopyRight.clear();
		m_Version.clear();

		return XE_OK;
	}

	const Scene& Model::operator[](uint64_t pSceneIndex) const
	{
		XE_ASSERT(pSceneIndex < m_Scenes.size(), "Out of Bound");
		return (*m_Scenes.at(pSceneIndex));
	}

	std::shared_ptr<BinaryData> Model::GetRawBinary(uint64_t pIndex) const
	{
		XE_ASSERT(pIndex < m_BuffersCount, "Out of Buffers bound");
		return m_Buffers[pIndex];
	}



	BinaryData BufferView::FetchData(uint64_t pOffset, uint64_t pElementSize, uint64_t pCount) const
	{
		if(ByteStride == 0)
			return std::move(Buffer->SubData(pOffset+ByteOffset, ByteLength));

		//Non-zero stride needs repacking:

		//Log::PushLog(LOG_INFO, "non-zero Stride, Repacking. Stride:%i \t Offset:%i", ByteStride, pOffset);

		BinaryData View(pElementSize*pCount, (void*)new uint8_t[pElementSize*pCount]);
		uint64 OffsetIn = pOffset + ByteOffset;

		for (uint64_t Iter = 0 ; Iter < pCount ; ++Iter)
		{
			memcpy((void*)(View.Ptr+(Iter*pElementSize)), (void*)(Buffer->Ptr+ OffsetIn), pElementSize);
			OffsetIn += ByteStride;
		}

		return std::move(View);
	}



#ifndef XE_STREAM_READ_IMPL

	void* StreamRead(const XE_STRING& pPath, uint64_t& pByteSize)
	{

		std::ifstream SourceFile(pPath, std::ios::binary);
		if (!SourceFile.is_open())
		{
			LOG(LOG_ERROR, "Cannot Open the file");
			pByteSize = 0;
			return nullptr;
		}

		//Retrive the size:
		SourceFile.seekg(0, SourceFile.end);
		pByteSize = SourceFile.tellg();
		SourceFile.seekg(0, SourceFile.beg);



		char* Data = new char[pByteSize];
		SourceFile.read(Data, pByteSize);

		SourceFile.close();
		return (void*)Data;
	}
#endif

#ifndef XE_STREAM_WRITE_IMPL
	XE_STRING StreamWrite(const XE_STRING& pPath, XE_STRING& Data)
	{
		return XE_STRING();
	}
#endif




/*

	Binary Data Implementation

*/


	BinaryData::BinaryData(uint64_t pLength, void* pData)
	{
		ByteLength = pLength;
		Ptr = new uint8_t[ByteLength];
		memcpy((void*)Ptr, pData, ByteLength);
	}

	BinaryData::~BinaryData()
	{
		Free();
	}

	BinaryData::BinaryData(BinaryData&& pOther)
	{
		Ptr         = pOther.Ptr;
		ByteLength  = pOther.ByteLength;


		pOther.Ptr         = nullptr;
		pOther.ByteLength  = 0;
	}

	BinaryData::BinaryData(const BinaryData& pOther)
	{
	
		if (pOther.Ptr != nullptr)
		{
			Ptr = new uint8_t[pOther.ByteLength]; //Allocate new memory

			//Copy
			memcpy((void*)Ptr, (void*)pOther.Ptr, pOther.ByteLength);

			ByteLength = pOther.ByteLength;

		}
	}

	BinaryData& BinaryData::operator=(BinaryData&& pOther)
	{
		if (&pOther == this)
			return *this;

		Ptr = pOther.Ptr;
		ByteLength  = pOther.ByteLength;



		pOther.Ptr         = nullptr;
		pOther.ByteLength  = 0;

		return *this;

	}

	
	BinaryData& BinaryData::operator=(const BinaryData& pOther)
	{
		if (&pOther == this)
			return *this;
		if (pOther.Ptr != nullptr)
		{
			Ptr = new uint8_t[pOther.ByteLength]; //Allocate new memory

			//Copy
			memcpy((void*)Ptr, (void*)pOther.Ptr, pOther.ByteLength);

			ByteLength  = pOther.ByteLength;

		}
		return *this;
	}

	BinaryData::operator void*()
	{
		return (void*)(Ptr);
	}

	void BinaryData::Copy(const BinaryData& pData, uint64_t pOffset, int64_t pByteLength) 
	{
		Free(); //Free previously allocated memory if any

		ByteLength = (pByteLength > -1) ? pByteLength : pData.ByteLength;
		Ptr        = new uint8_t[ByteLength];

		memcpy((void*)Ptr, (void*)pData[pOffset], ByteLength);

	}
	
	uint8_t* BinaryData::operator[](uint64_t Index) const
	{
		XE_ASSERT("Out Of Bound", (Index >= ByteLength));
		return Ptr + Index;
	}

	void BinaryData::Free() 
	{
		delete[] Ptr;
		ByteLength = 0;
		Ptr = nullptr;
	}


	BinaryData BinaryData::SubData(uint64_t pOffset, uint64_t pByteLength) const
	{

		XE_ASSERT("Invalid offset value", (pOffset >= pByteLength));
		XE_ASSERT("Out of Bound", (pOffset >= ByteLength) || (pByteLength > ByteLength));

		BinaryData Chunk;
		Chunk.ByteLength = (pByteLength != 0)? pByteLength : Chunk.ByteLength;

		if (Chunk.ByteLength == 0)
			return std::move(Chunk);

		Chunk.Ptr = new uint8_t[Chunk.ByteLength];

		memcpy((void*)Chunk.Ptr, (void*)(Ptr+pOffset), Chunk.ByteLength);

		return std::move(Chunk);
	}






	/*
	
		Accessor Implementation:
	
	*/

	uint32_t Accessor::RetriveTypeSize(DataType pEnum)
	{
		switch (pEnum)
		{
		case XE_SIGNED_BYTE:
			return 1;
		case XE_UNSIGNED_BYTE:
			return 1;
		case XE_SIGNED_SHORT:
			return 2;
		case XE_UNSIGNED_SHORT:
			return 2;
		case XE_UNSIGNED_INT:
			return 4;
		case XE_FLOAT:
			return 4;
		}
		return XE_UNSIGNED_BYTE;
	}

	BinaryData Accessor::RetriveData() const
	{
		return std::move(Data.FetchData(Offset, Accessor::RetriveTypeSize(ComponentType)*(uint64_t)Type, Count));
	}

	ArrayType Accessor::RetriveArrayTypeFromString(const std::string_view& pType)
	{
		if (pType == "SCALAR")
			return XE_TYPE_SCALER;
		else if (pType == "VEC2")
			return XE_TYPE_VEC2;
		else if (pType == "VEC3")
			return XE_TYPE_VEC3;
		else if (pType == "VEC4")
			return XE_TYPE_VEC4;
		else if (pType == "MAT2")
			return XE_TYPE_MAT2;
		else if (pType == "MAT3")
			return XE_TYPE_MAT3;
		else if (pType == "MAT4")
			return XE_TYPE_MAT4;

		return XE_TYPE_SCALER;
	}

	uint32 Accessor::GetVulkanIndexTypeEnum(DataType ComponentType) noexcept
	{
		switch (ComponentType)
		{
			case XE_UNSIGNED_BYTE:
			 return 1000265000;
			case XE_UNSIGNED_SHORT:
				return 0;
			case XE_UNSIGNED_INT:
				return 1;
			default:
				return 0x7FFFFFFF;
		}
		return 0x7FFFFFFF;
	}


	/*
		Material:
	*/

	Material::Material(const Material& pOther)
	{
		m_Name              = pOther.m_Name;
		m_Alpha             = pOther.m_Alpha;
		m_AlphaCutoff       = pOther.m_AlphaCutoff;
		m_BaseColorFactor   = pOther.m_BaseColorFactor;
		m_DoubleSided       = pOther.m_DoubleSided;
		m_EmissiveFactor    = pOther.m_EmissiveFactor;
		m_MetallicFactor    = pOther.m_MetallicFactor;
		m_OcclusionStrength = pOther.m_OcclusionStrength;
		m_RoughnessFactor   = pOther.m_RoughnessFactor;
		m_TextureIndices    = pOther.m_TextureIndices;
	}



	Material& Material::operator=(const Material& pOther)
	{
		if (&pOther == this)
			return *this;


		m_Name				= pOther.m_Name;
		m_Alpha				= pOther.m_Alpha;
		m_AlphaCutoff		= pOther.m_AlphaCutoff;
		m_BaseColorFactor	= pOther.m_BaseColorFactor;
		m_DoubleSided		= pOther.m_DoubleSided;
		m_EmissiveFactor    = pOther.m_EmissiveFactor;
		m_MetallicFactor    = pOther.m_MetallicFactor;
		m_OcclusionStrength = pOther.m_OcclusionStrength;
		m_RoughnessFactor   = pOther.m_RoughnessFactor;
		m_TextureIndices    = pOther.m_TextureIndices;

		return *this;
	}


	/*
		Primitive:
	
	*/

	Primitive::Primitive(const Primitive& pOther)
	{
		m_Accessors	 = pOther.m_Accessors;
		m_Indices	 = pOther.m_Indices;
		m_Material   = pOther.m_Material;
		m_RenderMode = pOther.m_RenderMode;
	}	

	Primitive::Primitive(Primitive&& pOther)
	{
		m_Accessors  = std::move(pOther.m_Accessors);
		m_Indices	 = std::move(pOther.m_Indices);
		m_Material   = pOther.m_Material;
		m_RenderMode = pOther.m_RenderMode;
	}

	Primitive& Primitive::operator=(const Primitive& pOther)
	{
		if (&pOther == this)
			return *this;

		m_Accessors  = std::move(pOther.m_Accessors);
		m_Indices	 = pOther.m_Indices;
		m_Material   = pOther.m_Material;
		m_RenderMode = pOther.m_RenderMode;
		return *this;
	}

	Primitive& Primitive::operator=(Primitive&& pOther)
	{
		if (&pOther == this)
			return *this;

		m_Accessors  = std::move(pOther.m_Accessors);
		m_Indices	 = std::move(pOther.m_Indices);
		m_Material   = pOther.m_Material;
		m_RenderMode = pOther.m_RenderMode;
		return *this;
	}

	uint64_t Primitive::ComputeVertexDataSize() const
	{
		uint64_t Sum = 0;

		for (auto& access : m_Accessors)
		{
			Sum += (access.Count*Accessor::RetriveTypeSize(access.ComponentType)*(uint64_t)access.Type);
		}

		return Sum;
	}


	/*
		Mesh Implementation:
	*/

	Mesh::~Mesh()
	{
		m_Primitives.clear();
		m_Name.clear();
	}

	Mesh::Mesh(Mesh&& pOther)
	{
		m_Primitives = std::move(pOther.m_Primitives);
		m_Name       = std::move(pOther.m_Name);
	}

	Mesh::Mesh(const Mesh& pOther)
	{
		m_Name       = pOther.m_Name;
		m_Primitives = pOther.m_Primitives;
	}

	Mesh& Mesh::operator=(Mesh&& pOther)
	{
		m_Primitives = std::move(pOther.m_Primitives);
		m_Name       = std::move(pOther.m_Name);
		return *this;
	}

	Mesh& Mesh::operator=(const Mesh& pOther)
	{
		m_Name       = pOther.m_Name;
		m_Primitives = pOther.m_Primitives;
		return *this;
	}

	const Primitive& Mesh::operator[](uint64_t pIndex) const
	{
		XE_ASSERT(pIndex < m_Primitives.size(), "Out of Primitive Range");
		return m_Primitives.at(pIndex);
	}



	/*
		Node Implementation:
	
	*/

	Node::Node(Node&& pOther)
	{
		m_Childern  = std::move(pOther.m_Childern);
		m_Name		= std::move(pOther.m_Name);
		m_Mesh		= std::move(pOther.m_Mesh);
		m_Camera	= pOther.m_Camera;

		m_Depth		= pOther.m_Depth;
		pOther.m_Depth = 0;

		m_Translation    = pOther.m_Translation;
		m_Rotation	     = pOther.m_Rotation;
		m_Scale		     = pOther.m_Scale;

		m_LocalTransform = pOther.m_LocalTransform;
		m_PointerToRoot  = pOther.m_PointerToRoot;
		pOther.m_PointerToRoot = nullptr;
	}

	Node::Node(const Node& pOther)
	{
		m_Childern = pOther.m_Childern;

		m_Name   = pOther.m_Name;
		m_Mesh   = pOther.m_Mesh;
		m_Camera = pOther.m_Camera;
		m_Depth = pOther.m_Depth;

		m_Translation = pOther.m_Translation;
		m_Rotation    = pOther.m_Rotation;
		m_Scale	      = pOther.m_Scale;

		m_LocalTransform = pOther.m_LocalTransform;
		m_PointerToRoot  = pOther.m_PointerToRoot;

	}

	Node& Node::operator=(Node&& pOther)
	{
		m_Childern  = std::move(pOther.m_Childern);
		m_Name		= std::move(pOther.m_Name);
		m_Mesh		= std::move(pOther.m_Mesh);
		m_Camera	= pOther.m_Camera;

		m_Depth = pOther.m_Depth;
		pOther.m_Depth = 0;

		m_Translation = pOther.m_Translation;
		m_Rotation    = pOther.m_Rotation;
		m_Scale       = pOther.m_Scale;

		m_LocalTransform = pOther.m_LocalTransform;
		m_PointerToRoot  = pOther.m_PointerToRoot;
		pOther.m_PointerToRoot = nullptr;
		return *this;
	}

	Node& Node::operator=(const Node& pOther)
	{
		m_Childern = pOther.m_Childern;

		m_Name   = pOther.m_Name;
		m_Mesh   = pOther.m_Mesh;
		m_Camera = pOther.m_Camera;
		m_Depth = pOther.m_Depth;

		m_Translation = pOther.m_Translation;
		m_Rotation    = pOther.m_Rotation;
		m_Scale       = pOther.m_Scale;

		m_LocalTransform = pOther.m_LocalTransform;
		return *this;
	}

	const Node& Node::operator[](size_t pIndex) const
	{
		XE_ASSERT(pIndex < m_Childern.size(), "Out of Node's Children bound");
		return m_Childern.at(pIndex);
	}

	const Mesh& Node::GetMesh() const
	{
		return m_PointerToRoot->GetMesh(m_Mesh);
	}



	void Node::RetriveTRS(XE_VEC3* pTranslate, XE_VEC4* pRotation, XE_VEC3* pScale)
	{
		if (pTranslate)
			*pTranslate = m_Translation;
		if (pRotation)
			*pRotation  = m_Rotation;
		if (pScale)
			*pScale     = m_Scale;
	}

	Scene::Scene(Scene&& pOther)
	{
		m_Name   = std::move(pOther.m_Name);
		m_Nodes  = std::move(pOther.m_Nodes);
		m_Meshes = std::move(pOther.m_Meshes);
	}

	Scene::Scene(const Scene& pOther)
	{
		m_Name   = pOther.m_Name;
		m_Nodes  = pOther.m_Nodes;
		m_Meshes = pOther.m_Meshes;

	}

	Scene& Scene::operator=(Scene&& pOther)
	{
		if (&pOther == this)
			return *this;

		m_Name   = std::move(pOther.m_Name);
		m_Nodes  = std::move(pOther.m_Nodes);
		m_Meshes = std::move(pOther.m_Meshes);

		return *this;
	}

	Scene& Scene::operator=(const Scene& pOther)
	{
		if (&pOther == this)
			return *this;
		m_Name   = pOther.m_Name;
		m_Nodes  = pOther.m_Nodes;
		m_Meshes = pOther.m_Meshes;
		return *this;

	}

	const Node& Scene::operator[](size_t pIndex) const
	{
		XE_ASSERT(pIndex >= m_Nodes.size(), ("Out of Nodes Vector Range"));
		return m_Nodes.at(pIndex);
	}

	inline Mesh& Scene::GetMesh(uint64_t pMeshIndex)
	{
		XE_ASSERT(pMeshIndex < m_Meshes.size(), "Out of meshes range");
		return m_Meshes.at(pMeshIndex);
	}


	BinaryData Texture::RetriveImageData() const
	{
		return BinaryData(ImageData.Buffer->ByteLength, ImageData.Buffer->Ptr);
	}

	uint32 Sampler::VulkanFilterEnum(TextureFilter pFilter) noexcept
	{

		switch (pFilter)
		{
		case NEAREST:
			return 0;
		case LINEAR:
			return 1;
		case NEAREST_MIPMAP_NEAREST: 
			return 0;
		case LINEAR_MIPMAP_NEAREST:
			return 1;
		case NEAREST_MIPMAP_LINEAR:
			return 0;
		case LINEAR_MIPMAP_LINEAR:  
			return 1;
		}

		return 0x7FFFFFFF; //No Valid filter
	}

	uint32 Sampler::VulkanMipmapFilterEnum(TextureFilter pFilter)   noexcept
	{
		switch (pFilter)
		{
		case NEAREST:
			return 0x7FFFFFFF;
		case LINEAR:
			return 0x7FFFFFFF;
		case NEAREST_MIPMAP_NEAREST: 
			return 0;
		case LINEAR_MIPMAP_NEAREST:
			return 0;
		case NEAREST_MIPMAP_LINEAR:
			return 1;
		case LINEAR_MIPMAP_LINEAR:  
			return 1;
		}
		return 0x7FFFFFFF;
	}


	uint32 Sampler::VulkanWrapEnum(TextureWrap pWrapMdoe) noexcept
	{

		switch(pWrapMdoe)
		{
			case REPEAT:	
				return 0;
			case MIRRORED_REPEAT: 
				return 1;
			case CLAMP_TO_EDGE: 
				return 2;
		}

		return 0x7FFFFFFF;
	};

};


