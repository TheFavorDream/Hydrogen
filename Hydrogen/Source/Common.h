#pragma once




#ifdef HYD_DLL_BUILD
	#define HYD _declspec(dllexport)
#else
	#define HYD _declspec(dllimport)
#endif

//#define HYD

#define HYD_OK		     0x00		
#define HYD_INVALID_FILE 0x01
#define HYD_INVALID_PATH 0x02
#define HYD_INVALID_GLTF 0x03
#define HYD_INVALID_BIN  0x04
#define HYD_INVALID_GLB  0x05
#define HYD_INVALID_API  0x06
#define HYD_OPENGL_VERTEX_BUFFER_FAILED 0x07
#define HYD_GLEW_FAILED  0x08
#define HYD_CORRUPTED_GLTF 0x09
#define HYD_CORRUPTED_GLB  0x10
#define HYD_URI_FAILED 0x11
#define HYD_INVALID_COUNT 0x12
#define HYD_INVALID_TARGET 0x13	
#define HYD_WINDOW_FAILD 0x14
#define HYD_FAILED_TEXTURE_CREATION 0x15
#define HYD_INVALID_VALUE 0x16
#define HYD_UI_INVALID_WINDOW 0x17
#define HYD_UI_INVALID_ELEMENT 0x18
#define HYD_SHADER_FAILED 0x19
#define HYD_IMAGE_FAILED 0x20
#define HYD_NO_CONTENT 0x21

#define GLB_MAGIC     0x46546C67
#define GLB_JSON_TYPE 0x4E4F534A
#define GLB_BIN_TYPE  0x004E4942

typedef unsigned long long int uint64;
typedef unsigned int		   uint32;
typedef unsigned short int	   uint16;
typedef unsigned char		   uint8;
typedef long long int		   int64;
typedef int					   int32;
typedef short				   int16;
typedef char				   int8;

#include "HydPch.h"

//Decleartions
namespace Hydrogen
{

	struct Primitive;

	template<typename ResourceType>
	class ResourcePool;


	class ModelPool;
	class MaterialPool;
	class ShaderPool;

	class  MeshGenerator;
	class  Scene;
	class  Renderer;
	class  Mesh;
	class  Model;
	class  Shader;
	class  Material;
	class  Texture;
	class  Transformation;
	class  GLTFLoader;


	class Mouse;
	class Keyboard;
	
};


