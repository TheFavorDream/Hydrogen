#pragma once

#include "../Common.h"
#include "Primitive.h"
#include "VecMath/Transform/Transformation.h"
#include "../Camera/Camera.h"
#include "Core/ResourcePool.h"
#include "Xenon/include/Xenon.h"

namespace Hydrogen
{




	class Mesh
	{
	public:

		//Creates a mesh obhect from given xenon::mesh
		static Mesh CreateGLTFMesh(const Xenon::Mesh& pMesh, GraphicsPipelineRef pPipeline);

	public:

		HYD Mesh();
		HYD Mesh(const std::string& pName, std::vector<Primitive>&& pPrimitives);

		HYD Mesh(const Mesh& pOther);
		HYD Mesh(Mesh&& pOther);

		HYD Mesh& operator=(const Mesh& pOther);
		HYD Mesh& operator=(Mesh&& pOther);
		

		HYD inline HYD_VEC<Primitive>::iterator begin() const { return m_Primitives.begin(); }
		HYD inline HYD_VEC<Primitive>::iterator end()   const { return m_Primitives.end(); }


		HYD uint32     PushPremitive(const Primitive& pPrimitive);
		HYD Primitive& GetPrimitve(uint64 pIndex);

		HYD inline const std::string GetName() { return m_Name; }

		HYD void Render(
			std::vector<Instruction>& pInstructionSet,
			UniformRef		      	  pUniform,
			LightCollection&          pLights,
			const Transformation& 	  pTransform = Transformation()
		) noexcept;


	private:
		std::string				m_Name; 
		mutable HYD_VEC<Primitive>      m_Primitives;
	};

};