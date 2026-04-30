#include "GLTextures.h"
#include "HydPch.h"


namespace Hydrogen
{


	GLenum RetriveChannel(int32 pChannel)
	{
		switch (pChannel)
		{
		case 1:
			return GL_RED;
		case 2:
			return GL_RG;
		case 3:
			return GL_RGB;
		case 4:
			return GL_RGBA;
		}
		return GL_INVALID_ENUM;
	}


//======================Texture 2D=================================================:
	GLTexture2D::GLTexture2D(Image& pImage, Sampler pSampler)
	{
		CreateTexture(pImage, pSampler);
	}

	GLTexture2D::~GLTexture2D()
	{
		DestroyTexture();
	}

	GLTexture2D::GLTexture2D(GLTexture2D&& pOther)
	{
		m_Width     = pOther.m_Width;
		m_Height    = pOther.m_Height;
		m_Channels  = pOther.m_Channels;
		m_TextureID = pOther.m_TextureID;

		pOther.m_TextureID = 0;
		pOther.m_Width     = 0;
		pOther.m_Height    = 0;
		pOther.m_Channels  = 0;
	}

	GLTexture2D& GLTexture2D::operator=(GLTexture2D&& pOther) noexcept
	{
		if (&pOther != this)
		{
			m_TextureID = pOther.m_TextureID;
			m_Width     = pOther.m_Width;
			m_Height    = pOther.m_Height;
			m_Channels  = pOther.m_Channels;

			pOther.m_TextureID  = 0;
			pOther.m_Width		= 0;
			pOther.m_Height		= 0;
			pOther.m_Channels	= 0;
		}

		return *this;
	}


	uint32 GLTexture2D::CreateTexture(const Image& pImage, Sampler pSampler)
	{
		m_Width    = pImage.Width;
		m_Height   = pImage.Height;
		m_Channels = (int32)pImage.ImageChannel;

		GL_CALL(glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID));

		if (m_TextureID == 0)
		{
			Log::SetError("Couldn't Create Texture", HYD_FAILED_TEXTURE_CREATION, __FILE__, __LINE__);
			return HYD_FAILED_TEXTURE_CREATION;
		}

		GL_CALL(glBindTexture(GL_TEXTURE_2D, m_TextureID));

		//pSampler = Sampler();

		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, pSampler.Min));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, pSampler.Mag));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, pSampler.WrapS));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, pSampler.WrapT));
		
		GLenum Channel = RetriveChannel(pImage.ImageChannel);
		GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, Channel, m_Width, m_Height, 0, Channel, GL_UNSIGNED_BYTE, (void*)pImage.Data));

		GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
		

		Unbind(0);
		return HYD_OK;
	}



	uint32 GLTexture2D::DestroyTexture()
	{
		Unbind(0);
		GL_CALL(glDeleteTextures(1, &m_TextureID));
		return HYD_OK;
	}

	void GLTexture2D::Bind(uint32 pSlot) const
	{
		GL_CALL(glActiveTexture(GL_TEXTURE0+pSlot));
		GL_CALL(glBindTexture(GL_TEXTURE_2D, m_TextureID));
	}
	void GLTexture2D::Unbind(uint32 pSlot) const
	{
		GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
	}


//============================Texture Cube=======================

	GLTextureCube::~GLTextureCube()
	{
		DestroyTexture();
	}

	GLTextureCube::GLTextureCube(GLTextureCube&& pOther)
	{
		m_TextureID			= pOther.m_TextureID;
		m_Width				= pOther.m_Width;
		m_Height			= pOther.m_Height;
		m_Channels			= pOther.m_Channels;

		pOther.m_TextureID	= 0;
		pOther.m_Width		= 0;
		pOther.m_Height		= 0;
		pOther.m_Channels	= 0;
	}

	GLTextureCube& GLTextureCube::operator=(GLTextureCube&& pOther)
	{
		m_Width		= pOther.m_Width;
		m_Height	= pOther.m_Height;
		m_Channels	= pOther.m_Channels;
		m_TextureID = pOther.m_TextureID;

		pOther.m_TextureID	= 0;
		pOther.m_Width		= 0;
		pOther.m_Height		= 0;
		pOther.m_Channels	= 0;

		return *this;
	}


	uint32 GLTextureCube::CreateCubeMap(const std::array<Image, 6>& pImages, const std::array<Sampler, 6>& pSamplers)
	{
		GL_CALL(glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_TextureID));
		GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID));

		uint16 Face = 0;
		for (auto& image : pImages)
		{
			GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+Face, 0, RetriveChannel(image.ImageChannel), image.Width, image.Height, 0, RetriveChannel(image.ImageChannel), GL_UNSIGNED_BYTE, image.Data));
		
			GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE));
			GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, pSamplers[Face].Min));
			GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, pSamplers[Face].Mag));
			GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,	  pSamplers[Face].WrapS));
			GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,	  pSamplers[Face].WrapT));
		
			GL_CALL(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
			Face += 1;
		}

		return HYD_OK;
	}

	uint32 GLTextureCube::DestroyTexture()
	{
		Unbind(0);
		GL_CALL(glDeleteTextures(1, &m_TextureID));
		return HYD_OK;
	}


	void GLTextureCube::Bind(uint32 pSlot) const
	{
		GL_CALL(glActiveTexture(GL_TEXTURE0+pSlot));
		GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID));
	}

	void GLTextureCube::Unbind(uint32 pSlot) const
	{
		GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
	}

};