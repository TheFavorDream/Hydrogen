#include "Log.h"

namespace Hydrogen
{

	std::vector<LogHolder> Log::m_Logs;

	void Log::SetError(uint32 pErrorCode, const char * pFilePath, uint32 pLine, const char* pDescription)
	{
		std::cout << "[ErrorCode:]" << TranslateErrorCode(pErrorCode) << " [Line:]" << pLine << " [File:]" << pFilePath << " [Description:]" << pDescription << '\n';
		m_Logs.push_back({pErrorCode, pLine, pFilePath, pDescription});
	}

	void Log::SetInfo(std::string pText)
	{
		SetInfo(pText.c_str());
	}

	void Log::SetInfo(const char * pText)
	{
		std::cout << "[Engine Info]" << pText << '\n';
	}


	void Log::EngineDump(const char * pDumpPath)
	{

		if (m_Logs.size() == 0)
			return;

		std::fstream DumpFile(pDumpPath, std::ios::out);
		if (DumpFile.fail())
			return;
		for (auto& i : m_Logs)
		{
			DumpFile << "[ErrorCode:]" << TranslateErrorCode(i.ErrorCode) << " [Line:]" << i.Line << " [File:]" << i.FilePath << " [Description:]" << i.OptionalText << '\n';

		}
		DumpFile.close();

	}

	void Log::CheckOpenGLErrors(const char * file, uint32 Line)
	{
		GLenum ErrorCode = 0;
		while ((ErrorCode = glGetError()) != GL_NO_ERROR)
		{
			SetError(ErrorCode, file, Line);
		}	
	}

	std::vector<LogHolder>& Log::GetLogs()
	{
		return m_Logs;
	}

	std::string Log::TranslateErrorCode(uint32 ErrorCode)
	{

		switch (ErrorCode)
		{
			case HYD_INVALID_FILE:
				return "HYD_INVALID_FILE";
			case HYD_INVALID_PATH:
				return "HYD_INVALID_PATH";
			case HYD_INVALID_GLTF:
				return "HYD_INVALID_GLTF";
			case HYD_INVALID_BIN :
				return "HYD_INVALID_BIN";
			case HYD_INVALID_GLB :
				return "HYD_INVALID_GLB";
			case HYD_INVALID_API :
				return "HYD_INVALID_API";
			case HYD_OPENGL_VERTEX_BUFFER_FAILED :
				return "HYD_OPENGL_VERTEX_BUFFER_FAILED";
			case HYD_GLEW_FAILED  :
				return "HYD_GLEW_FAILED";
			case HYD_CORRUPTED_GLTF :
				return "HYD_CORRUPTED_GLTF";
			case HYD_CORRUPTED_GLB  :
				return "HYD_CORRUPTED_GLB";
			case HYD_URI_FAILED :
				return "HYD_URI_FAILED";
			case HYD_INVALID_COUNT:
				return "HYD_INVALID_COUNT";
			case HYD_INVALID_TARGET:
				return "HYD_INVALID_TARGET";
			case HYD_WINDOW_FAILD:
				return "HYD_WINDOW_FAILD";
			case HYD_FAILED_TEXTURE_CREATION:
				return "HYD_FAILED_TEXTURE_CREATION";
			case HYD_INVALID_VALUE:
				return "HYD_INVALID_VALUE";
			case HYD_UI_INVALID_WINDOW:
				return "HYD_UI_INVALID_WINDOW";

			//OpenGL:
			case GL_INVALID_ENUM:
				return "GL_INVALID_ENUM";
			case GL_INVALID_VALUE:
				return "GL_INVALID_VALUE";
			case GL_INVALID_OPERATION:
				return "GL_INVALID_OPERATION";
			case GL_STACK_OVERFLOW:
				return "GL_STACK_OVERFLOW";
			case GL_STACK_UNDERFLOW:
				return "GL_STACK_UNDERFLOW";
			case GL_OUT_OF_MEMORY:
				return "GL_OUT_OF_MEMORY";
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				return "GL_INVALID_FRAMEBUFFER_OPERATION";
			case GL_CONTEXT_LOST:
				return "GL_CONTEXT_LOST";
			
		}

		return "Undefined Error Code";
	}
};