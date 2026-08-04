#pragma once

#include "Hydrogen.h"
#include <iostream>

using namespace Hydrogen;


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
	Hydrogen::Instance<Hydrogen::Scene> m_Scene;
};