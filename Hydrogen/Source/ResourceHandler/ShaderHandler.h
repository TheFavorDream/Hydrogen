#pragma once

#include "Common.h"
#include "ResourceHandler.h"
#include "Shader/Shader.h"

namespace Hydrogen
{

	class ShaderHandler
	{
	public:

		//Init and Shutdown
		HYD static uint32 InitHandler();
		HYD static uint32 ShutdownHandler();

		//Shader Creation
		HYD static Id CreateShader(const std::string& pPath);
		HYD static Id CreateShader(const std::string& pVertex, const std::string& pFragment);
		
		HYD static Id	  PushShader(Shader* pShader);
		HYD static uint32 PopShader(Id* pShaderID);

		HYD static Shader& GetShader(Id pShaderID);

		//Shader Binding

		HYD static uint32 BindShader(Id pShaderID);
		HYD static uint32 BindRecentShader();
		HYD static uint32 UnbindCurrentShader();

		//Uniform Setting

		HYD static uint32 SetUniformFloat1(Id pShaderID, const char* pName, float pValue);
		HYD static uint32 SetUniformFloat2(Id pShaderID, const char* pName, float pValue1, float pValue2);
		HYD static uint32 SetUniformFloat3(Id pShaderID, const char* pName, float pValue1, float pValue2, float pValue3);
		
		HYD static uint32 SetUniformInt1(Id pShaderID, const char* pName, int32 pValue);
		HYD static uint32 SetUniformInt2(Id pShaderID, const char* pName, int32 pValue1, int32 pValue2);
		HYD static uint32 SetUniformInt3(Id pShaderID, const char* pName, int32 pValue1, int32 pValue2, int32 pValue3);

		HYD static uint32 SetUniformMat4(Id pShaderID, const char* pName, float* pValue);


		//Uniform Broadcasting

		HYD static uint32 BroadCastUniformF1(const char* pName, float pValue);
		HYD static uint32 BroadCastUniformF2(const char* pName, float pValue1, float pValue2);
		HYD static uint32 BroadCastUniformF3(const char* pName, float pValue1, float pValue2, float pValue3);


		HYD static uint32 BroadCastUniformI1(const char* pName, int pValue);
		HYD static uint32 BroadCastUniformI2(const char* pName, int pValue1, int pValue2);
		HYD static uint32 BroadCastUniformI3(const char* pName, int pValue1, int pValue2, int pValue3);

		HYD static uint32 BroadCastUniformMat4(const char* pName, float* pValue);

	private:
		static Id s_CurrentShader;
		static Id s_PrevShader;

		HYD static std::vector<Id> s_ShaderIDs;
	};

};