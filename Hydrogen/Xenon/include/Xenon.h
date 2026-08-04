/*

	Xenon - 2026
	By: Pouya Alizadeh

	Introduction:
		Xenon is a fast, easy to use, 3D Model loader.
		Formats supported: gltf, glb

		Xenon is NOT a rendering engine, only a loader
*/

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <unordered_map>
#include <stack>
#include <queue>
#include <memory>
#include <set>
#include <array>
#include <optional>
#include <cstdint>
#include <cstring>
#include "Config.h"

#ifndef DIST
	//#include "3rdParty/Hydrofiler/Hydrofiler.h"
#endif


#ifndef XE_VEC2_IMPL
	#define XE_VEC2  Xenon::Vec2<float>
#else
	#define XE_VEC2 XE_VEC2_IMPL
#endif

#ifndef XE_VEC3_IMPL
	#define XE_VEC3  Xenon::Vec3<float>
#else
	#define XE_VEC3  XE_VEC3_IMPL
#endif

#ifndef XE_VEC4_IMPL
	#define XE_VEC4  Xenon::Vec4<float>
#else
	#define XE_VEC4  XE_VEC4_IMPL
#endif

#ifndef XE_MAT4_IMPL
	#define XE_MAT4  Xenon::Mat4<float>
#else
	#define XE_MAT4  XE_MAT4_IMPL
#endif



/*
	Xenon Internal Assertions and logging
*/
#ifndef XE_LOG_ASSERT_IMPL
	#define XE_ASSERT(x, a) assert(x)
#else
	#define XE_ASSERT(x, a) XE_LOG_ASSERT_IMPL(x, a)
#endif

#include "Vectors.h"


/*
	Default Data Structures
	maybe overwritten by the user

*/

#ifdef XE_STRUCTURE_VECTOR
	#define XE_VECTOR XE_STRUCTURE_VECTOR
#else
	#define XE_VECTOR std::vector
#endif

#ifdef XE_STRUCTURE_MAP
	#define XE_MAP XE_STRUCTURE_MAP
#else
	#define XE_MAP std::unordered_map
#endif

#ifdef XE_STRUCTURE_STRING
	#define XE_STRING XE_STRUCTURE_STRING
#else
	#define XE_STRING std::string
#endif

/*

	Default Streaming Functions
	maybe overwritten by user

*/

#ifdef XE_STREAM_READ_IMPL
	#define XE_STREAM_READ(Path) XE_STREAM_READ_IMPL(Path)
#else
	#define XE_STREAM_READ(Path, Bytes) Xenon::StreamRead(Path, Bytes);
#endif

#ifdef XE_STREAM_WRITE_IMPL
	#define XE_STREAM_WRITE(Path, Data) XE_STREAM_READ_IMPL(Path, Data)
#else
	#define XE_STREAM_WRITE(Path, Data) Xenon::StreamWrite(Path, Data);
#endif



#define XE_OK				0x00
#define XE_NO_DEFAULT_SCENE 0x01

//Xenon namespace used for all Xenon related functions and structures
namespace Xenon
{

	enum LoadFlags_ {
		//Material loading settings
		LF_DEFAULT_MATERIAL  = 1 * 1, // Default state (Loads everything)
		LF_NO_MATERIAL       = 2 * 1, // Uses the engines default material for all primitives (all primitives share a single material)
		LF_BASE_COLOR_ONLY   = 3 * 1, // Only loads in base color textures and ignores others
		LF_BASE_NORMAL_ONLY  = 4 * 1, // Only loads in base color and normals maps and ignores others 

		//Transform settings
		LF_DEFAULT_TRANSFORM = 1 * 10, // Default Transformations.
		LF_NO_TRANSFORM      = 2 * 10, // Ignores All transformations.
		LF_IGNORE_MATRIX     = 3 * 10, // Only ignores supplied matrices.
		LF_IGNORE_TRS        = 4 * 10, // Only ignores supplied TRS values.


		LF_DEFAULT           = LF_DEFAULT_MATERIAL + LF_DEFAULT_TRANSFORM
	};

