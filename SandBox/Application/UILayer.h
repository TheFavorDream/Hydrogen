#pragma once

#include "Hydrogen.h"
#include <string>

class UILayer : public Hydrogen::Layer
{

public:

	void Setup() override;
	void Shutdown() override;
	void Event() override;
	void Update() override;

	void UpdateWidgetSizes();


private:

	float m_EditorHeight = 0.0f;
	float m_ControlWidth = 0.0f;

	Hydrogen::LongAverage m_DeltaTimeAvg;

	Hydrogen::VecF2 m_WindowSize;
	Hydrogen::VecF4 m_ViewPortSize;

	Hydrogen::Id m_CurrentModel = 0;
	Hydrogen::Id m_CurrentMesh  = 0;

	Hydrogen::UICore* m_UI = nullptr;
	Hydrogen::GuiWindow* m_ControlWindow = nullptr;
	Hydrogen::GuiWindow* m_EditorWindow = nullptr;
};