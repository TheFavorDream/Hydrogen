#pragma once

#include "Common.h"
#include "Core/ResourcePool.h"
#include "VecMath/Matrix/MatDef.h"
#include "VecMath/Transform/Transformation.h"


namespace Hydrogen
{

	class MatrixPool 
	{
	public:

		//Init and Shutdown
		HYD uint32 InitPool();
		HYD uint32 ShutdownPool();

		HYD Id GenerateMatrix(const Transformation& pTransform = Transformation());

		//Recalculate matrix based on Transformation
		HYD uint32 CalculateMatrix(Id pId, const Transformation& pTransform);


		HYD inline bool Exists(Id pId) { return m_Pool.Exists(pId); };

		//Get matrix
		HYD MatF4& operator[](Id pId);

		//Get Matrix raw pointer
		HYD float* GetPtr(Id pId);

	private:
		ResourcePool<MatF4> m_Pool;
	};


};