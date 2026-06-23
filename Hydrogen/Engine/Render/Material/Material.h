#pragma once

#include "Common.h"
#include "Texture.h"
#include "Core/ResourcePool.h"
#include "Render/Platform/OpenGL/GLShader.h"
//#include "Image.h"

namespace Hydrogen
{

	class Material
	{
	public:

		HYD  Material();
		HYD ~Material();

		HYD Material(const Material& pOther) = delete;
		HYD Material(Material&& pOther);

		HYD Material& operator=(const Material& pOther) = delete;
		HYD Material& operator=(Material&& pOther);


		HYD uint32 SetMetallicnessFactor(float pMetalicnessFactor);
		HYD uint32 SetRoughnessFactor(float   pRoughnessFactor);


		HYD uint32 SetBaseColorTexture(Texture2D&  pBaseColor);
		HYD uint32 SetNormalMapTexture(Texture2D&  pNormal);
		HYD uint32 SetMetallicTexture( Texture2D&  pMetallic);
		HYD uint32 SetEmissiveTexture( Texture2D&  pEmissive);
		HYD uint32 SetOcclusionTexture(Texture2D&  pOcclusion);


		//Bind Material:
		HYD uint32 Bind(const Shader& pShader);
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
		ResourcePool<Texture2D>       m_TexturePool;
	};

};