	/*
		Supported Formats:
	*/

	enum FILE_FORMAT {FORMAT_GLTF, FORMAT_GLB, FORMAT_INVALID};


	enum BinaryTarget {
		XE_BIN_UNSPECIFIED,
		XE_BIN_IMAGE,
		XE_BIN_GEOMETRY,
		XE_BIN_MISC
	};

	enum ArrayType {
		XE_TYPE_UNSPECIFIED = 0,
		XE_TYPE_SCALER = 1,
		XE_TYPE_VEC2 = 2,
		XE_TYPE_VEC3 = 3,
		XE_TYPE_VEC4 = 4,
		XE_TYPE_MAT2 = 4,
		XE_TYPE_MAT3 = 9,
		XE_TYPE_MAT4 = 16
	};

	enum DataType {
		XE_SIGNED_BYTE    = 5120,
		XE_UNSIGNED_BYTE  = 5121,
		XE_SIGNED_SHORT   = 5122,
		XE_UNSIGNED_SHORT = 5123,
		XE_UNSIGNED_INT   = 5125,
		XE_FLOAT          = 5126
	};


	enum Attributes
	{
		XE_POSITION	   = 0,
		XE_NORMALS     = 1,
		XE_TANGENT     = 2,
		XE_TEXCOORDS_0 = 3,
		XE_TEXCOORDS_1 = 4,
		XE_COLOR_0     = 5,
		XE_INDEX	   = 6,
		XE_EXTRA       = 7
	};

	enum TextureWrap {
		CLAMP_TO_EDGE   = 33071,
		MIRRORED_REPEAT = 33648,
		REPEAT			= 10497
	};

	enum TextureFilter {
		NEAREST					= 9728,
		LINEAR					= 9729,
		NEAREST_MIPMAP_NEAREST  = 9984,
		LINEAR_MIPMAP_NEAREST   = 9985,
		NEAREST_MIPMAP_LINEAR   = 9986,
		LINEAR_MIPMAP_LINEAR    = 9987
	};


	enum AlphaMode{
		XE_OPAQUE,
		XE_MASK,
		XE_BLEND
	};

	enum CameraType { 
		XE_CAMERA_UNDEFINED,
		XE_CAMERA_PRESPECTIVE,
		XE_CAMERA_ORTHOGONAL 
	};

	enum MimeType{
		XE_IMG_UNSPECIFIED,
		XE_IMG_PNG,
		XE_IMG_JPG,
		XE_IMG_GIF,
		XE_IMG_BMP,
		XE_IMG_UNKNOWN
	};

	enum LogLevel{
		LOG_DISABLE=0,
		LV1,
		LV2,
		LV3
	};

	enum LogKind{
		LOG_ERROR,
		LOG_WARNING,
		LOG_INFO 
	};





#ifndef XE_STREAM_READ_IMPL
	void* StreamRead(const XE_STRING& pPath, uint64_t& pByteSize);
#endif

#ifndef XE_STREAM_WRITE_IMPL
	XE_STRING StreamWrite(const XE_STRING& pPath, XE_STRING& Data);
#endif



	class Log
	{
	public:

		struct LogMessage
		{
			XE_STRING Message;
			LogKind   Kind;
		};
	public:

		//For Client Use:

		//Returns the current unretrived logs in the queue:
		static uint64_t  GetLogCount();
		//Returns the first log in queue:
		static LogMessage GetLastLog();

		static void SetLogLevel(LogLevel pLevel);
		//Pushes a new log to queue
		static void PushLog(LogKind pKind, XE_STRING pString, ...);

		static void SetLogCallBack(void(*pLogCallBack)(const LogMessage& pMessage));

	private:

		static void(*s_LogCallBack)(const LogMessage& pMessage);
		static std::queue<LogMessage> s_Logs;
		static LogLevel               s_Level;
		static uint64_t				  s_LogCount;

		friend class Loader;
	};

#ifndef XE_SHUTUP
	#define LogCount() Xenon::Log::GetLogCount()
	#define GetLog()   Xenon::Log::GetLastLog()
#else
	#define LogCount() 0
	#define GetLog()   Xenon::Log::GetLastLog()
#endif

#define LOG Log::PushLog



