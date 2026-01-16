#pragma once

#include "Common.h"
#include "3rdParty/imgui/imgui.h"
#include "Math/Vector/VectorDef.h"

namespace Hydrogen
{
	class Element
	{
	public:

		virtual void Render() = 0;


		inline std::string& GetLabel() { return m_Label; }
		inline Vec2 GetSize() { return m_Size; }
		inline Vec2 GetPosition() { return m_Position; }

	protected:
		std::string m_Label;
		Vec2 m_Size;
		Vec2 m_Position;
	};


};