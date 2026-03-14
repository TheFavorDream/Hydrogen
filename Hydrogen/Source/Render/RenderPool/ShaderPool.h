#pragma once

#include "Common.h"
#include "Core/ResourcePool.h"
#include "Render/Platform/OpenGL/GLShader.h"
#include "Log/Log.h"

namespace Hydrogen
{

	class ShaderPool
	{
	public:

		//Init and Shutdown
		HYD uint32 InitHandler();
		HYD uint32 ShutdownHandler();

		//Shader Creation
		HYD Id CreateShader(const std::string& pPath);
		HYD Id CreateShaderFromSource(const std::string& pSource);
		HYD Id CreateShader(const std::string& pVertex, const std::string& pFragment);
		HYD uint32 PopShader(Id* pShaderID);

		HYD Shader& GetShader(Id pShaderID);

		//Shader Binding

		HYD uint32 BindShader(Id pShaderID);
		HYD uint32 BindRecentShader();
		HYD uint32 UnbindCurrentShader();

		//Uniform Setting

		HYD uint32 SetUniformFloat1(Id pShaderID, const char* pName, float pValue);
		HYD uint32 SetUniformFloat2(Id pShaderID, const char* pName, float pValue1, float pValue2);
		HYD uint32 SetUniformFloat3(Id pShaderID, const char* pName, float pValue1, float pValue2, float pValue3);
		
		HYD uint32 SetUniformInt1(Id pShaderID, const char* pName, int32 pValue);
		HYD uint32 SetUniformInt2(Id pShaderID, const char* pName, int32 pValue1, int32 pValue2);
		HYD uint32 SetUniformInt3(Id pShaderID, const char* pName, int32 pValue1, int32 pValue2, int32 pValue3);

		HYD uint32 SetUniformMat4(Id pShaderID, const char* pName, float* pValue);


		//Uniform Broadcasting

		HYD uint32 BroadCastUniformF1(const char* pName, float pValue);
		HYD uint32 BroadCastUniformF2(const char* pName, float pValue1, float pValue2);
		HYD uint32 BroadCastUniformF3(const char* pName, float pValue1, float pValue2, float pValue3);


		HYD uint32 BroadCastUniformI1(const char* pName, int pValue);
		HYD uint32 BroadCastUniformI2(const char* pName, int pValue1, int pValue2);
		HYD uint32 BroadCastUniformI3(const char* pName, int pValue1, int pValue2, int pValue3);

		HYD uint32 BroadCastUniformMat4(const char* pName, float* pValue);

	private:

		Id m_CurrentShader;
		Id m_PrevShader;
		ResourcePool<Shader> m_Shaders;
	};

};