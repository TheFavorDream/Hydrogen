#pragma once

#include "../../Common.h"
#include "Image.h"

/*
	Texture API-independent interface:
*/

namespace Hydrogen
{

	class Texture2D
	{
	public:

		//Constructors
		HYD Texture2D();

		HYD ~Texture2D();

		HYD Texture2D(const Texture2D& pOther) = delete;
		HYD Texture2D(Texture2D&& pOther);

		HYD Texture2D& operator=(const Texture2D& pOther) = delete;
		HYD Texture2D& operator=(Texture2D&& pOther);


		//Creators:
		HYD uint32 CreateTexture(const Image& pImage, Sampler pSampler = Sampler());
		HYD uint32 DestroyTexture();

		//Read & Copy Texture:


		//Bind & Unbind:
		HYD void Bind(uint32 pSlot = 0) const;
		HYD void Unbind(uint32 pSlot)   const;

		//Getters:
		HYD uint32 GetTextureID() noexcept; 
		HYD uint32 GetWidth()	  noexcept;
		HYD uint32 GetHeight()	  noexcept;
		HYD uint32 GetChannels()  noexcept;

	private:
		void* m_TexObject = nullptr;
	};


	class CubeMap
	{
	public:

		HYD  CubeMap();
		HYD ~CubeMap();

		HYD CubeMap(const CubeMap& pOther) = delete;
		HYD CubeMap(CubeMap&& pOther);

		HYD CubeMap& operator=(const CubeMap& pOther) = delete;
		HYD CubeMap& operator=(CubeMap&& pOther);

		//Creators:

		//Creates an cube map with images and samplers
		HYD uint32 CreateCubeMap(const std::array<Image, 6>& pImages, const std::array<Sampler, 6>& pSamplers = {Sampler()});
		HYD uint32 DestroyTexture();

		//Read & Copy Texture:


		//Bind & Unbind:
		HYD void Bind(uint32 pSlot = 0) const;
		HYD void Unbind(uint32 pSlot)   const;

		//Getters:
		HYD uint32 GetTextureID() noexcept;
		HYD uint32 GetWidth()	  noexcept;
		HYD uint32 GetHeight()	  noexcept;
		HYD uint32 GetChannels()  noexcept;

	private:
		void* m_TexObject = nullptr;
	};

};