#include "Shader.h"

namespace Hydrogen
{

	Shader::Shader(std::string pShaderPath)
	{
		CreateShader(pShaderPath);
	}

	Shader::Shader(const std::string & pVertexSource, const std::string & pFragmentSource)
	{
		CreateShader(pVertexSource, pFragmentSource);
	}

	Shader::~Shader()
	{
		DeleteShader();
	}


	//This function does the overal creation of Shader Program
	int Shader::CreateShader(std::string pShaderPath)
	{
		m_ShaderPath = pShaderPath;
		std::string Content = ReadShaderFile(pShaderPath);
		if (Content == "")
		{
			std::cout << ("Content String was empty");
			return -1;
		}

		std::vector<std::string> Lines = FilterSource(Content);

		if (Lines.size() == 0)
		{
			std::cout << ("There were nothing to process");
			return -2;
		}

		if (Preprocess(Lines) < 0)
		{
			std::cout << ("Unable to Preprocess Shader Source");
			return -3;
		}

		int Res = CreateProgram();
		if (Res != -1)
		{
			std::cout << ("Shader Compiled & Linked Successfully. ID:") << Res << '\n';
		}
		return Res;
	}

	int Shader::CreateShader(const std::string & pVertexSource, const std::string & pFragmentSource)
	{
		m_ShadersSources[GL_VERTEX_SHADER] = pVertexSource;
		m_ShadersSources[GL_FRAGMENT_SHADER] = pFragmentSource;

		int Res = CreateProgram();
		if (Res != -1)
		{
			std::cout << ("Shader Compiled & Linked Successfully. ID:") << Res << '\n';
		}
		m_ShadersSources.clear();
		return Res;
	}

	//Undoes the actions of CreateShader()
	int Shader::DeleteShader()
	{
		Unbind();
		m_ShadersSources.clear();
		glDeleteProgram(m_ProgramID);
		return 0;
	}

