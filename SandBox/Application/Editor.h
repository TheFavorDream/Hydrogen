#pragma once
#include "../../Hydrogen/Hydrogen.h"


class Editor : public Hydrogen::Layer
{
public:

     Editor() = default;
    ~Editor() = default;


	void Setup()    override;
	void Shutdown() override;

	void Event(
		Hydrogen::FrameEvent& pEvents
	)  override;

	void Update() override;

	void Render(
		Hydrogen::FrameRenderConfig& pRenderConf
	) override; 


	static void GridController()  noexcept; 
	static void LightController() noexcept; 


public:
	static float FogQuad     ;
	static float FogLinear   ;
	static float FogConstant ;

	static Hydrogen::Ptr<Hydrogen::LightCollection> Lights; 

	bool IsGridWindow   = false;
	bool IsLightsWindow = true;


	ImFont* RegularFont = nullptr;

};