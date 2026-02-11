#include "Misc/LinkedList.h"
#include <iostream>

int main(int argc, char** arvg)
{

	Hydrogen::LinkedList<uint32> List;
	
	for (int i = 0; i < 100; i++)
	{
		List.AddTail(i);
	}
	std::cout << "Hello, World";

	return 0;
}