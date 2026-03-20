#pragma once

#include "Common.h"
#include "imgui/imgui.h"
#include "VecMath/Vector/VectorDef.h"

namespace Hydrogen
{
	class Element
	{
	public:

		HYD virtual void Render() = 0;


		HYD inline std::string& GetLabel() { return m_Label; }
		HYD inline Vec2 GetSize() { return m_Size; }
		HYD inline Vec2 GetPosition() { return m_Position; }

	protected:
		std::string m_Label;
		Vec2 m_Size;
		Vec2 m_Position;
	};


};