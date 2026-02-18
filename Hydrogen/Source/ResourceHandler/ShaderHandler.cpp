#include "ShaderHandler.h"

namespace Hydrogen
{

	Id ShaderHandler::s_CurrentShader = 0;
	Id ShaderHandler::s_PrevShader    = 0;
	std::vector<Id> ShaderHandler::s_ShaderIDs;


//Init and Shutdown
	uint32 ShaderHandler::InitHandler()
	{
		Log::SetInfo("Shader Handler: Ready");
		return HYD_OK;
	}

	uint32 ShaderHandler::ShutdownHandler()
	{
		Log::SetInfo("Shader Handler: Shutdown");

		for (auto& id : s_ShaderIDs)
		{
			ResourceHandler::PopResource(&id);
		}
		return HYD_OK;
	}

//Shader Creation

	Id ShaderHandler::CreateShader(const std::string& pPath)
	{
		Shader* NewShader = new Shader();
		NewShader->CreateShader(pPath);
		return PushShader(NewShader);
	}

	Id ShaderHandler::CreateShader(const std::string & pVertex, const std::string & pFragment)
	{
		Shader* NewShader = new Shader();
		NewShader->CreateShader(pVertex, pFragment);
		return PushShader(NewShader);
	}

    Id ShaderHandler::PushShader(Shader* pShader)
	{
		if (pShader == nullptr)
		{
			Log::SetWarning("Shader Handler: Shader was null");
			return 0;
		}
		return ResourceHandler::PushResource(pShader);
	}

	uint32 ShaderHandler::PopShader(Id * pShaderID)
	{
		if (s_CurrentShader == *pShaderID)
			UnbindCurrentShader();
		return ResourceHandler::PopResource(pShaderID);
	}

	Shader& ShaderHandler::GetShader(Id pShaderID)
	{
		if (pShaderID == 0)
		{
			__debugbreak();
		}
		return *dynamic_cast<Shader*>(ResourceHandler::GetResource(pShaderID));
	}

//Binding

	uint32 ShaderHandler::BindShader(Id pShaderID)
	{
		GetShader(pShaderID).Bind();
		s_CurrentShader = pShaderID;
		return HYD_OK;
	}

	uint32 ShaderHandler::BindRecentShader()
	{
		if (s_PrevShader == 0)
			return HYD_INVALID_TARGET;
		s_CurrentShader = s_PrevShader;
		GetShader(s_PrevShader).Bind();
		return HYD_OK;
	}

	uint32 ShaderHandler::UnbindCurrentShader()
	{
		s_PrevShader = s_CurrentShader;
		GetShader(s_CurrentShader).Unbind();
		s_CurrentShader = 0;
		return HYD_OK;
	}

//Uniform Setting

	uint32 ShaderHandler::SetUniformFloat1(Id pShaderID, const char* pName, float pValue)
	{
		if (pShaderID == 0)
			return HYD_INVALID_VALUE;
		GetShader(pShaderID).SetUniformFloat1(pName, pValue);
		return HYD_OK;
	}

	uint32 ShaderHandler::SetUniformFloat2(Id pShaderID, const char* pName, float pValue1, float pValue2)
	{
		if (pShaderID == 0)
			return HYD_INVALID_VALUE;
		GetShader(pShaderID).SetUniformFloat2(pName, pValue1, pValue2);
		return HYD_OK;
	}

	uint32 ShaderHandler::SetUniformFloat3(Id pShaderID, const char* pName, float pValue1, float pValue2, float pValue3)
	{
		if (pShaderID == 0)
			return HYD_INVALID_VALUE;
		GetShader(pShaderID).SetUniformFloat3(pName, pValue1, pValue2, pValue3);
		return HYD_OK;
	}


	uint32 ShaderHandler::SetUniformInt1(Id pShaderID, const char* pName, int32 pValue)
	{
		if (pShaderID == 0)
			return HYD_INVALID_VALUE;
		GetShader(pShaderID).SetUniformInt1(pName, pValue);
		return HYD_OK;
	}

	uint32 ShaderHandler::SetUniformInt2(Id pShaderID, const char* pName, int32 pValue1, int32 pValue2)
	{
		if (pShaderID == 0)
			return HYD_INVALID_VALUE;
		GetShader(pShaderID).SetUniformInt2(pName, pValue1, pValue2);
		return HYD_OK;
	}

	uint32 ShaderHandler::SetUniformInt3(Id pShaderID, const char* pName, int32 pValue1, int32 pValue2, int32 pValue3)
	{
		if (pShaderID == 0)
			return HYD_INVALID_VALUE;
		GetShader(pShaderID).SetUniformInt3(pName, pValue1, pValue2, pValue3);
		return HYD_OK;
	}

	uint32 ShaderHandler::SetUniformMat4(Id pShaderID, const char* pName, float * pValue)
	{
		if (pShaderID == 0)
			return HYD_INVALID_VALUE;
		GetShader(pShaderID).SetUniformMat4(pName, pValue);
		return HYD_OK;
	}


//BroadCast

	uint32 ShaderHandler::BroadCastUniformF1(const char* pName, float pValue)
	{
		for (auto& id : s_ShaderIDs)
		{
			GetShader(id).SetUniformFloat1(pName, pValue);
		}
		return HYD_OK;
	}

	uint32 ShaderHandler::BroadCastUniformF2(const char* pName, float pValue1, float pValue2)
	{
		for (auto& id : s_ShaderIDs)
		{
			GetShader(id).SetUniformFloat2(pName, pValue1, pValue2);
		}
		return HYD_OK;
	}

	uint32 ShaderHandler::BroadCastUniformF3(const char* pName, float pValue1, float pValue2, float pValue3)
	{
		for (auto& id : s_ShaderIDs)
		{
			GetShader(id).SetUniformFloat3(pName, pValue1, pValue2, pValue3);
		}
		return HYD_OK;
	}

	uint32 ShaderHandler::BroadCastUniformI1(const char* pName, int pValue)
	{
		for (auto& id : s_ShaderIDs)
		{
			GetShader(id).SetUniformInt1(pName, pValue);
		}
		return HYD_OK;
	}

	uint32 ShaderHandler::BroadCastUniformI2(const char* pName, int pValue1, int pValue2)
	{
		for (auto& id : s_ShaderIDs)
		{
			GetShader(id).SetUniformInt2(pName, pValue1, pValue2);
		}
		return HYD_OK;
	}

	uint32 ShaderHandler::BroadCastUniformI3(const char* pName, int pValue1, int pValue2, int pValue3)
	{
		for (auto& id : s_ShaderIDs)
		{
			GetShader(id).SetUniformInt3(pName, pValue1, pValue2, pValue3);
		}
		return HYD_OK;
	}

	uint32 ShaderHandler::BroadCastUniformMat4(const char* pName, float* pValue)
	{
		for (auto& id : s_ShaderIDs)
		{
			GetShader(id).SetUniformMat4(pName, pValue);
		}
		return HYD_OK;
	}





};