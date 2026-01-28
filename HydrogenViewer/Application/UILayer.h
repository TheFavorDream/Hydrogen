#pragma once

#include "Hydrogen.h"

class UILayer : public Hydrogen::Layer
{

public:

	void Setup() override;
	void Shutdown() override;
	void Event(float pDeltaTime) override;
	void Update() override;
	void Render() override;



private:

	float m_EditorHeight = 0.0f;
	float m_ControlWidth = 0.0f;

	Hydrogen::LongAverage m_DeltaTimeAvg;

	Hydrogen::Vec2 m_WindowSize;
	Hydrogen::Vec4 m_ViewPortSize;


	Hydrogen::UICore* m_UI = nullptr;
	Hydrogen::GuiWindow* m_ControlWindow = nullptr;
	Hydrogen::GuiWindow* m_EditorWindow = nullptr;
};