#pragma once

#include "../../Common.h"
#include "Texture.h"
#include "Core/ResourcePool.h"
#include "Render/Shader.h"

//#include "Image.h"

namespace Hydrogen
{

	class Material
	{
	public:
		static Material CreateMaterialGLTF(const Xenon::Material& pMaterial, std::unordered_map<uint64, Instance<Texture2D>>& pTextureTable);
	public:

		HYD  Material();
		HYD ~Material();

		HYD Material(const Material& pOther) = delete;
		HYD Material(Material&& pOther);

		HYD Material& operator=(const Material& pOther) = delete;
		HYD Material& operator=(Material&& pOther);

		HYD uint32 SetMetallicnessFactor(float pMetalicnessFactor);
		HYD uint32 SetRoughnessFactor(float   pRoughnessFactor);

		//Bind Material:
		//HYD uint32 Bind(Instance<Program> pShader);
		HYD uint32 Unbind();


	private:

		float m_MatallicnessFactor = 0.0f;
		float m_RoughnessFactor    = 1.0f;
		VecF3 m_BaseColorFactor    = VecF3(1.0f);

		Instance<Texture2D>           m_BaseColor;
		Instance<Texture2D>           m_NormalMap;
		Instance<Texture2D>           m_MetallicMap;
		Instance<Texture2D>			  m_EmissiveMap;
		Instance<Texture2D>			  m_OcclusionMap;

		friend class Core;
	};

};