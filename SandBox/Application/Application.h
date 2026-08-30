#pragma once

#include "../../Hydrogen//Hydrogen.h"
#include "ApplicationLayer.h"

class Application : public Hydrogen::Core
{
public:
	Application();
	~Application();

	void Run();


private:

	uint32 CreateRenderPass() noexcept;

private:
	Hydrogen::Layer* m_AppLayer = nullptr;
};

