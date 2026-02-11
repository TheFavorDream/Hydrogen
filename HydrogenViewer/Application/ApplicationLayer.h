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

	void Event(float pDeltaTime) override;
	void Update() override;
	void Render() override;


private:

	Hydrogen::Camera m_Camera;
	Hydrogen::Shader m_BasicShader;

	//Models
	std::unique_ptr<Hydrogen::Model> m_Interior;
	std::unique_ptr<Hydrogen::Model> m_Character;



};