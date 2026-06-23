#pragma once

#include "Hydrogen.h"
#include <iostream>

class AppLayer : public Hydrogen::Layer
{
public:

	AppLayer() = default;
	~AppLayer() = default;

	void Setup() override;
	void Shutdown() override;

	void Event() override;
	void Update() override;


private:
	Hydrogen::Wraper<Hydrogen::World> m_World;
};