#include "Misc/LinkedList.h"
#include <iostream>
#include <vector>

int main(int argc, char** arvg)
{
	Hydrogen::LinkedList<uint32> List1;
	Hydrogen::LinkedList<uint32> List2;
	


	for (int i = 1; i <= 5; ++i)
	{
		
		List1.AddTail(i);
	}


	List2 = std::move(List1);

	std::cout << "List1:";
 	List1.PrintList();
	
	
	std::cout << "\nList2:";
	List2.PrintList();

	std::cout << "\nElement:";


	std::cout << List2[20]->Data << '\n';

	//std::cout << "\nAppended\n";
	//List2.PrintList();
	std::cin.get();

	return 0;
}