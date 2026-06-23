#include "Texture.h"
#include "Render/Renderer.h"
#include "Render/Platform/OpenGL/GLTextures.h"
#include "HydPch.h"

//Texture interface implementation:

namespace Hydrogen
{
	Texture2D::Texture2D()
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			Ptr<GLTexture2D> NewTexture = Memory::AllocateRaw<GLTexture2D>();
			m_TexObject = static_cast<void*>(NewTexture);
			break;
		}
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
		if (m_TexObject)
		{
			switch (Renderer::GetRenderingAPI())
			{
			case API_OPENGL:
				delete static_cast<Ptr<GLTexture2D>>(m_TexObject);
				m_TexObject = nullptr;
				break;
			}
		}
	}

	uint32 Texture2D::CreateTexture(const Image& pImage, Sampler pSampler)
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLTexture2D>>(m_TexObject)->CreateTexture(pImage, pSampler);
		}
		return HYD_FAILED;
	}

	uint32 Texture2D::DestroyTexture()
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLTexture2D>>(m_TexObject)->DestroyTexture();
		}
		return HYD_FAILED;
	}

	//Read & Copy Texture:


	//Bind & Unbind:
	void Texture2D::Bind(uint32 pSlot) const
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			static_cast<Ptr<GLTexture2D>>(m_TexObject)->Bind(pSlot);
			break;
		}
	}

	void Texture2D::Unbind(uint32 pSlot) const
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			static_cast<Ptr<GLTexture2D>>(m_TexObject)->Unbind(pSlot);
			break;
		}
	}

	//Getters:
	uint32 Texture2D::GetTextureID() noexcept
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLTexture2D>>(m_TexObject)->GetTextureID();
		}
		return 0;
	}

	uint32 Texture2D::GetWidth() noexcept
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLTexture2D>>(m_TexObject)->GetWidth();
		}
		return 0;
	}

	uint32 Texture2D::GetHeight()  noexcept
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLTexture2D>>(m_TexObject)->GetHeight();
		}
		return 0;
	}

	uint32 Texture2D::GetChannels() noexcept
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLTexture2D>>(m_TexObject)->GetChannels();
		}
		return 0;
	}



	//Texture Cube:
	CubeMap::CubeMap()
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			Ptr<GLTextureCube> NewCubeMap = Memory::AllocateRaw<GLTextureCube>();
			m_TexObject = static_cast<void*>(NewCubeMap);
			break;
		}
	}

	CubeMap::~CubeMap()
	{
		if (m_TexObject)
		{
			switch (Renderer::GetRenderingAPI())
			{
			case API_OPENGL:
				delete static_cast<Ptr<GLTextureCube>>(m_TexObject);
				m_TexObject = nullptr;
			}
		}
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
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLTextureCube>>(m_TexObject)->CreateCubeMap(pImages, pSamplers);
		}
		return HYD_FAILED;
	}

	uint32 CubeMap::DestroyTexture()
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLTextureCube>>(m_TexObject)->DestroyTexture();
		}
		return HYD_FAILED;
	}

	void CubeMap::Bind(uint32 pSlot) const
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			static_cast<Ptr<GLTextureCube>>(m_TexObject)->Bind(pSlot);
			break;
		}
	}

	void CubeMap::Unbind(uint32 pSlot) const
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			static_cast<Ptr<GLTextureCube>>(m_TexObject)->Unbind(pSlot);
			break;
		}
	}

	uint32 CubeMap::GetTextureID() noexcept
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLTextureCube>>(m_TexObject)->GetTextureID();
		}
		return 0;
	}
	
	uint32 CubeMap::GetWidth() noexcept
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLTextureCube>>(m_TexObject)->GetWidth();
		}
		return 0;
	}
	
	uint32 CubeMap::GetHeight() noexcept
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLTextureCube>>(m_TexObject)->GetHeight();
		}
		return 0;
	}
	
	uint32 CubeMap::GetChannels() noexcept
	{
		switch (Renderer::GetRenderingAPI())
		{
		case API_OPENGL:
			return static_cast<Ptr<GLTextureCube>>(m_TexObject)->GetChannels();
		}
		return 0;
	}

};