	//Binds shader
	void Shader::Bind() const
	{
		glUseProgram(m_ProgramID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	//==========================Uniform Setter==========================

	void Shader::SetUniformFloat1(const char * pName, float pValue) const
	{
		int Location = GetUniformLocation(pName);
		if (Location < 0)
			return;
		glProgramUniform1f(m_ProgramID, Location, pValue);

	}

	void Shader::SetUniformFloat2(const char * pName, float pValue1, float pValue2) const
	{
		int Location = GetUniformLocation(pName);
		if (Location < 0)
			return;
		glProgramUniform2f(m_ProgramID, Location, pValue1, pValue2);
	}

	void Shader::SetUniformFloat3(const char * pName, float pValue1, float pValue2, float pValue3) const
	{
		int Location = GetUniformLocation(pName);
		if (Location < 0)
			return;
		glProgramUniform3f(m_ProgramID, Location, pValue1, pValue2, pValue3);
	}


	void Shader::SetUniformInt1(const char * pName, int32 pValue) const
	{
		int Location = GetUniformLocation(pName);
		if (Location < 0)
			return;
		glProgramUniform1i(m_ProgramID, Location, pValue);
	}

	void Shader::SetUniformInt2(const char * pName, int32 pValue1, int32 pValue2) const
	{
		int Location = GetUniformLocation(pName);
		if (Location < 0)
			return;
		glProgramUniform2i(m_ProgramID, Location, pValue1, pValue2);
	}

	void Shader::SetUniformInt3(const char * pName, int32 pValue1, int32 pValue2, int32 pValue3) const
	{
		int Location = GetUniformLocation(pName);
		if (Location < 0)
			return;
		glProgramUniform3i(m_ProgramID, Location, pValue1, pValue2, pValue3);
	}


	void Shader::SetUniformMat4(const char * pName, float * pValue) const
	{
		int Location = GetUniformLocation(pName);
		if (Location < 0)
			return;
		glProgramUniformMatrix4fv(m_ProgramID, Location, 1, GL_FALSE, pValue);
	}

	int32 Shader::GetUniformLocation(const char * pName) const
	{
		if (m_UniformLookUp.find(pName) != m_UniformLookUp.end())
			return m_UniformLookUp[pName];

		int Loc = glGetUniformLocation(m_ProgramID, pName);
		if (Loc == -1 && m_ProgramID != 0)
		{
			//Log::GetSelf()->SetWarning("Cannot Find Uniform: %c in Program: %i", pName, m_ProgramID);
			return -1;
		}
		m_UniformLookUp[pName] = Loc;
		return Loc;
	}

	std::string Shader::ReadShaderFile(const std::string & pShaderPath)
	{

		std::ifstream Source(pShaderPath, std::ios::in | std::ios::binary);
		if (!Source.is_open())
		{
			std::cout << ("Cannot Open Shader File At: %s");
			return "";
		}

		Source.seekg(0, Source.end);
		uint32 FileSize = Source.tellg();
		Source.seekg(0, Source.beg);

		std::string Content;
		Content.resize(FileSize);
		Source.read(&Content[0], FileSize);

		//Log::GetSelf()->SetInfo("Source: %s", Content);
		return Content;
	}


	//Files are different on Linux and Window, so we configure them depending on Platform

	std::vector<std::string> Shader::FilterSource(const std::string & pShaderSource)
	{
		std::vector<std::string> Lines;
		std::string Line;

		for (int i = 0; i <= pShaderSource.length(); i++)
		{
#ifdef HYD_PLATFORM_WINDOWS
			if ((pShaderSource[i] == char(13) && pShaderSource[i + 1] == char(10)) || i == pShaderSource.length())
#else
			if (pShaderSource[i] == char(10) || i == pShaderSource.length())
#endif			
			{

#ifdef HYD_PLATFORM_WINDOWS
				i++; //Carrige Return
#endif
				if (Line.length() == 0)//Skip Empty Lines
				{
					Line.clear();
					continue;
				}

				if (Line[0] == '/' && Line[1] == '/') //We ignore comments
				{
					Line.clear();
					continue;
				}
				Lines.push_back(Line);
				Line.clear();
			}
			else
			{
				Line += pShaderSource[i];
			}
		}

		return Lines;
	}

	int Shader::Preprocess(const std::vector<std::string>& pShaderContent)
	{

		std::string Token = "#type:";
		GLenum CurrentShader = 0;

		for (auto &i : pShaderContent)
		{
			int Location = i.find(Token);
			if (Location < i.length())
			{
				std::string Name = i.substr(Location + Token.length());
				CurrentShader = GetShaderEnum(Name);
				std::cout << ("Current Shader Type:") << Name << '\n';
				if (CurrentShader == 0)
				{
					std::cout << ("Invalid Shader type:") << Name << '\n';
				}
				continue;
			}

			if (CurrentShader != 0)
			{
				m_ShadersSources[CurrentShader] += (i + '\n');
			}

		}

		return 0;
	}

	int Shader::CompileShader(GLenum pType, const std::string & pSource)
	{
		int Shader = glCreateShader(pType);

		const char* Source = pSource.c_str();
		glShaderSource(Shader, 1, &Source, NULL);

		glCompileShader(Shader);

		//Error Handling:
		int Succeed = 0;
		glGetShaderiv(Shader, GL_COMPILE_STATUS, &Succeed);
		if (Succeed == GL_FALSE)
		{
			int LogSize = 0;
			glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, &LogSize);
			std::string Log;
			Log.resize(LogSize);
			glGetShaderInfoLog(Shader, LogSize, &LogSize, &Log[0]);
			std::cout << ("Can't compile shader:") << Log;

			glDeleteShader(Shader);
			return -1;
		}
		return Shader;
	}

	int Shader::CreateProgram()
	{
		m_ProgramID = glCreateProgram();

		//Compile each shader and attach theme to program
		std::vector<int> Shaders;
		for (auto &i : m_ShadersSources)
		{
			int CurrentShader = CompileShader(i.first, i.second);
			if (CurrentShader == -1)
			{
				std::cout << ("No shader compiled");
				continue;
			}

			glAttachShader(m_ProgramID, CurrentShader);
			Shaders.push_back(CurrentShader);
		}

		glLinkProgram(m_ProgramID);
		glValidateProgram(m_ProgramID);

		//Error Handling
		int Succeed = 0;
		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &Succeed);

		if (!Succeed)
		{
			int LogSize = 0;
			glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &LogSize);

			std::string Log;
			Log.resize(LogSize);

			glGetProgramInfoLog(m_ProgramID, LogSize, &LogSize, &Log[0]);

			std::cout << ("Couldn't Link Program. Message: ") <<  Log << '\n';

			//Detach and Delete Shaders:
			for (auto &i : Shaders)
			{
				glDetachShader(m_ProgramID, i);
				glDeleteShader(i);
			}
			return -1;
		}
		//Detach and Delete Shaders:
		for (auto &i : Shaders)
		{
			glDetachShader(m_ProgramID, i);
			glDeleteShader(i);
		}
		return m_ProgramID;
	}

	GLenum Shader::GetShaderEnum(std::string ShaderName)
	{

		if (ShaderName == "Vertex" || ShaderName == "vertex")
		{
			return GL_VERTEX_SHADER;
		}

		else if (ShaderName == "Fragment" || ShaderName == "fragment")
		{
			return GL_FRAGMENT_SHADER;
		}

		else if (ShaderName == "Pixel" || ShaderName == "pixel")
		{
			return GL_FRAGMENT_SHADER;
		}

		else if (ShaderName == "Compute" || ShaderName == "compute")
		{
			return GL_COMPUTE_SHADER;
		}

		else if (ShaderName == "Geometry" || ShaderName == "geometry")
		{
			return GL_GEOMETRY_SHADER;
		}

		return 0;
	}

};