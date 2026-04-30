#include "MatrixPool.h"

namespace Hydrogen
{

	uint32 MatrixPool::InitPool()
	{
		return HYD_OK;
	}

	uint32 MatrixPool::ShutdownPool()
	{
		m_Pool.Clear();
		return HYD_OK;
	}

	uint32 MatrixPool::CalculateMatrix(Id pId, const Transformation& pTransform)
	{


		//Calculate Matrix and Store it 
		MatF4& Mat = m_Pool.GetResource(pId);
		MatF4 Res = Transformation::CalculateMatrix(pTransform);
		memcpy(Mat.GetPointer(), Res.GetPointer(), 16 * sizeof(float));


		return HYD_OK;
	}

	Id MatrixPool::GenerateMatrix(const Transformation& pTransform)
	{
		MatF4* Matrix = new MatF4(1.0f);
		Transformation::CalculateMatrix(pTransform);
		return m_Pool.Push(Matrix);
	}


	MatF4& MatrixPool::operator[](Id pId)
	{
		return m_Pool.GetResource(pId);
	}

	float* MatrixPool::GetPtr(Id pId)
	{
		return  m_Pool.GetResource(pId).GetPointer();
	}

};