
/*
namespace Hydrogen
{

	Ptr<ShaderPool> ShaderPool::s_Self = nullptr;

	ShaderPool& ShaderPool::Self()
	{
		return *s_Self;
	}



	Shader::Shader(ShaderType pType, const std::string& pName) :
		m_Type(pType), m_Name(pName)
	{

	}

	Shader::~Shader()
	{
		DeleteShader();
	}


	Shader::Shader(const Shader& pOther) :
		m_Name(pOther.m_Name), m_SourceCode(pOther.m_SourceCode), m_Type(pOther.m_Type)
	{

	}

	Shader::Shader(Shader&& pOther) :
		m_Name(std::move(pOther.m_Name)), m_SourceCode(std::move(pOther.m_SourceCode)), m_Type(pOther.m_Type), m_ShaderObj(pOther.m_ShaderObj)
	{
		pOther.m_ShaderObj = nullptr;

	}

	Shader& Shader::operator=(const Shader& pOther)
	{
		if (&pOther == this)
			return *this;

		m_Name = pOther.m_Name;
		m_SourceCode = pOther.m_SourceCode;
		m_Type = pOther.m_Type;
		return *this;
	}

	Shader& Shader::operator=(Shader&& pOther)
	{
		m_Name = std::move(pOther.m_Name);
		m_SourceCode = std::move(pOther.m_SourceCode);
		m_Type = std::move(pOther.m_Type);
		m_ShaderObj = pOther.m_ShaderObj;

		pOther.m_ShaderObj = nullptr;
		return *this;
	}

	uint32 Shader::Create(ShaderType pType, const std::string& pName)
	{
		m_Type = pType;
		m_Name = pName;

		return HYD_OK;
	}



	uint32 Shader::LoadShader(const std::string& pSource)
	{
		return HYD_OK;
	}

	uint32 Shader::DeleteShader()
	{

		if (!m_ShaderObj)
			return HYD_FAILED;


		return HYD_FAILED;
	}

	std::pair<uint32, std::string> Shader::CompileShader()
	{

		return std::pair<uint32, std::string>(HYD_FAILED, "");
	}








	Program::~Program()
	{
		DeleteProgram();
	}

	Program::Program(const Program& pOther)
	{
		ASSERT(false, "Not Implemented");
	}

	Program::Program(Program&& pOther)
		:m_ProgramObject(pOther.m_ProgramObject), m_Name(std::move(pOther.m_Name)), m_Table(std::move(pOther.m_Table))
	{
		pOther.m_ProgramObject = nullptr;
	}

	Program& Program::operator=(const Program& pOther)
	{
		ASSERT(false, "Not Implemented");
		return *this;
	}

	Program& Program::operator=(Program&& pOther)
	{
		m_Name  = std::move(pOther.m_Name);
		m_Table = std::move(pOther.m_Table);
		m_ProgramObject = pOther.m_ProgramObject;
		pOther.m_ProgramObject = nullptr;
		return *this;
	}

	uint32 Program::CreateProgram(std::string pName)
	{
		m_Name = pName;

		return HYD_OK;
	}


	uint32 Program::AttachShader(uint64 pId, const Shader& pShader)
	{
		m_Table.PushShader(pId, pShader);

		return HYD_OK;
	}


	uint32 Program::AttachShader(uint64 pId, Shader&& pShader)
	{
		m_Table.PushShader(pId, std::move(pShader));

		return HYD_OK;
	}

	std::pair<uint32, std::string> Program::LinkProgram()
	{
		return std::pair<uint32, std::string>();
	}

	uint32 Program::DeleteProgram()
	{

		if (m_ProgramObject == nullptr)
			return HYD_FAILED;

		return HYD_FAILED;
	}

	void Program::Bind() const
	{

	}

	void Program::Unbind() const
	{

	}


	void Program::SetUniformFloat1(const char* pName, float pValue) const
	{

	}

	void Program::SetUniformFloat2(const char* pName, float pValue1, float pValue2) const
	{

	}

	void Program::SetUniformFloat3(const char* pName, float pValue1, float pValue2, float pValue3) const
	{

	}

	void Program::SetUniformInt1(const char* pName, int32 pValue) const
	{

	}

	void Program::SetUniformInt2(const char * pName, int32 pValue1, int32 pValue2) const
	{

	}

	void Program::SetUniformInt3(const char * pName, int32 pValue1, int32 pValue2, int32 pValue3) const
	{

	}

	void Program::SetUniformMat4(const char* pName, float* pValue, bool pTranspose) const
	{

	}



//Shader Pool:



	ShaderPool::~ShaderPool()
	{
		m_ActiveProgram.Reset();
		m_RecentProgram.Reset();
		for (auto& ShaderProgram : m_ShaderPrograms)
		{
			ShaderProgram.Object.DeleteProgram();
		}
		m_ShaderPrograms.Shutdown();
	}

	ShaderTable ShaderPool::LoadShaders(const std::string& pPath)
	{
		Buffer      Content = ReadShaderFile(pPath);
		ShaderTable Table   = Process(Content);
		CompileShaders(Table);
		return std::move(Table);
	}

	Shader  ShaderPool::CreateShaderFromSource(ShaderType pType, const std::string& pSource, const std::string& pName)
	{
		return Shader();
	}

	Instance<Program> ShaderPool::CreateProgram(const ShaderTable& pTable, std::string pName)
	{
		Instance<Program> program = m_ShaderPrograms.Resource();
		program->CreateProgram(pName);
		for (auto& shader : pTable.m_Table)
		{
			program->AttachShader(shader.first, shader.second);
		}
		auto Status = program->LinkProgram();

		if (Status.first == HYD_OK)
			Log::SetInfo("Program Linked Successfully");
		

		else
			Log::SetError(Log::FmtStr("Failed to Link Shader Program. Log:%s", Status.second.c_str()));
		

		return std::move(program);
	}


	bool ShaderPool::CompareStr(std::string_view pFirst, std::string_view pSecond)
	{
		if (pFirst.length() != pSecond.length())
			return false;
		for (uint64 Ind = 0; Ind < pFirst.length(); Ind++)
		{
			if (abs(pFirst[Ind] - pSecond[Ind]) != 32 && pFirst[Ind] - pSecond[Ind] != 0)
				return false;
		}
		return true;
	}

	std::pair<Token, std::string> ShaderPool::GetToken(const std::string & pLine)
	{
		uint64 BegTokenPos = pLine.find('@') + 1;
		uint64 EndTokenPos = pLine.find(':');

		std::string_view TokenLabel(&pLine[BegTokenPos], EndTokenPos - BegTokenPos);
		std::string  Data = pLine.substr(EndTokenPos + 1);


		//Specifing Token: 
		if (ShaderPool::CompareStr(TokenLabel, "Type"))
			return std::pair<Token, std::string>(TOKEN_TYPE, std::move(Data));
		if (ShaderPool::CompareStr(TokenLabel, "Name"))
			return std::pair<Token, std::string>(TOKEN_NAME, std::move(Data));
		if (ShaderPool::CompareStr(TokenLabel, "Id"))
			return std::pair<Token, std::string>(TOKEN_ID, std::move(Data));


		return std::pair<Token, std::string>(TOKEN_UNKNOWN, "");
	}

	uint64 ShaderPool::ExtractID(std::string pIdStr)
	{
		return std::stoi(pIdStr);
	}

	ShaderType ShaderPool::GetShaderType(std::string pType)
	{
		if (CompareStr(pType, "Vertex"))
			return HYD_VERTEX_SHADER;

		else if (CompareStr(pType, "Fragment") || CompareStr(pType, "Pixel"))
			return HYD_FRAGMENT_SHADER;

		else if (CompareStr(pType, "Geometry"))
			return HYD_GEOMETRY_SHADER;

		return HYD_UNSPECIFIED;
	}


	Buffer ShaderPool::ReadShaderFile(const std::string & pPath)
	{
		//Read the Content of the file:
		Buffer Content = FileSys::ReadFile(pPath);
		//Check
		if (Content.Length() == 0)
		{
			Log::SetError(Log::FmtStr("Unable to Read File at %s", pPath));
			return Buffer();
		}
		return Content;
	}

	ShaderTable ShaderPool::Process(Buffer& pData)
	{
		ShaderTable Table;

		uint64 Length = pData.Length();

		std::string Name;
		uint64	ID;
		std::string Line;
		Ptr<std::string> Source = nullptr;

		for (uint64 Ind = 0; Ind <= Length; ++Ind)
		{
			//When Line ends:
			if (Ind == Length || *pData[Ind] == '\r' && *pData[Ind + 1] == '\n')
			{
				//Filter Comments
				if (Line.find("//") < Line.length())
				{
					Line.clear();
					continue;
				}

				//Token Processing:
				if (Line.find("@") < Line.length())
				{
					auto Token = ShaderPool::GetToken(Line);


					if (Token.first == TOKEN_NAME)
					{
						Name = Token.second;
					}

					else if (Token.first == TOKEN_ID)
					{
						ID = ExtractID(Token.second);
						if (Table.Exist(ID))
						{
							Log::SetError(Log::FmtStr("Duplicated Shader ID:%i", ID));
							return Table;
						}
					}

					else if (Token.first == TOKEN_TYPE)
					{
						//Vertex Shader Creation:
						ShaderType Type = GetShaderType(Token.second);
						Shader shader(Type, Name);
						Table.m_Table.emplace(ID, std::move(shader));
						Source = &Table.m_Table[ID].GetSource();
					}

					//bad token, Panic
					else
					{
						Log::SetWarning("Invalid Token used in shader source code");
					}



					Line.clear();
					continue;
				}


				//If we're here, means its shader source code
				if (Source == nullptr){
					Log::SetError("Invalid Shader Code");
					return Table;
				}

				(*Source) += Line+"\r\n";
				Line.clear();
				Ind++;
			}

			else
			{
				Line += (*pData[Ind]);
			}
		}
		return Table;
	}

	uint32 ShaderPool::CompileShaders(ShaderTable& pTable)
	{
		Log::SetInfo(Log::FmtStr("Compiling %i Shaders", pTable.Count()));
		for (auto& shader : pTable.m_Table)
		{
			std::pair<uint32, std::string> Res = shader.second.CompileShader();
			if (Res.first != HYD_OK)
			{
				Log::SetInfo(Res.second);
			}
			else
			{
				Log::SetInfo(Log::FmtStr("Shader %s Compiled Successfully", shader.second.GetName().c_str()));
			}
		}
		return HYD_OK;
	}


//Binding

	uint32 ShaderPool::BindShaderProgram(Instance<Program>& pProgram)
	{
		pProgram->Bind();
		m_RecentProgram = pProgram;
		return HYD_OK;
	}

	uint32 ShaderPool::BindRecentShader()
	{
		if (m_RecentProgram.IsNull())
			return HYD_INVALID_TARGET;
		m_ActiveProgram = m_RecentProgram;
		m_RecentProgram->Bind();
		return HYD_OK;
	}

	uint32 ShaderPool::UnbindCurrentShader()
	{
		m_RecentProgram = m_ActiveProgram;
		m_ActiveProgram->Unbind();
		return HYD_OK;
	}



//BroadCast

	uint32 ShaderPool::BroadCastUniformF1(const char* pName, float pValue)
	{
		for (auto& shader : m_ShaderPrograms)
		{
			shader.Object.SetUniformFloat1(pName, pValue);
		}
		return HYD_OK;
	}

	uint32 ShaderPool::BroadCastUniformF2(const char* pName, float pValue1, float pValue2)
	{
		for (auto& shader : m_ShaderPrograms)
		{
			shader.Object.SetUniformFloat2(pName, pValue1, pValue2);
		}
		return HYD_OK;
	}

	uint32 ShaderPool::BroadCastUniformF3(const char* pName, float pValue1, float pValue2, float pValue3)
	{
		for (auto& shader : m_ShaderPrograms)
		{
			shader.Object.SetUniformFloat3(pName, pValue1, pValue2, pValue3);
		}
		return HYD_OK;
	}

	uint32 ShaderPool::BroadCastUniformI1(const char* pName, int pValue)
	{
		for (auto& shader : m_ShaderPrograms)
		{
			shader.Object.SetUniformInt1(pName, pValue);
		}
		return HYD_OK;
	}

	uint32 ShaderPool::BroadCastUniformI2(const char* pName, int pValue1, int pValue2)
	{
		for (auto& shader : m_ShaderPrograms)
		{
			shader.Object.SetUniformInt2(pName, pValue1, pValue2);
		}
		return HYD_OK;
	}

	uint32 ShaderPool::BroadCastUniformI3(const char* pName, int pValue1, int pValue2, int pValue3)
	{
		for (auto& shader : m_ShaderPrograms)
		{
			shader.Object.SetUniformInt3(pName, pValue1, pValue2, pValue3);
		}
		return HYD_OK;
	}

	uint32 ShaderPool::BroadCastUniformMat4(const char* pName, float* pValue)
	{
		for (auto& shader : m_ShaderPrograms)
		{
			shader.Object.SetUniformMat4(pName, pValue);
		}
		return HYD_OK;
	}





	ShaderTable::ShaderTable(const ShaderTable& pOther)
		: m_Table(pOther.m_Table)
	{}

	ShaderTable::ShaderTable(ShaderTable&& pOther)
		: m_Table(std::move(pOther.m_Table))
	{}

	ShaderTable& ShaderTable::operator=(const ShaderTable& pOther)
	{
		if (&pOther == this) { return *this; }
		m_Table = pOther.m_Table;
		return *this;
	}

	ShaderTable& ShaderTable::operator=(ShaderTable&& pOther)
	{
		if (&pOther == this) { return *this; }
		m_Table = std::move(pOther.m_Table);
		return *this;
	}

	uint32 ShaderTable::PushShader(uint64 pID, Shader pShader)
	{
		if (pID < 0)
			return HYD_INVALID_VALUE;
		
		if (Exist(pID))
			return HYD_FAILED;

		m_Table.emplace(pID, pShader);
		return HYD_OK;
	}

	Shader& ShaderTable::operator[](uint64 pID)
	{
		ASSERT(Exist(pID), "Invalid Shader ID");
		return m_Table[pID];
	}







};

*/