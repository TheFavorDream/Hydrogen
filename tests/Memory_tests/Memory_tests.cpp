#include <iostream>
#include "Core\Memory.h"

using namespace Hydrogen;



class Base
{
public:
	Base()
	{
		std::cout << "Base Created: " << this << '\n';
	}
	~Base()
	{
		std::cout << "Base Destructed: " << this << '\n';
	}

private:
	int X=0, Y=0, Z = 0;
};

int main()
{


	uint8* Data = new uint8[1024];
	delete[] Data;

	//std::cin.get();

	std::cout << "Size of :" << sizeof(Wraper<Base>) << '\n';

	std::cin.get();
	return 0;
}