	/*
		the Core structures for gltf models
	*/

	//Forward-Declaration
	class  Loader;
	class  Model;
	class  Scene;
	class  Node;
	class  Mesh;
	class  Primitive;
	class  Material;
	struct Camera;
	struct BinaryData;
	struct BufferView;
	struct Accessor;
	struct Image;
	struct Sampler;
	struct Texture;


	/*
		Buffer Storage:
	*/

	struct BinaryData
	{

		BinaryData() = default;
		BinaryData(uint64_t pLength, void* pData);
	   ~BinaryData();

	   //Move & Copy
		BinaryData(BinaryData&& pOther);
		BinaryData(const BinaryData& pOther);

		BinaryData& operator=(BinaryData&& pOther);
		BinaryData& operator=(const BinaryData& pOther);

		operator void*();

		void Copy(const BinaryData& pData, uint64_t pOffset = 0, int64_t pByteLength = -1);

		/*
			Lets you to fetch bytes within the Data
		*/
		uint8_t* operator[](uint64_t Index) const;

		/*
			Free's the allocated memory
		*/

		void Free();

		/*
			Makes a BinaryData object and copies the specified chunk of data to it.
		*/

		BinaryData SubData(uint64_t pOffset, uint64_t pByteLength=0) const;


		uint8_t*  Ptr         = nullptr;
		uint64_t  ByteLength  = 0;
	};


	/*

		Buffer View:
		A view into a buffer generally representing a subset of the buffer.

	*/

	struct BufferView
	{
		std::shared_ptr<BinaryData> Buffer; //The Buffer We're pointing to 
		size_t		ByteLength = 0; //The Length of the data
		size_t		ByteStride = 0; //Space between data
		size_t		ByteOffset = 0; //Start Offset in buffer

		//Retrives the specified data from buffer
		BinaryData FetchData(uint64_t pOffset, uint64_t pElementSize, uint64_t pCount) const;

		//Specifies what this binary data is used for
		BinaryTarget Target = XE_BIN_UNSPECIFIED;
	};


	/*
		Accessors:
		An accessor defines a method for retrieving data as typed arrays from within a buffer view
	*/

	struct Accessor
	{
		static ArrayType RetriveArrayTypeFromString(const std::string_view& pType);
		static uint32_t  RetriveTypeSize(DataType pEnum);


		BinaryData RetriveData() const; 

		uint64_t   Count  = 0;
		uint64_t   Offset = 0;
		bool       Normalized = false;
		ArrayType  Type = XE_TYPE_UNSPECIFIED;
		DataType   ComponentType;
		Attributes Kind;

		inline uint64_t ElementSize() const { return Accessor::RetriveTypeSize(ComponentType)*(uint64_t)Type; }
	private:
		BufferView Data;

		friend class Loader;
	};


	/*
		Sampler Objects specifiy how a texture should be sampled
	*/
	struct Sampler
	{
		//Wrapping mode of texture. Repeat by default
		TextureWrap WrapS = REPEAT; 
		TextureWrap WrapT = REPEAT;

		TextureFilter Mag = LINEAR;
		TextureFilter Min = LINEAR;
	};



	/*
		Image info required to load Textures
	*/

	struct ImageInfo
	{
		XE_STRING  URI;
		MimeType   ImageMime;
		BufferView ImageBV;
		Sampler    ImageSampler;

	};





	/*
		Acuall Texture 
	*/

	struct Texture
	{
		BinaryData    RetriveImageData() const;
		BufferView    ImageData;
		Sampler       TextureSample;
		uint16_t      TexCoordSet = 0; // Default TEXCOORD_0
		MimeType	  ImageMime = XE_IMG_UNSPECIFIED;
	};


	class Material
	{
	public:


		Material() = default;
	   ~Material() = default;

	    Material(const Material& pOther);

		Material& operator=(const Material& pOther);


		inline std::array<std::optional<uint64_t>, 5>::iterator begin() const { return m_TextureIndices.begin(); }
		inline std::array<std::optional<uint64_t>, 5>::iterator end()   const { return m_TextureIndices.end(); }


