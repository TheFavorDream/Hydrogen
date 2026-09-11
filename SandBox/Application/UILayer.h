#pragma once
#include "../../Hydrogen/Hydrogen.h"


class UILayer : public Hydrogen::Layer
{
public:

     UILayer() = default;
    ~UILayer() = default;


	void Setup()    override;
	void Shutdown() override;

	void Event()  override;
	void Update() override;

private:
    Hydrogen::UI::Canvas m_DebugCanvas;
};