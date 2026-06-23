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
		HYD Instance<Shader> CreateShader(const std::string& pPath);
		HYD Instance<Shader> CreateShaderFromSource(const std::string& pSource);
		HYD Instance<Shader> CreateShader(const std::string& pVertex, const std::string& pFragment);


		//Shader Binding

		HYD uint32 BindShader(Instance<Shader>& pShaderID);
		HYD uint32 BindRecentShader();
		HYD uint32 UnbindCurrentShader();

		//Uniform Setting

		HYD uint32 SetUniformFloat1(Instance<Shader>& pShaderID, const char* pName, float pValue);
		HYD uint32 SetUniformFloat2(Instance<Shader>& pShaderID, const char* pName, float pValue1, float pValue2);
		HYD uint32 SetUniformFloat3(Instance<Shader>& pShaderID, const char* pName, float pValue1, float pValue2, float pValue3);
		
		HYD uint32 SetUniformInt1(Instance<Shader>& pShaderID, const char* pName, int32 pValue);
		HYD uint32 SetUniformInt2(Instance<Shader>& pShaderID, const char* pName, int32 pValue1, int32 pValue2);
		HYD uint32 SetUniformInt3(Instance<Shader>& pShaderID, const char* pName, int32 pValue1, int32 pValue2, int32 pValue3);

		HYD uint32 SetUniformMat4(Instance<Shader>& pShaderID, const char* pName, float* pValue, bool Transpose=false);


		//Uniform Broadcasting

		HYD uint32 BroadCastUniformF1(const char* pName, float pValue);
		HYD uint32 BroadCastUniformF2(const char* pName, float pValue1, float pValue2);
		HYD uint32 BroadCastUniformF3(const char* pName, float pValue1, float pValue2, float pValue3);


		HYD uint32 BroadCastUniformI1(const char* pName, int pValue);
		HYD uint32 BroadCastUniformI2(const char* pName, int pValue1, int pValue2);
		HYD uint32 BroadCastUniformI3(const char* pName, int pValue1, int pValue2, int pValue3);

		HYD uint32 BroadCastUniformMat4(const char* pName, float* pValue);

	private:

		Instance<Shader>     m_CurrentShader;
		Instance<Shader>     m_PrevShader;
		ResourcePool<Shader> m_Shaders;
	};

};