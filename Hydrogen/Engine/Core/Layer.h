#pragma once

#include "../Common.h"
#include "../Event/Event.h"

namespace Hydrogen
{

	class Layer
	{
	public:

		HYD Layer() 		 = default;
		HYD virtual ~Layer() = default;

		HYD virtual void Setup()               					 = 0;
		HYD virtual void Shutdown()            					 = 0;
		HYD virtual void Event(FrameEvent& pEvents) 			 = 0;
		HYD virtual void Update() 			   					 = 0;
		HYD virtual void Render(FrameRenderConfig& pRenderConf)  = 0;



	
	};

};