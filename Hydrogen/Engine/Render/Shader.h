#pragma once

#include "../Common.h"
#include "../Core/ResourcePool.h"

namespace Hydrogen
{



	enum ShaderType {
		HYD_STAGE_UNSPECIFIED       			 = 0x00000000,
		HYD_STAGE_VERTEX_SHADER     			 = 0x00000001,
		HYD_STAGE_TESSELLATION_CONTROL_SHADER    = 0x00000002,
		HYD_STAGE_TESSELLATION_EVALUATION_SHADER = 0x00000004,
		HYD_STAGE_GEOMETRY_SHADER 				 = 0x00000008,
		HYD_STAGE_FRAGMENT_SHADER 				 = 0x00000010,
	};


	enum Token {
		TOKEN_UNKNOWN = 0,
		TOKEN_TYPE    = 1,
		TOKEN_NAME    = 2,
		TOKEN_ID	  = 3
	};


/*

	
	Shader Object, Represents a part of rendering pipeline.
	can be attached to multiple pipeline programs


	class Shader
	{
	public:

		HYD  Shader() = default;
		HYD  Shader(ShaderType pType, const std::string& pName = "");
		HYD ~Shader();

		HYD Shader(const Shader& pOther);
		HYD Shader(Shader&& pOther);

		HYD Shader& operator=(const Shader& pOther);
		HYD Shader& operator=(Shader&& pOther);

		//Loading Shader Source 
		HYD uint32 Create(ShaderType pType, const std::string& pName = "");
		HYD uint32 LoadShader(const std::string& pSource);
		HYD uint32 DeleteShader();

		//compiles the shader. if failed, returns an log
		HYD std::pair<uint32, std::string> CompileShader();

		HYD inline std::string& GetSource() { return m_SourceCode; }
		HYD inline std::string GetName() const { return m_Name; }
		HYD inline void* GetRawObj() const { return m_ShaderObj; }


	private:
		ShaderType  m_Type = HYD_UNSPECIFIED;
		std::string m_Name;
		std::string m_SourceCode;
		void*       m_ShaderObj = nullptr; //API specific implementation:

		friend class ShaderPool;
	};

	//Shader Table Definition:

	struct ShaderTable
	{
	public:

		 ShaderTable()  = default;
		~ShaderTable()  = default;

		HYD ShaderTable(const ShaderTable& pOther);
		HYD ShaderTable(ShaderTable&& pOther);
		 
		HYD ShaderTable& operator=(const ShaderTable& pOther);
		HYD ShaderTable& operator=(ShaderTable&& pOther);


		//Pushes and new shader
		HYD uint32 PushShader(uint64 pID, Shader pShader);

		//Retrive Shader using ID (checking)
		HYD Shader& operator[](uint64 pID);

		HYD inline bool Exist(uint64 pID) const { return m_Table.find(pID) != m_Table.end(); }

		HYD inline bool   Empty() const { return m_Table.empty(); }
		HYD inline uint64 Count() const { return m_Table.size(); }

	private:
		std::unordered_map<uint64, Shader> m_Table;
		friend class ShaderPool;
	};




	class Program
	{
	public:

		HYD Program() = default;
		HYD ~Program();

		HYD Program(const Program& pOther);
		HYD Program(Program&& pOther);

		HYD Program& operator=(const Program& pOther);
		HYD Program& operator=(Program&& pOther);


		HYD uint32 CreateProgram(std::string pName="");

		HYD uint32 AttachShader(uint64 pId, const Shader& pShader); // copies the shader
		HYD uint32 AttachShader(uint64 pId, Shader&& pShader); //Moves the shader 


		HYD std::pair<uint32, std::string> LinkProgram();
		HYD uint32 DeleteProgram();

		HYD void Bind()   const;
		HYD void Unbind() const;


		//Uniform Binding:
		
		HYD void SetUniformFloat1(const char* pName, float pValue) const;
		HYD void SetUniformFloat2(const char* pName, float pValue1, float pValue2) const;
		HYD void SetUniformFloat3(const char* pName, float pValue1, float pValue2, float pValue3) const;

		HYD void SetUniformInt1(const char* pName, int32 pValue) const;
		HYD void SetUniformInt2(const char* pName, int32 pValue1, int32 pValue2) const;
		HYD void SetUniformInt3(const char* pName, int32 pValue1, int32 pValue2, int32 pValue3) const;

		HYD void SetUniformMat4(const char* pName, float* pValue, bool pTranspose = false) const;
		HYD void SetUniformMat4(const char* pName, double* pValue) const;
		

	private:
		std::string m_Name;
		ShaderTable m_Table;
		void*		m_ProgramObject = nullptr;
	};


	//Shader Pool Definition:
	class ShaderPool
	{
	public:
		HYD static ShaderPool& Self();
	public:

		HYD  ShaderPool() = default;
		HYD ~ShaderPool();

		ShaderPool(const ShaderPool& pOther) = delete;
		ShaderPool(ShaderPool&& pOther)      = delete;

		ShaderPool& operator=(const ShaderPool& pOther) = delete;
		ShaderPool& operator=(ShaderPool&& pOther)		= delete;


		//Shader Creation
		HYD ShaderTable		  LoadShaders(const std::string& pPath);
		HYD Shader			  CreateShaderFromSource(ShaderType pType, const std::string& pSource, const std::string& pName = "");
		HYD Instance<Program> CreateProgram(const ShaderTable& pTable, std::string pName="");

		//Shader Binding
		HYD uint32 BindShaderProgram(Instance<Program>& pProgram);
		HYD uint32 BindRecentShader();
		HYD uint32 UnbindCurrentShader();


		//Uniform Broadcasting
		HYD uint32 BroadCastUniformF1(const char* pName, float pValue);
		HYD uint32 BroadCastUniformF2(const char* pName, float pValue1, float pValue2);
		HYD uint32 BroadCastUniformF3(const char* pName, float pValue1, float pValue2, float pValue3);


		HYD uint32 BroadCastUniformI1(const char* pName, int pValue);
		HYD uint32 BroadCastUniformI2(const char* pName, int pValue1, int pValue2);
		HYD uint32 BroadCastUniformI3(const char* pName, int pValue1, int pValue2, int pValue3);

		HYD uint32 BroadCastUniformMat4(const char* pName, float* pValue);

	private:

		bool CompareStr(std::string_view pFirst, std::string_view pSecond);
		std::pair<Token, std::string> GetToken(const std::string& pLine);
		uint64 ExtractID(std::string pIdStr);
		ShaderType GetShaderType(std::string pType);

		Buffer      ReadShaderFile(const std::string& pPath);
		ShaderTable Process(Buffer& pData);

		uint32      CompileShaders(ShaderTable& pTable);

	private:
		Instance<Program>	  m_RecentProgram;
		Instance<Program>	  m_ActiveProgram;
		ResourcePool<Program> m_ShaderPrograms;

	private:
		static Ptr<ShaderPool> s_Self;
		friend class Core;
	};
*/
};