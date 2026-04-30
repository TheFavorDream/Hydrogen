#include "ShaderPool.h"
#include "HydPch.h"


namespace Hydrogen
{

//Init and Shutdown
	uint32 ShaderPool::InitHandler()
	{
		
		return HYD_OK;
	}

	uint32 ShaderPool::ShutdownHandler()
	{
		m_Shaders.Clear();
		return HYD_OK;
	}

//Shader Creation

	Id ShaderPool::CreateShader(const std::string& pPath)
	{
		Shader* NewShader = ResourcePool<Shader>::New();
		NewShader->CreateShader(pPath);
		return m_Shaders.Push(NewShader);
	}

	Id ShaderPool::CreateShaderFromSource(const std::string& pSource)
	{
		Shader* NewShader = ResourcePool<Shader>::New();
		NewShader->CreateShaderFromSource(pSource);
		return m_Shaders.Push(NewShader);
	}

	Id ShaderPool::CreateShader(const std::string& pVertex, const std::string& pFragment)
	{
		Shader* NewShader = ResourcePool<Shader>::New();
		NewShader->CreateShader(pVertex, pFragment);
		return m_Shaders.Push(NewShader);
	}

  
	uint32 ShaderPool::PopShader(Id * pShaderID)
	{
		if (m_CurrentShader == *pShaderID)
			UnbindCurrentShader();
		return m_Shaders.Pop(pShaderID);
	}

	Shader& ShaderPool::GetShader(Id pShaderID)
	{
		if (pShaderID == 0)
		{
			__debugbreak();
		}
		return m_Shaders.GetResource(pShaderID);
	}

//Binding

	uint32 ShaderPool::BindShader(Id pShaderID)
	{
		GetShader(pShaderID).Bind();
		m_CurrentShader = pShaderID;
		return HYD_OK;
	}

	uint32 ShaderPool::BindRecentShader()
	{
		if (m_PrevShader == 0)
			return HYD_INVALID_TARGET;
		m_CurrentShader = m_PrevShader;
		GetShader(m_PrevShader).Bind();
		return HYD_OK;
	}

	uint32 ShaderPool::UnbindCurrentShader()
	{
		m_PrevShader = m_CurrentShader;
		GetShader(m_CurrentShader).Unbind();
		m_CurrentShader = 0;
		return HYD_OK;
	}

//Uniform Setting

	uint32 ShaderPool::SetUniformFloat1(Id pShaderID, const char* pName, float pValue)
	{
		if (pShaderID == 0)
			return HYD_INVALID_VALUE;
		GetShader(pShaderID).SetUniformFloat1(pName, pValue);
		return HYD_OK;
	}

	uint32 ShaderPool::SetUniformFloat2(Id pShaderID, const char* pName, float pValue1, float pValue2)
	{
		if (pShaderID == 0)
			return HYD_INVALID_VALUE;
		GetShader(pShaderID).SetUniformFloat2(pName, pValue1, pValue2);
		return HYD_OK;
	}

	uint32 ShaderPool::SetUniformFloat3(Id pShaderID, const char* pName, float pValue1, float pValue2, float pValue3)
	{
		if (pShaderID == 0)
			return HYD_INVALID_VALUE;
		GetShader(pShaderID).SetUniformFloat3(pName, pValue1, pValue2, pValue3);
		return HYD_OK;
	}


	uint32 ShaderPool::SetUniformInt1(Id pShaderID, const char* pName, int32 pValue)
	{
		if (pShaderID == 0)
			return HYD_INVALID_VALUE;
		GetShader(pShaderID).SetUniformInt1(pName, pValue);
		return HYD_OK;
	}

	uint32 ShaderPool::SetUniformInt2(Id pShaderID, const char* pName, int32 pValue1, int32 pValue2)
	{
		if (pShaderID == 0)
			return HYD_INVALID_VALUE;
		GetShader(pShaderID).SetUniformInt2(pName, pValue1, pValue2);
		return HYD_OK;
	}

	uint32 ShaderPool::SetUniformInt3(Id pShaderID, const char* pName, int32 pValue1, int32 pValue2, int32 pValue3)
	{
		if (pShaderID == 0)
			return HYD_INVALID_VALUE;
		GetShader(pShaderID).SetUniformInt3(pName, pValue1, pValue2, pValue3);
		return HYD_OK;
	}

	uint32 ShaderPool::SetUniformMat4(Id pShaderID, const char* pName, float * pValue, bool pTranspose)
	{
		if (pShaderID == 0)
			return HYD_INVALID_VALUE;
		GetShader(pShaderID).SetUniformMat4(pName, pValue, pTranspose);
		return HYD_OK;
	}


//BroadCast

	uint32 ShaderPool::BroadCastUniformF1(const char* pName, float pValue)
	{
		for (auto& id : m_Shaders.GetTable())
		{
			GetShader(id.first).SetUniformFloat1(pName, pValue);
		}
		return HYD_OK;
	}

	uint32 ShaderPool::BroadCastUniformF2(const char* pName, float pValue1, float pValue2)
	{
		for (auto& id : m_Shaders.GetTable())
		{
			GetShader(id.first).SetUniformFloat2(pName, pValue1, pValue2);
		}
		return HYD_OK;
	}

	uint32 ShaderPool::BroadCastUniformF3(const char* pName, float pValue1, float pValue2, float pValue3)
	{
		for (auto& id : m_Shaders.GetTable())
		{
			GetShader(id.first).SetUniformFloat3(pName, pValue1, pValue2, pValue3);
		}
		return HYD_OK;
	}

	uint32 ShaderPool::BroadCastUniformI1(const char* pName, int pValue)
	{
		for (auto& id : m_Shaders.GetTable())
		{
			GetShader(id.first).SetUniformInt1(pName, pValue);
		}
		return HYD_OK;
	}

	uint32 ShaderPool::BroadCastUniformI2(const char* pName, int pValue1, int pValue2)
	{
		for (auto& id : m_Shaders.GetTable())
		{
			GetShader(id.first).SetUniformInt2(pName, pValue1, pValue2);
		}
		return HYD_OK;
	}

	uint32 ShaderPool::BroadCastUniformI3(const char* pName, int pValue1, int pValue2, int pValue3)
	{
		for (auto& id : m_Shaders.GetTable())
		{
			GetShader(id.first).SetUniformInt3(pName, pValue1, pValue2, pValue3);
		}
		return HYD_OK;
	}

	uint32 ShaderPool::BroadCastUniformMat4(const char* pName, float* pValue)
	{
		for (auto& id : m_Shaders.GetTable())
		{
			GetShader(id.first).SetUniformMat4(pName, pValue);
		}
		return HYD_OK;
	}

};