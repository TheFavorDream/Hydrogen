#pragma once

#include "../Common.h"

namespace Hydrogen
{

	class Layer
	{
	public:

		HYD Layer();
		HYD virtual ~Layer();

		HYD virtual void Setup() = 0;
		HYD virtual void Shutdown() = 0;
		HYD virtual void Event(float pDeltaTime) = 0;
		HYD virtual void Update() = 0;
		HYD virtual void Render() = 0;


	
	};

};