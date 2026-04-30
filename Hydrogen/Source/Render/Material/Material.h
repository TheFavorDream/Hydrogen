#pragma once

#include "Common.h"
#include "Textures/TextureBase.h"
#include "Render/RenderPool/TexturePool.h"
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


		HYD uint32 SetBaseColorTexture(Wraper<TextureBase>&  pBaseColor);
		HYD uint32 SetNormalMapTexture(Wraper<TextureBase>&  pNormal);
		HYD uint32 SetMetallicTexture( Wraper<TextureBase>&  pMetallic);
		HYD uint32 SetEmissiveTexture( Wraper<TextureBase>&  pEmissive);
		HYD uint32 SetOcclusionTexture(Wraper<TextureBase>&  pOcclusion);


		//Bind Material:
		HYD uint32 Bind(const Shader& pShader);
		HYD uint32 Unbind();


	private:

		float m_MatallicnessFactor = 0.0f;
		float m_RoughnessFactor    = 1.0f;
		VecF3 m_BaseColorFactor    = VecF3(1.0f);

		Id          m_BaseColor    = 0;
		Id          m_NormalMap    = 0;
		Id          m_MetallicMap  = 0;
		Id			m_EmissiveMap  = 0;
		Id			m_OcclusionMap = 0;
		TexturePool m_TexturePool;
	};

};