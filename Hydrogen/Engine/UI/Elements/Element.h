#pragma once

#include "Common.h"
#include "imgui/imgui.h"
#include "VecMath/Vector/Vectors.h"

namespace Hydrogen
{
	class Element
	{
	public:

		HYD virtual void Render() = 0;


		HYD inline std::string& GetLabel() { return m_Label; }
		HYD inline VecF2 GetSize() { return m_Size; }
		HYD inline VecF2 GetPosition() { return m_Position; }

	protected:
		std::string m_Label;
		VecF2 m_Size;
		VecF2 m_Position;
	};


};