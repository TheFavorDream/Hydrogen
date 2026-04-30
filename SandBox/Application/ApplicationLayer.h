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

	Hydrogen::Id model = 0;
	Hydrogen::Ptr<Hydrogen::Scene> m_Scene = nullptr;
};