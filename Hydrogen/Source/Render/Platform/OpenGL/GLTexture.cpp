#include "GLTexture.h"


namespace Hydrogen
{



	Texture::Texture(Image& pImage, Sampler pSampler)
	{
		CreateTexture(pImage, pSampler);
	}

	Texture::~Texture()
	{
		DestroyTexture();
	}



	Texture::Texture(Texture && pOther)
	{
		m_Width = pOther.m_Width;
		m_Height = pOther.m_Height;
		m_Channels = pOther.m_Channels;
		m_TextureID = pOther.m_TextureID;

		pOther.m_TextureID = 0;
		pOther.m_Width = 0;
		pOther.m_Height = 0;
		pOther.m_Channels = 0;
	}

	Texture& Texture::operator=(Texture && pOther) noexcept
	{
		if (&pOther != this)
		{
			m_TextureID = pOther.m_TextureID;
			m_Width = pOther.m_Width;
			m_Height = pOther.m_Height;
			m_Channels = pOther.m_Channels;

			pOther.m_TextureID  = 0;
			pOther.m_Width		= 0;
			pOther.m_Height		= 0;
			pOther.m_Channels	= 0;
		}

		return *this;
	}


	int Texture::CreateTexture(const Image& pImage, Sampler pSampler)
	{
		m_Width    = pImage.Width;
		m_Height   = pImage.Height;
		m_Channels = (int32)pImage.ImageChannel;

		GL_CALL(glGenTextures(1, &m_TextureID));

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

	int Texture::CreateTexture(std::string pPath)
	{
		Image image;
		
		uint32 ReturnCode;
		if ((ReturnCode = image.LoadImage(pPath.c_str())) != HYD_OK)
		{
			Log::SetError(Log::FmtStr("Unable to Load image at %s", pPath.c_str()), ReturnCode);
			return ReturnCode;
		}

		
		ReturnCode = CreateTexture(image);
		image.FreeImage();
		return ReturnCode;
	}

	int Texture::DestroyTexture()
	{

		Unbind(0);
		GL_CALL(glDeleteTextures(1, &m_TextureID));
		return HYD_OK;
	}

	void Texture::Bind(uint32 pSlot) const
	{
		GL_CALL(glActiveTexture(GL_TEXTURE0+pSlot));
		GL_CALL(glBindTexture(GL_TEXTURE_2D, m_TextureID));
	}

	void Texture::Unbind(uint32 pSlot) const
	{
		GL_CALL(glActiveTexture(GL_TEXTURE0+pSlot))
		GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
	}
	HYD GLenum Texture::RetriveChannel(int32 pChannel)
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
};