		inline bool HasBaseColor()   const { return m_BaseColorTexture.has_value(); }
		inline bool HasNormalMap()   const { return m_Normal.has_value(); }
		inline bool HasMetallicMap() const { return m_MetallicRoughnessTexture.has_value(); }
		inline bool HasOclusionMap() const { return m_Occlusion.has_value(); }
		inline bool HasEmissiveMap() const { return m_Emissive.has_value(); }


		//Getters:
		
		inline const XE_STRING& GetName()      const { return m_Name; }
		inline const std::optional<uint64> GetBaseColor()   const { return m_BaseColorTexture; }
		inline const std::optional<uint64> GetNormalMap()   const { return m_Normal; }
		inline const std::optional<uint64> GetMetallicMap() const { return m_MetallicRoughnessTexture; }
		inline const std::optional<uint64> GetOclusionMap() const { return m_Occlusion; }
		inline const std::optional<uint64> GetEmissiveMap() const { return m_Emissive; }

		inline  const XE_VEC4  GetBaseColorFactor()   const { return m_BaseColorFactor; }
		inline  const XE_VEC3  GetEmissiveFactor()    const { return m_EmissiveFactor; }
		inline  const float    GetMetallicFactor()    const { return m_MetallicFactor; }
		inline  const float    GetRoughnessFactor()   const { return m_RoughnessFactor; }
		inline  const float	   GetOcclusionStrength() const { return m_OcclusionStrength; }
		inline  const float    GetAlphaCutoff()	      const { return m_AlphaCutoff; }
		inline  const bool	   GetDoubleSided()	      const { return m_DoubleSided; }

	private:
		XE_STRING			  m_Name = "Unnamed Material";

		mutable std::array<std::optional<uint64_t>, 5> m_TextureIndices;

		//Textures
		std::optional<uint64_t>& m_BaseColorTexture			= m_TextureIndices[0];
		std::optional<uint64_t>& m_Occlusion				= m_TextureIndices[1];
		std::optional<uint64_t>& m_MetallicRoughnessTexture = m_TextureIndices[2];
		std::optional<uint64_t>& m_Normal					= m_TextureIndices[3];
		std::optional<uint64_t>& m_Emissive					= m_TextureIndices[4];
		AlphaMode			    m_Alpha;

		XE_VEC4         m_BaseColorFactor     = XE_VEC4(1.0f);
		XE_VEC3		    m_EmissiveFactor	  = XE_VEC3(0.0f);
		float           m_MetallicFactor      = 1.0f;
		float           m_RoughnessFactor     = 1.0f;
		float			m_OcclusionStrength   = 1.0;
		float           m_AlphaCutoff         = 0.5f;
		bool			m_DoubleSided         = false;

		friend class Loader;

	};

/*

	Primitive:
		the actual renderable object in the scene.
		stores properties such as:
			Material
			Primitive Type
			Vertex Data
			Index  Data

*/

	class Primitive 
	{
	public:

		Primitive() = default;

		Primitive(const Primitive& pOther);
		Primitive(Primitive&& pOther);

		Primitive& operator=(const Primitive& pOther);
		Primitive& operator=(Primitive&& pOther);


		inline std::array<Accessor, 6>::iterator begin() { return m_Accessors.begin(); }
		inline std::array<Accessor, 6>::iterator end()   { return m_Accessors.end(); }


		uint64_t ComputeVertexDataSize() const;

		//Getter Functions

		inline uint64_t GetTopology() { return m_RenderMode; }

