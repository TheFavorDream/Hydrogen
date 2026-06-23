#include "ShaderPool.h"
#include "HydPch.h"


namespace Hydrogen
{

//Init and Shutdown
	uint32 ShaderPool::InitHandler()
	{
		m_Shaders.Init();
		return HYD_OK;
	}

	uint32 ShaderPool::ShutdownHandler()
	{
		m_Shaders.Shutdown();
		return HYD_OK;
	}

//Shader Creation

	Instance<Shader> ShaderPool::CreateShader(const std::string& pPath)
	{
		Shader NewShader{};
		NewShader.CreateShader(pPath);
		return m_Shaders.PushObject(std::move(NewShader));
	}

	Instance<Shader> ShaderPool::CreateShaderFromSource(const std::string& pSource)
	{
		Shader NewShader{};
		NewShader.CreateShaderFromSource(pSource);
		return m_Shaders.PushObject(std::move(NewShader));
	}

	Instance<Shader> ShaderPool::CreateShader(const std::string& pVertex, const std::string& pFragment)
	{
		Shader NewShader{};
		NewShader.CreateShader(pVertex, pFragment);
		return m_Shaders.PushObject(std::move(NewShader));
	}


//Binding

	uint32 ShaderPool::BindShader(Instance<Shader>& pShader)
	{
		pShader->Bind();
		m_CurrentShader = pShader;
		return HYD_OK;
	}

	uint32 ShaderPool::BindRecentShader()
	{
		if (m_PrevShader.IsNull())
			return HYD_INVALID_TARGET;
		m_CurrentShader = m_PrevShader;
		m_PrevShader->Bind();
		return HYD_OK;
	}

	uint32 ShaderPool::UnbindCurrentShader()
	{
		m_PrevShader = m_CurrentShader;
		m_CurrentShader->Unbind();
		m_CurrentShader.Reset();
		return HYD_OK;
	}

//Uniform Setting

	uint32 ShaderPool::SetUniformFloat1(Instance<Shader>& pShaderID, const char* pName, float pValue)
	{
		if (pShaderID.IsNull())
			return HYD_INVALID_VALUE;
		pShaderID->SetUniformFloat1(pName, pValue);
		return HYD_OK;
	}

	uint32 ShaderPool::SetUniformFloat2(Instance<Shader>& pShaderID, const char* pName, float pValue1, float pValue2)
	{
		if (pShaderID.IsNull())
			return HYD_INVALID_VALUE;
		pShaderID->SetUniformFloat2(pName, pValue1, pValue2);
		return HYD_OK;
	}

	uint32 ShaderPool::SetUniformFloat3(Instance<Shader>& pShaderID, const char* pName, float pValue1, float pValue2, float pValue3)
	{
		if (pShaderID.IsNull())
			return HYD_INVALID_VALUE;
		pShaderID->SetUniformFloat3(pName, pValue1, pValue2, pValue3);
		return HYD_OK;
	}


	uint32 ShaderPool::SetUniformInt1(Instance<Shader>& pShaderID, const char* pName, int32 pValue)
	{
		if (pShaderID.IsNull())
			return HYD_INVALID_VALUE;
		pShaderID->SetUniformInt1(pName, pValue);
		return HYD_OK;
	}

	uint32 ShaderPool::SetUniformInt2(Instance<Shader>& pShaderID, const char* pName, int32 pValue1, int32 pValue2)
	{
		if (pShaderID.IsNull())
			return HYD_INVALID_VALUE;
		pShaderID->SetUniformInt2(pName, pValue1, pValue2);
		return HYD_OK;
	}

	uint32 ShaderPool::SetUniformInt3(Instance<Shader>& pShaderID, const char* pName, int32 pValue1, int32 pValue2, int32 pValue3)
	{
		if (pShaderID.IsNull())
			return HYD_INVALID_VALUE;
		pShaderID->SetUniformInt3(pName, pValue1, pValue2, pValue3);
		return HYD_OK;
	}

	uint32 ShaderPool::SetUniformMat4(Instance<Shader>& pShaderID, const char* pName, float * pValue, bool pTranspose)
	{
		if (pShaderID.IsNull())
			return HYD_INVALID_VALUE;
		pShaderID->SetUniformMat4(pName, pValue, pTranspose);
		return HYD_OK;
	}


//BroadCast

	uint32 ShaderPool::BroadCastUniformF1(const char* pName, float pValue)
	{
		for (auto& shader : m_Shaders)
		{
			shader.Object.SetUniformFloat1(pName, pValue);
		}
		return HYD_OK;
	}

	uint32 ShaderPool::BroadCastUniformF2(const char* pName, float pValue1, float pValue2)
	{
		for (auto& shader : m_Shaders)
		{
			shader.Object.SetUniformFloat2(pName, pValue1, pValue2);
		}
		return HYD_OK;
	}

	uint32 ShaderPool::BroadCastUniformF3(const char* pName, float pValue1, float pValue2, float pValue3)
	{
		for (auto& shader : m_Shaders)
		{
			shader.Object.SetUniformFloat3(pName, pValue1, pValue2, pValue3);
		}
		return HYD_OK;
	}

	uint32 ShaderPool::BroadCastUniformI1(const char* pName, int pValue)
	{
		for (auto& shader : m_Shaders)
		{
		shader.Object.SetUniformInt1(pName, pValue);
		}
		return HYD_OK;
	}

	uint32 ShaderPool::BroadCastUniformI2(const char* pName, int pValue1, int pValue2)
	{
		for (auto& shader : m_Shaders)
		{
			shader.Object.SetUniformInt2(pName, pValue1, pValue2);
		}
		return HYD_OK;
	}

	uint32 ShaderPool::BroadCastUniformI3(const char* pName, int pValue1, int pValue2, int pValue3)
	{
		for (auto& shader : m_Shaders)
		{
			shader.Object.SetUniformInt3(pName, pValue1, pValue2, pValue3);
		}
		return HYD_OK;
	}

	uint32 ShaderPool::BroadCastUniformMat4(const char* pName, float* pValue)
	{
		for (auto& shader : m_Shaders)
		{
			shader.Object.SetUniformMat4(pName, pValue);
		}
		return HYD_OK;
	}

};