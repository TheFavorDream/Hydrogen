#pragma once

#include "Hydrogen.h"
#include "ApplicationLayer.h"

class Application : public Hydrogen::Core
{
public:
	Application();
	~Application();

	void Run();

private:
	Hydrogen::Layer* m_AppLayer = nullptr;
};

