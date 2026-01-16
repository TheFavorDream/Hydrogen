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

	Hydrogen::Shader m_BaseShader;
	Hydrogen::Shader m_CharacterShader;


	Hydrogen::ModelRef m_Model;
	Hydrogen::ModelRef m_CharcterModel;

	Hydrogen::Matrix m_ModelMatrix;
	Hydrogen::Matrix m_CharMatrix;

	Hydrogen::Texture m_CharModelBaseColor;

	Hydrogen::Camera m_Camera;

};