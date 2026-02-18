#pragma once

#include "Common.h"
#include "3rdParty/glm/glm.hpp"

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

		static Type RetriveType(std::string& pType)
		{
			if (pType == "SCALAR")
				return SCALER;
			if (pType == "VEC2")
				return VEC2;
			if (pType == "VEC3")
				return VEC3;
			if (pType == "VEC4")
				return VEC4;
			if (pType == "MAT2")
				return MAT2;
			if (pType == "MAT3")
				return MAT3;
			if (pType == "MAT4")
				return MAT4;
			return INVALID;
		}
	};

	struct GeoNode //structure for GLTF nodes
	{
		int32 Mesh = -1;
		glm::mat4 Transformation = glm::mat4(1.0f);
		std::vector<uint32> Children;
	};

};