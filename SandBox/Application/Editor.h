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


	static void GridController() noexcept; 

public:
	static float FogQuad     ;
	static float FogLinear   ;
	static float FogConstant ;
};