#pragma once

#include "../Common.h"
#include "Primitive.h"
#include "../VecMath/Transform/Transformation.h"
#include "../Camera/Camera.h"
#include "Core/ResourcePool.h"
#include "Xenon/include/Xenon.h"
#include "../Render/InstructionSet.h"

namespace Hydrogen
{

	/*
		Purpose: Used for Bindless rendering
	*/
	struct MeshData
	{
		MatF4 ModelMatrix;
	};

	class Mesh
	{
	public:

		//Creates a mesh obhect from given xenon::mesh
		static Mesh CreateGLTFMesh(
			const Xenon::Mesh&  pMesh,
			uint32 			    pMeshID,
			GraphicsPipelineRef pPipeline
		) noexcept;

	public:

		HYD Mesh() noexcept;
		
		HYD Mesh(
			const std::string&       pName,
			std::vector<Primitive>&& pPrimitives
		) noexcept;

		HYD Mesh(
			const Mesh& pOther
		) noexcept;

		HYD Mesh(
			Mesh&& pOther
		) noexcept;

		HYD Mesh& operator=(
			const Mesh& pOther
		) noexcept;

		HYD Mesh& operator=(
			Mesh&& pOther
		) noexcept;
		
		//Iterators:
		HYD inline HYD_VEC<Primitive>::iterator begin() { return m_Primitives.begin(); }
		HYD inline HYD_VEC<Primitive>::iterator end()   { return m_Primitives.end(); }

		/*
			Purpose: Push a new primitive object to the mesh:
		*/
		HYD void PushPremitive(
			Primitive&& pPrimitive
		) noexcept;


		/*
			Purpose: Primitive Accessing Patterns:
		*/
		HYD Primitive& GetPrimitve(
			uint32 pIndex
		) noexcept;

		HYD Primitive operator[](
			uint32 pIndex
		) noexcept;

		/*
			Purpose: Specifies if current mesh should be rendered or not
		*/

		HYD void SetMeshRenderingStatus(
			bool pEnable
		) noexcept;
		
		
		/*
			Purpose: Create a Render Instruction   
		*/

		HYD InstructionSet Render(
			const Transformation& 	  pTransform = Transformation()
		) noexcept;
		

		HYD inline const std::string GetName()  	  const { return m_Name; }
		HYD inline bool 			 IsEnable() 	  const {return m_Enable;}
		HYD inline MatF4 			 GetModelMatrix() const {return m_ModelMatrix;}
		HYD inline uint32 		     PrimitiveCount() const {return m_Primitives.size();}
		HYD inline uint32 		     GetID() 		  const {return m_ObjectID;}
		HYD inline bool&		     GetEnbaleState()       {return m_Enable;}


	private:
		std::string			    m_Name; 
		HYD_VEC<Primitive>      m_Primitives;
		uint32 				    m_ObjectID;
		MatF4					m_ModelMatrix;
		bool 				    m_Enable = true;
	};

};