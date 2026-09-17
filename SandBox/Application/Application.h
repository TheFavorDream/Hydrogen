#pragma once

#include "../../Hydrogen//Hydrogen.h"
#include "ApplicationLayer.h"
#include "UI/Editor.h"

class Application : public Hydrogen::Core
{
public:
	 Application();
	~Application();

	void Run();


private:
	Ptr<Hydrogen::Layer> m_AppLayer = nullptr;
	Ptr<Hydrogen::Layer> m_Editor   = nullptr;

};

