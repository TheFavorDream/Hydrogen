#pragma once

#include "../Common.h"

namespace Hydrogen
{

	class Layer
	{
	public:

		Layer();
		virtual ~Layer();

		virtual void Setup() = 0;
		virtual void Shutdown() = 0;
		virtual void Event(float pDeltaTime) = 0;
		virtual void Update() = 0;
		virtual void Render() = 0;


	
	};

};