		inline bool HasPosition()   const { return (m_Positions.Type  != XE_TYPE_UNSPECIFIED); }
		inline bool HasNormal()	    const { return (m_Normals.Type    != XE_TYPE_UNSPECIFIED); }
		inline bool HasTangent()    const { return (m_Tangents.Type   != XE_TYPE_UNSPECIFIED); }
		inline bool HasColor()		const { return (m_Color_0.Type    != XE_TYPE_UNSPECIFIED); }
		inline bool HasTexCoord_0() const { return (m_TexCoord_0.Type != XE_TYPE_UNSPECIFIED); }
		inline bool HasTexCoord_1() const { return (m_TexCoord_0.Type != XE_TYPE_UNSPECIFIED); }
		inline bool HasIndices()	const { return (m_Indices.Type    != XE_TYPE_UNSPECIFIED); }
		
		
		const Accessor& GetPosition()	const {return m_Positions;}
		const Accessor& GetNormal()		const {return m_Normals;}
		const Accessor& GetTangent()	const {return m_Tangents;}
		const Accessor& GetTexCoord_0() const {return m_TexCoord_0;}
		const Accessor& GetTexCoord_1() const {return m_TexCoord_1;}
		const Accessor& GetColor()      const {return m_Color_0;}
		const Accessor& GetIndices()	const {return m_Indices;}

		const Material& GetMatrial()   const { return m_Material; }

	private:

		mutable std::array<Accessor, 6> m_Accessors;

		Accessor& m_Positions  = m_Accessors[0]; 
		Accessor& m_Normals    = m_Accessors[1];
		Accessor& m_Tangents   = m_Accessors[2];
		Accessor& m_TexCoord_0 = m_Accessors[3];
		Accessor& m_TexCoord_1 = m_Accessors[4];
		Accessor& m_Color_0    = m_Accessors[5];

		Accessor m_Indices;

		Material m_Material;

		uint16_t m_RenderMode = 4; //Triangles by Default
		
		friend class Loader;

	};
/*

	Mesh:
		Stores the properties of a mesh such as:
			Primitives
			TRS (Translation, Rotation, Scale)
		TRS is the result of combining parantal node transformations.

*/

	class Mesh
	{
	public:
		 Mesh() = default;
		~Mesh();

		Mesh(Mesh&& pOther);
		Mesh(const Mesh& pOther);

		Mesh& operator=(Mesh&& pOther);
		Mesh& operator=(const Mesh& pOther);

		//Iterator over primitives:

		inline std::vector<Primitive>::iterator begin() const { return m_Primitives.begin(); }
		inline std::vector<Primitive>::iterator end()   const { return m_Primitives.end(); }

		//Primitive Access:
		const Primitive&  operator[](uint64_t pIndex) const;


		inline       std::string GetName()    const { return m_Name; }
		inline       size_t      PrimitivesCount() const { return m_Primitives.size(); }

	private:

		XE_STRING			 m_Name;
		mutable XE_VECTOR<Primitive> m_Primitives;

		friend class Loader;
	};


/*
	Camera:
		Stores the information required to define a camera in 3D. such as:
			Projection Matrix (Most be Orthogonal or prespective)
*/

	struct Camera 
	{
		union
		{
			float Xmag = 0.0f;
			float AspectRatio;
		};

		union
		{
			float Ymag = 0.0f;
			float Yfov;
		};


			float Zfar;
			float Znear;
		
		CameraType Type = XE_CAMERA_UNDEFINED;
	};


	/*

		Node:
			Storage for meshes & trs properties

	*/

	class Node
	{
	public:

		Node() = default;

		Node(Node&& pOther);
		Node(const Node& pOther);

		Node& operator=(Node&& pOther);
		Node& operator=(const Node& pOther);

		//Iterators
		inline std::vector<Node>::iterator begin() const { return m_Childern.begin(); }
		inline std::vector<Node>::iterator end()   const { return m_Childern.end(); }

		//Returns a child of this node
		const Node&  operator[](size_t pIndex) const;

		inline const XE_STRING GetName()   const { return m_Name; }
		const Mesh& GetMesh() const;
		inline const uint64_t  GetMeshIndex() const { return m_Mesh; }
		inline const Camera&   GetCamera()    const { return m_Camera; }
		inline const uint64_t  GetDepth()     const { return m_Depth; }

		inline uint64_t ChildCount() { return m_Childern.size(); }

		inline bool IsMeshEmpty()   const { return (m_Mesh < 0); }
		inline bool IsCameraEmpty() const { return (m_Camera.Type == XE_CAMERA_UNDEFINED); }

