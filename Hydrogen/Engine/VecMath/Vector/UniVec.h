#pragma once
#include "../../Common.h"
#include "../../Log/Log.h"

#ifdef TEST
	#define HYD
#endif


namespace Hydrogen
{

	//UniVec represents a user-defined size vector with size t_Length and Type of 'Type'
	//UniVecs are static which implies that the length of it cannot change after is's been created.

	template <typename Type, uint32 t_Length>
	class UniVec
	{
	public:

		//Default constructor
		HYD  UniVec(Type pDefValue=Type())
		{
			for (uint32 i = 0; i < t_Length; i++)
				m_Array[i] = pDefValue;
		}
		//Default Destructor
		HYD ~UniVec()
		{
			
		}

		HYD UniVec(UniVec&& pOther)
		{
			memcpy(m_Array, pOther.m_Array, sizeof(Type)*t_Length);
		}

		//Copy constructors
		HYD UniVec(const UniVec& pOther)
		{
			memcpy(m_Array, pOther.m_Array, sizeof(Type)*t_Length);
		}


		HYD UniVec& operator=(const UniVec& pOther)
		{
			if (this != &pOther)
			{
				memcpy(m_Array, pOther.m_Array, sizeof(Type)*t_Length);
			}
			return *this;
		}

		HYD UniVec& operator=(UniVec&& pOther)
		{
			if (this != &pOther)
			{
				memcpy(m_Array, pOther.m_Array, sizeof(Type)*t_Length);
			}
			return *this;
		}



		//Operators:

		//the operator+ performs an component-wise addition between both vectors
		HYD UniVec operator+(const UniVec& pOther) const
		{
			UniVec<Type, t_Length> Res;
			for (uint32 i = 0; i < t_Length; i++)
				Res[i] = m_Array[i] + pOther.At(i);
			return Res;
		}

		//the operator- performs an component-wise subtraction between both vectors
		HYD UniVec operator-(const UniVec& pOther) const
		{
			UniVec<Type, t_Length> Res;
			for (uint32 i = 0; i < t_Length; i++)
				Res[i] = m_Array[i] - pOther.At(i);
			return Res;
		}


		HYD UniVec operator*(const Type& pScaler)
		{
			UniVec<Type, t_Length> Res;
			for (uint32 i = 0; i < t_Length; i++)
				Res[i] = m_Array[i] * pScaler;
			return Res;
		}

		//TODO: Add SIMD
		HYD Type Dot(const UniVec& pOther)
		{
			Type Product = Type();
			for (int i = 0; i < t_Length; i++)
			{
				Product += (m_Array[i] * pOther.At(i));
			}
			return Product;
		}


		HYD Type& operator[](uint32 pIndex)
		{
			//ASSERT((pIndex >= t_Length), "Out of Range");

			return m_Array[pIndex];
		}

		HYD Type At(uint32 pIndex) const
		{

			//ASSERT((pIndex >= t_Length), "Out of Range");

			return m_Array[pIndex];
		}

		HYD inline uint32 Length() const { return t_Length; };

	private:
		Type m_Array[t_Length];
	};

};