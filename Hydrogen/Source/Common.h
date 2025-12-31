/*
	Hydrogen is a GLTF asset loader designed by Pooya aka TheVoltage.
	Hydrogen is fast and reliable 3D gltf asset parser that can work with any Graphical API.
*/
#pragma once

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


#define GLB_MAGIC     0x46546C67
#define GLB_JSON_TYPE 0x4E4F534A
#define GLB_BIN_TYPE  0x004E4942

#define GL_CALL(x) x;Hydrogen::CheckOpenGLErrors(__FILE__, __LINE__);

typedef unsigned long long int uint64;
typedef unsigned int		   uint32;
typedef unsigned short int	   uint16;
typedef unsigned char		   uint8;
typedef long long int		   int64;
typedef int					   int32;
typedef short				   int16;
typedef char				   int8;

#include <iostream>
#include <fstream>

namespace Hydrogen
{
	class Loader;
	class Model;
	class Scene;
	class Node;
	class Mesh;
	class Material;
	class Matrix;

	struct BufferView
	{
		std::string  Data;
		uint32 Target;
	};

	enum Type { INVALID = 0, SCALER = 1, VEC2 = 2, VEC3 = 3, VEC4 = 4, MAT2 = 4, MAT3 = 9, MAT4 = 16 };
	struct Accessor
	{

		BufferView Data;
		uint32 Count;
		uint32 ComponentType;
		Type DataType;

		static Type RetriveType(std::string& pType);
	};

	/*
		Each Primitive has a Attribute.
		Attribute Objects store the Index for the specific Vertex Array Attrib Pointer.
	*/
	struct Attribute
	{
		int32 POSITION = -1;
		int32 NORMALS = -1;
		int32 TANGENT = -1;
		int32 TEXCOORDS_0 = -1;
		int32 TEXCOORDS_1 = -1;
		int32 COLOR_0 = -1;
	};

	struct Primitive
	{
		int32 VboID=-1;//-1 means no buffer is setted
		int32 EboID=-1;//-1 means no buffer is setted
		int32 VaoID=-1;//-1 means no buffer is setted
		int32 MaterialID;
		int32 RenderingMode=4;
		Attribute Attributes;
	};

	enum FileFormat {GLTF, GLB};

	void CheckOpenGLErrors(const char* file, uint32 Line);
};


