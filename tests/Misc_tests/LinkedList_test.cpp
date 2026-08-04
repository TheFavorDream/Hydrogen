#include "Misc/LinkedList.h"
#include <iostream>
#include <vector>

struct Node
{
	Node(uint32 pData) : Data(pData) {}


	~Node() {
		std::cout << "Destroyed: " << this << '\t' << Data << '\n';
	}

	int Data = 0;

	Node* Prev = nullptr;
	Node* Next = nullptr;
};


int main(int argc, char** arvg)
{
	Hydrogen::LinkedList<Node> List1;
	std::vector<Node*> Ids;

	for (int i = 0; i < 10; i++)
	{
		Ids.push_back(List1.AddTail(new Node(i)));
		//List1.AddTail(new Node(i));
	}

	std::cout << "Before Deletation";
	List1.PrintList();

	for (int i = 0; i < 5; i++)
	{
		List1.Remove(&Ids[i]);
	}

	std::cout << "\nAfter Deletation";
	List1.PrintList();
	std::cin.get();

	return 0;
}