		//Transformation:

		void RetriveTRS(XE_VEC3* pTranslate, XE_VEC4* pRotation, XE_VEC3* pScale);
		
		inline XE_VEC3 Translation() { return m_Translation; }
		inline XE_VEC4 Rotation()    { return m_Rotation; }
		inline XE_VEC3 Scale()		 { return m_Scale; }

		inline XE_MAT4 Matrix()      { return m_LocalTransform; }

	private:
		mutable XE_VECTOR<Node>     m_Childern;
		XE_STRING			m_Name;

		uint64_t m_Depth = 0; // Show the depth of this node in tree

		int64_t   m_Mesh = -1; //An index into scene::m_meshes, -1 means no mesh has been setted
		Camera    m_Camera;

		//TRS
		XE_VEC4 m_Rotation       = XE_VEC4(0.0f, 0.0f, 0.0f, 1.0f);
		XE_VEC3 m_Translation    = XE_VEC3(0.0f);
		XE_VEC3 m_Scale          = XE_VEC3(1.0f);

		//Matrix
		XE_MAT4 m_LocalTransform = XE_MAT4(1.0f);

		std::shared_ptr<Scene> m_PointerToRoot = nullptr;

		friend class Loader;
	};


/*

	Scene:
		Stores Nodes
*/

	class Scene
	{
	public:

		Scene() = default;

		Scene(Scene&& pOther);
		Scene(const Scene&  pOther);

		Scene& operator=(Scene&& pOther);
		Scene& operator=(const Scene&  pOther);

		
		//Iterator for nodes:
		inline std::vector<Node>::iterator begin() const { return m_Nodes.begin(); }
		inline std::vector<Node>::iterator end()   const { return m_Nodes.end(); }

		//Return a refrence to a node
		const Node&  operator[](size_t pIndex) const;

		inline std::string GetName()   { return m_Name; }
		inline size_t      NodeCount() { return m_Nodes.size(); }

		inline Mesh& GetMesh(uint64_t pMeshIndex);
		inline const XE_VECTOR<Mesh>&           GetMeshes()   const { return m_Meshes; }
		inline const XE_MAP<uint64_t, Texture>& GetTextures() const { return m_Textures; }


	private:
		XE_STRING                 m_Name;
		mutable XE_VECTOR<Node>   m_Nodes;
		XE_VECTOR<Mesh>			  m_Meshes;
		XE_MAP<uint64_t,Texture>  m_Textures;
		friend class Loader;
	};



	/*

		Model:
			The Entirity of a gltf file. stores properties such as:
				Scenes
				Default scene
				Asset info

	*/
	class Model
	{
	public:

		Model();
		~Model();

		Model(const Model& pOther);
		Model(Model&& pOther);

		Model& operator=(const Model& pOther);
		Model& operator=(Model&& pOther);

		uint32_t Free();

		//iterator for scenes
		inline XE_VECTOR<std::shared_ptr<Scene>>::iterator begin() { return m_Scenes.begin(); }
		inline XE_VECTOR<std::shared_ptr<Scene>>::iterator end()   { return m_Scenes.end(); }

		//Returns a refrence to scene in model
		const Scene& operator[](uint64_t pSceneIndex) const;

		inline std::string Generator() { return m_Generator; }
		inline std::string CopyRight() { return m_CopyRight; }
		inline std::string Version()   { return m_Version; }

		inline int64_t     DefaultScene()   { return m_DefaultScene; }
		inline uint64_t    GetBufferCount() { return m_BuffersCount; }

		 std::shared_ptr<BinaryData> GetRawBinary(uint64_t pIndex) const;


	private:
		//Meta Data About file
		XE_STRING m_Generator;
		XE_STRING m_CopyRight;
		XE_STRING m_Version;


		int32_t      m_DefaultScene = 0;
		uint64_t	 m_BuffersCount = 0;

		//Stores the available scenes
		XE_VECTOR<std::shared_ptr<Scene>>		m_Scenes;
		XE_VECTOR<std::shared_ptr<BinaryData>>  m_Buffers;

		friend class Loader;
	};
};

