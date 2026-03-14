#pragma once

#include <iostream>
#include "Core/ResourcePool.h"

class Object
{

public:

	Object()
	{
		std::cout << "Object Created: address:" << this << "\tID:" << reinterpret_cast<Hydrogen::Id>(this) << '\n';
	}

	~Object()
	{
		std::cout << "Object Destructed: address:" << this  << '\n';
	}

	void Do(Hydrogen::Id pID)
	{
		std::cout << "I did something: ID:" << pID << '\n';
	}

	//Used by Resource Pool
	uint32 Destroy()
	{
		std::cout << "Object Destroyed: address:" << this << "\tID:" << reinterpret_cast<Hydrogen::Id>(this) << '\n';
		return HYD_OK;
	}

public:

	Object* Next = nullptr;
	Object* Prev = nullptr;

};