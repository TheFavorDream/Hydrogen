#include "TextureBase.h"
#include "HydPch.h"

namespace Hydrogen
{
	TextureBase::TextureBase()
	{
		m_TextureID = 0;
		m_Width		= 0;
		m_Height	= 0;
		m_Channels	= 0;
	}

	TextureBase::TextureBase(TextureBase&& pOther)
	{
		m_TextureID			= pOther.m_TextureID;
		m_Width				= pOther.m_Width;
		m_Height			= pOther.m_Height;
		m_Channels			= pOther.m_Channels;

		pOther.m_TextureID = 0;
		pOther.m_Width	   = 0;
		pOther.m_Height    = 0;
		pOther.m_Channels  = 0;
	}

	TextureBase& TextureBase::operator=(TextureBase&& pOther)
	{
		if (this != &pOther)
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
		return *this;
	}




	//Texture Cube:
	TextureCube::TextureCube()
	{
		m_TextureID = 0;
		m_Width		= 0;
		m_Height	= 0;
		m_Channels	= 0;
	}

	TextureCube::TextureCube(TextureCube&& pOther)
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

	TextureCube& TextureCube::operator=(TextureCube&& pOther)
	{


		if (&pOther == this)
			return *this;


		m_TextureID			= pOther.m_TextureID;
		m_Width				= pOther.m_Width;
		m_Height			= pOther.m_Height;
		m_Channels			= pOther.m_Channels;

		pOther.m_TextureID  = 0;
		pOther.m_Width		= 0;
		pOther.m_Height		= 0;
		pOther.m_Channels	= 0;
		return *this;
	}
};