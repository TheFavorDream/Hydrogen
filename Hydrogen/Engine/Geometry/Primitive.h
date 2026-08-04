#pragma once


#include "../Common.h"
#include "../Core/ResourcePool.h"
#include "../VecMath/Matrix/MatDef.h"
#include "../Render/Material/Material.h"
#include "Xenon/include/Xenon.h"
#include "Render/Shader.h"


namespace Hydrogen
{

	struct Primitive
	{
		MatF4				    m_Transform;
		Material				m_Material;


		uint16					m_RenderingMode = 4;


		HYD Primitive();
		HYD Primitive(const Primitive& pOther);
		HYD Primitive(Primitive&& pOther);


	};
};