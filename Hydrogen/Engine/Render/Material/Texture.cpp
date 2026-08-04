#include "Texture.h"
#include "Render/Renderer.h"
#include "HydPch.h"

//Texture interface implementation:

namespace Hydrogen
{
	Texture2D::Texture2D()
	{

	}

	Texture2D::Texture2D(Texture2D&& pOther)
	{
		m_TexObject = pOther.m_TexObject;
		pOther.m_TexObject = nullptr;
	}

	Texture2D& Texture2D::operator=(Texture2D&& pOther)
	{
		if (this == &pOther)
			return *this;

		m_TexObject = pOther.m_TexObject;
		pOther.m_TexObject = nullptr;
		
		return *this;
	}

	Texture2D::~Texture2D()
	{

	}

	uint32 Texture2D::CreateTexture(const Image& pImage, Sampler pSampler)
	{

		return HYD_FAILED;
	}

	uint32 Texture2D::DestroyTexture()
	{

		return HYD_FAILED;
	}

	//Read & Copy Texture:


	//Bind & Unbind:
	void Texture2D::Bind(uint32 pSlot) const
	{

	}

	void Texture2D::Unbind(uint32 pSlot) const
	{

	}

	//Getters:
	uint32 Texture2D::GetTextureID() noexcept
	{

		return 0;
	}

	uint32 Texture2D::GetWidth() noexcept
	{

		return 0;
	}

	uint32 Texture2D::GetHeight()  noexcept
	{

		return 0;
	}

	uint32 Texture2D::GetChannels() noexcept
	{
		return 0;
	}



	//Texture Cube:
	CubeMap::CubeMap()
	{

	}

	CubeMap::~CubeMap()
	{

	}

	CubeMap::CubeMap(CubeMap&& pOther)
	{
		m_TexObject = pOther.m_TexObject;
		pOther.m_TexObject = nullptr;
	}

	CubeMap& CubeMap::operator=(CubeMap&& pOther)
	{
		if (&pOther == this)
			return *this;

		m_TexObject = pOther.m_TexObject;
		pOther.m_TexObject = nullptr;

		return *this;
	}

	uint32 CubeMap::CreateCubeMap(const std::array<Image, 6>& pImages, const std::array<Sampler, 6>& pSamplers)
	{

		return HYD_FAILED;
	}

	uint32 CubeMap::DestroyTexture()
	{

		return HYD_FAILED;
	}

	void CubeMap::Bind(uint32 pSlot) const
	{

	}

	void CubeMap::Unbind(uint32 pSlot) const
	{

	}

	uint32 CubeMap::GetTextureID() noexcept
	{

		return 0;
	}
	
	uint32 CubeMap::GetWidth() noexcept
	{

		return 0;
	}
	
	uint32 CubeMap::GetHeight() noexcept
	{

		return 0;
	}
	
	uint32 CubeMap::GetChannels() noexcept
	{

		return 0;
	}

};