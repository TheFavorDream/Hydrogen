#include "Texture.h"


namespace Hydrogen
{



	Texture::Texture(int32 pWidth, int32 pHeight, int32 pChannels, uint8* pData)
	{
		CreateTexture(pWidth, pHeight, pChannels, pData);
	}

	Texture::~Texture()
	{
		DestroyTexture();
	}


	Texture::Texture(const Texture & pOther)
	{
		m_Width = pOther.m_Width;
		m_Height = pOther.m_Height;
		m_Channels = pOther.m_Channels;
		m_TextureID = pOther.m_TextureID;
	}

	Texture::Texture(Texture && pOther)
	{
		m_Width = pOther.m_Width;
		m_Height = pOther.m_Height;
		m_Channels = pOther.m_Channels;
		m_TextureID = pOther.m_TextureID;

		pOther.m_TextureID = 0;
	}


	int Texture::CreateTexture(int32 pWidth, int32 pHeight, int32 pChannels, uint8 * pData)
	{
		m_Width = pWidth;
		m_Height = pHeight;
		m_Channels = pChannels;
		GL_CALL(glGenTextures(1, &m_TextureID));

		if (m_TextureID == 0)
		{
			Log::SetError("Couldn't Create Texture", HYD_FAILED_TEXTURE_CREATION, __FILE__, __LINE__);
			return HYD_FAILED_TEXTURE_CREATION;
		}

		GL_CALL(glBindTexture(GL_TEXTURE_2D, m_TextureID));

		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
		
		GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, m_Channels, m_Width, m_Height, 0, m_Channels, GL_UNSIGNED_BYTE, pData));
		Unbind();
		return HYD_OK;
	}

	int Texture::CreateTexture(std::string pPath)
	{
		int32 Channel = 0;
		uint8* Data = stbi_load(pPath.c_str(), &m_Width, &m_Height, &Channel, NULL);

		if (!Data)
		{
			Log::SetError("Failed Reading Texture", HYD_FAILED_TEXTURE_CREATION, __FILE__, __LINE__);
			return HYD_FAILED_TEXTURE_CREATION;
		}

		return CreateTexture(m_Width, m_Height, RetiveChannels(Channel), Data);
	}

	int Texture::DestroyTexture()
	{
		//Unbind();
		//GL_CALL(glDeleteTextures(1, &m_TextureID));
		return HYD_OK;
	}

	void Texture::Bind(uint32 pSlot)
	{
		GL_CALL(glActiveTexture(GL_TEXTURE0+pSlot));
		GL_CALL(glBindTexture(GL_TEXTURE_2D, m_TextureID));
	}

	void Texture::Unbind()
	{
		GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
	}

	GLenum Texture::RetiveChannels(int32 pChannelNumber)
	{
		switch (pChannelNumber)
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
		Log::SetError("Invalid or unsupported Color Channel", GL_INVALID_ENUM, __FILE__, __LINE__);
		return GL_INVALID_ENUM;
	}

};