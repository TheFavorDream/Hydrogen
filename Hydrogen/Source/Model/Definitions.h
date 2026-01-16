#pragma once

#include "Common.h"

namespace Hydrogen
{

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
		int32 VboID = -1;//-1 means no buffer is setted
		int32 EboID = -1;//-1 means no buffer is setted
		int32 VaoID = -1;//-1 means no buffer is setted
		int32 MaterialID;
		int32 RenderingMode = 4;
		Attribute Attributes;
	};

	enum FileFormat { GLTF, GLB };

};