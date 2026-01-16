#pragma once

#include "Hydrogen.h"
#include "ApplicationLayer.h"
#include "UILayer.h"

class Application : public Hydrogen::Core
{
public:
	Application();
	~Application();

	void Run();

private:
	Hydrogen::Layer* m_UILayer = nullptr;
	Hydrogen::Layer* m_AppLayer = nullptr;
};

