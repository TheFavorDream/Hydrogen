#include "Core/ResourcePool.h"
#include "TestObject.h"
#include <string>
using namespace Hydrogen;


struct Dummy
{
	Dummy()
	{
		std::cout << "Hi\n";
	}

	~Dummy()
	{
		std::cout << "Goodbye\n";
	}

	int a; 
};

struct Data
{

	Data()
	{
		std::cout << "Data Constructed\n";
	}

	Data(Data&& pOther)
	{
		Name = std::move(pOther.Name);
		Age  = pOther.Age;
		R	 = pOther.R;
		m_ShadersSources = std::move(pOther.m_ShadersSources);
		m_UniformLookUp  = std::move(pOther.m_UniformLookUp);
	}

	Data& operator=(Data&& pOther)
	{
		if (&pOther == this)
			return *this;
		Name = std::move(pOther.Name);
		Age  = pOther.Age;
		R    = pOther.R;

		m_ShadersSources = (pOther.m_ShadersSources);
		m_UniformLookUp  = (pOther.m_UniformLookUp);

		return *this;
	}

	~Data()
	{
		std::cout << "Data Destructor\n";
		Name.clear();
	}

	std::string Name = "kfkfkrfokrofkdpafipjwifgjerigj";
	uint64 Age = 67;
	std::unordered_map<GLenum, std::string>			m_ShadersSources;
	std::unordered_map<const char*, int32>          m_UniformLookUp; //for faster uniform access
	uint64 R = 233;
	Dummy D;
};

int main(int argc, char** arvg)
{

	ResourcePool<Data> Pool;
	std::vector<Instance<Data>> Inss;

	Data D;

	for (int i = 0; i < 20; ++i)
	{
		D.R = i;
		Inss.push_back(Pool.Resource());
	}

	Inss.clear();

	return 0;
}