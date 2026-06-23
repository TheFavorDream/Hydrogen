/*

	Created by Pooya Alizadeg (The Voltage)
	Purpose:
		Shader Implemention class

*/
#pragma once

#include "Common.h"
#include "Glew/glew.h"
#include "Log/Log.h"
	

namespace Hydrogen
{
	class Shader 
	{
	public:

		HYD Shader() = default;
		HYD Shader(std::string pShaderPath);
		HYD Shader(const std::string& pVertexSource, const std::string& pFragmentSource);
		HYD ~Shader();


		HYD Shader(Shader&& pOther);
		HYD Shader(const Shader& pOther);

		HYD Shader& operator=(Shader&& pOther);
		HYD Shader& operator=(const Shader& pOther);

		HYD int CreateShader(std::string pShaderPath);
		HYD int CreateShader(const std::string& pVertexSource, const std::string& pFragmentSource);
		HYD int CreateShaderFromSource(const std::string& pSource);
		HYD int DeleteShader();
		 
		HYD void Bind() const;
		HYD void Unbind() const;
		 
		HYD void SetUniformFloat1(const char* pName, float pValue) const;
		HYD void SetUniformFloat2(const char* pName, float pValue1, float pValue2) const;
		HYD void SetUniformFloat3(const char* pName, float pValue1, float pValue2, float pValue3) const;
		 
		HYD void SetUniformInt1(const char* pName, int32 pValue) const;
		HYD void SetUniformInt2(const char* pName, int32 pValue1, int32 pValue2) const;
		HYD void SetUniformInt3(const char* pName, int32 pValue1, int32 pValue2, int32 pValue3) const;
		 
		HYD void SetUniformMat4(const char* pName, float* pValue, bool pTranspose=false) const;
		HYD void SetUniformMat4(const char* pName, double* pValue) const;
		 
		 
		 
		HYD inline const std::string GetShaderName() const { return m_ShaderPath.substr(m_ShaderPath.find_last_of('/') + 1); }
		HYD inline const std::string& GetShaderPath() const { return m_ShaderPath; }

	private:

		HYD int32 GetUniformLocation(const char* pName) const;
		HYD std::string ReadShaderFile(const std::string& pShaderPath);
		HYD std::vector<std::string> FilterSource(const std::string& pShaderSource);
		HYD int Preprocess(const std::vector<std::string>& pShaderContent);
		HYD int CompileShader(GLenum pType, const std::string& pSource);
		HYD int CreateProgram();

		HYD GLenum GetShaderEnum(std::string ShaderName);

	private:
		uint32											m_ProgramID = 0;
		std::string										m_ShaderPath;
		std::unordered_map<GLenum, std::string>			m_ShadersSources;
		mutable std::unordered_map<const char*, int32>  m_UniformLookUp; //for faster uniform access

	};
};
