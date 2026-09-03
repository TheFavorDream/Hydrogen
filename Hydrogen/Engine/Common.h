#pragma once

#if defined(__gnuc__)
	#define SHEARED_EXPORT  __attribute__((visibility("default")))
	#define SHEARED_IMPORT
#elif defined(_MSC_VER)
	#define SHEARED_EXPORT __declspec(dllexport)
	#define SHEARED_IMPORT __declspec(dllimport)
#else
	#define SHEARED_EXPORT
	#define SHEARED_IMPORT
#endif

#ifdef HYD_DLL_BUILD
	#define HYD SHEARED_EXPORT
#else
	#define HYD SHEARED_IMPORT
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
#define HYD_FAILED     0x22

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
#include "Core/Memory/Memory.h"

#ifdef WINDOWS

#include <windows.h>

#endif

#define VULKAN_ALLOCATION_CALLBACK nullptr


#define HYD_VERSION_MAJOR 0
#define HYD_VERSION_MINOR 3
#define HYD_VERSION_PATCH 0


#define HYD_TRUE  1U
#define HYD_FALSE 0U



enum DataType {
	TYPE_UNKNOWN	    = 0,
	TYPE_SIGNED_BYTE    = 5120,
	TYPE_UNSIGNED_BYTE  = 5121,
	TYPE_SIGNED_SHORT	= 5122,
	TYPE_UNSIGNED_SHORT = 5123,
	TYPE_UNSIGNED_INT	= 5125,
	TYPE_FLOAT			= 5126
};




#define HYD_ID_SPACE uint32

#define HYD_VEC    std::vector
#define HYD_STRING std::string

//Decleartions
namespace Hydrogen
{

	template <typename Type>
	class ResourcePool;
	
	template <typename Type>
	struct Instance;

	class  MeshGenerator;
	class  Scene;
	class  Node;
	class  Renderer;
	class  Mesh;
	class  Primitive;
	class  Material;

	
	class  Grid;

	class  Attachment;
	class  Texture2D;
	class  Texture3D;
	class  CubeMap;
	
	class  Mouse;
	class  Keyboard;
	
	namespace Internal
	{
		class  FileSys;
		class  Memeory;

		//Vulkan:
		namespace Vulkan
		{
			class FrameBuffer;
			class Shader;
			class Renderpass;
			class Swapchain;
			class CommandBuffer;
			class CommandPool;
			class GraphicsPipeline;
			class PipelineLayout;
			class BasicBuffer;
			class VertexBuffer;
			class IndexBuffer;
			class StagingBuffer;
			class UniformBuffer;
			class DescriptorPool;
			class DescriptorSet; 
			class DescriptorSetLayout;
			class Image;
			class ImageView;
			class Sampler;
			class Device;
			class Instance;
		}; 

	};

	    //typedef Instance<Internal::Vulkan::PipelineLayout>   PipelineLayout;
	typedef Instance<Internal::Vulkan::Image>            		     ImageRef;
    typedef Instance<Internal::Vulkan::GraphicsPipeline> 		     GraphicsPipelineRef;
	typedef Instance<Internal::Vulkan::VertexBuffer>	 		     VertexBufferRef;
	typedef Instance<Internal::Vulkan::IndexBuffer>	     		     IndexBufferRef;
	typedef Instance<std::vector<Internal::Vulkan::UniformBuffer>>	 UniformRef;
	typedef Internal::Vulkan::DescriptorSetLayout        			 UniformLayoutRef;
};


