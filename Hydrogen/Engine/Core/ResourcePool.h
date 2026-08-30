#pragma once

#include "../Common.h"
#include "Memory/Memory.h"
#include <queue>
#include "../Log/Log.h"
#include <optional>

#ifdef TEST
	#define HYD 
	#define ASSERT(x)  
#endif

namespace Hydrogen
{

	#define HYD_BUCKET_SIZE 128


	template <typename T>
	struct Instance;

	template <typename Type>
	struct Block;



	template <typename Type>
	struct Block
	{

		Block()
		{
			//memset((void*)&Raw, 0, sizeof(Type));
		}

		~Block()
		{

		}

		union
		{
			Type   Object;
			uint8  Raw[sizeof(Type)];
		};
		uint64 RefCount = 0;

	};


	/*
		Bucket:
			A Bucket is a fixed-size array of 'Type' Objects
	*/

	template <typename Type>
	struct Bucket
	{
	public:

		Block<Type> m_Bucket[HYD_BUCKET_SIZE];
		uint64		m_Used = 0;
		Ptr<Bucket> m_Next = nullptr;
		Ptr<Bucket> m_Prev = nullptr;

		Ptr<ResourcePool<Type>> m_Pool = nullptr;

		Bucket(Ptr<ResourcePool<Type>> pPoolPtr):
			m_Pool(pPoolPtr)
		{
			for (uint64 Iterator = 0; Iterator < HYD_BUCKET_SIZE; ++Iterator)
			{
				m_Pool->m_Unused.push_back(std::pair<Ptr<Bucket<Type>>, uint64>(this, Iterator));
			}
		}
		~Bucket()
		{
			uint64 Iter = 0;
			for (auto& Blk : m_Pool->m_Unused)
			{
				if (Blk.first == this)
				{
					m_Pool->m_Unused.erase(m_Pool->m_Unused.begin()+Iter);
					Iter -= 1;
				}
				Iter += 1;
			}

		}

		Ptr<Block<Type>> RetriveBlock(uint64 pIndex)
		{
			ASSERT(pIndex < HYD_BUCKET_SIZE, "Index Out of Bound");
			return (&m_Bucket[pIndex]);
		}

		uint64 DropRefCount(uint64 pIndex)
		{
			ASSERT(pIndex < HYD_BUCKET_SIZE, "Index Out of Bound");
			return --m_Bucket[pIndex].RefCount;
		}

		uint64 RiseRefCount(uint64 pIndex)
		{
			ASSERT(pIndex < HYD_BUCKET_SIZE, "Index Out of Bound");
			return ++m_Bucket[pIndex].RefCount;
		}

		void ResetObject(uint64 pIndex)
		{
			ASSERT(pIndex < HYD_BUCKET_SIZE, "Index Out of Bound");
			m_Bucket[pIndex].Object.~Type(); //explicitly calls the destructor
			m_Bucket[pIndex].RefCount = 0;
			memset((void*)&m_Bucket[pIndex].Raw[0], 0, sizeof(Type));
			m_Pool->m_Unused.push_front(std::pair<Ptr<Bucket<Type>>, uint64>(this, pIndex));
			m_Pool->m_Used -= 1;
			m_Used -= 1;

		}
	};

	/*
		Resource Pool:
			Resource Pool template class contains a linked-list of buckets and keeps track of creating/destroying buckets to the chain.
			when a bucket gets filled compeletly, a new bucket is created.

			Reusing Objects:
				when an object gets delete via the refcount reaching zero, its destructor gets called, and an reference to it gets pushed to unused objects queue.
				whenever engine needed a new object, it looks into unused objects queue and reinitializes one.

	*/

	template <typename Type>
	class ResourcePool
	{
	private:
		//Iterator Implementation:

		struct Iterator
		{
			using iterator_category = std::forward_iterator_tag;
			using difference_type   = std::ptrdiff_t;
			using value_type = Block<Type>;
			using pointer    = Block<Type>*;
			using reference  = Block<Type>&;

			Ptr<Bucket<Type>> m_Bucket;
			uint64			  m_TotalUnvisited  = 0;
			uint64			  m_BucketUnvisited = 0;
			uint64			  m_BlkIndex        = 0;


			Iterator(Ptr<Bucket<Type>> pPtr, uint64 pTotalUnVisited, uint64 pBucketUnVisited) :
				m_Bucket(pPtr), m_TotalUnvisited(pTotalUnVisited), m_BucketUnvisited(pBucketUnVisited)
			{

			}


			reference operator*() const { return m_Bucket->m_Bucket[m_BlkIndex]; }
			pointer   operator->() { 
				return &m_Bucket->m_Bucket[m_BlkIndex];
			}


			//Prefix incrament
			Iterator& operator++() {

				do
				{
					m_BlkIndex += 1;
				} while (m_Bucket->m_Bucket[m_BlkIndex].RefCount == 0);

				--m_BucketUnvisited;
				--m_TotalUnvisited;

				if (m_BucketUnvisited == 0 && m_TotalUnvisited > 0)
				{
					m_Bucket          = m_Bucket->m_Next;
					m_BucketUnvisited = m_Bucket->m_Used;
					m_BlkIndex		  = 0;
				}
				return *this;
			}


			friend bool operator== (const Iterator& a, const Iterator& b) {
				return (a.m_Bucket == b.m_Bucket) && (a.m_TotalUnvisited == b.m_TotalUnvisited);
			};
			friend bool operator!= (const Iterator& a, const Iterator& b) {
				return (a.m_Bucket != b.m_Bucket) && (a.m_TotalUnvisited != b.m_TotalUnvisited);
			};


		};

	public:


		ResourcePool() = default;

	   ~ResourcePool()
		{
		   Shutdown();
		}

	    ResourcePool(const ResourcePool& pOther)            = delete;
		ResourcePool& operator=(const ResourcePool& pOther) = delete;
		
		ResourcePool(ResourcePool&& pOther)
		{
			m_Head = pOther.m_Head;
			m_Tail = pOther.m_Tail;
			m_Used = pOther.m_Used;
			m_BucketCount = pOther.m_BucketCount;
			m_Unused      = std::move(pOther.m_Unused);

			pOther.m_Head = nullptr;
			pOther.m_Tail = nullptr;
			pOther.m_Used = 0;
			pOther.m_BucketCount = 0;
		}



		//Move asignment
		ResourcePool& operator=(ResourcePool&& pOther)
		{
			if (this == &pOther)
				return *this;

			m_Head = pOther.m_Head;
			m_Tail = pOther.m_Tail;
			m_Used = pOther.m_Used;
			m_BucketCount = pOther.m_BucketCount;
			m_Unused      = std::move(pOther.m_Unused);

			pOther.m_Head = nullptr;
			pOther.m_Tail = nullptr;
			pOther.m_Used = 0;
			pOther.m_BucketCount = 0;


			return *this;
		}



		//Init & Shutdown
		//Init Allocats a chunk of memory with size: sizeof(Type) * pCapacity. 
		uint32 Init(uint64 pSize = 0)
		{
			if (!pSize)
				return HYD_OK;

			for (uint64 Iter = 0; Iter < pSize; ++Iter)
				PushBucket();

			return HYD_OK;
		}

		//Deallocates the memory reserved by pool entirly
		uint32 Shutdown()
		{
			if (!m_BucketCount)
				return HYD_OK;

			if (m_Used > 0)
				return HYD_FAILED; //cannot delete buckets when begin used by active instances

			while (m_Tail != nullptr)
			{
				PopBucket();
			}

			m_Head = nullptr;
			m_Tail = nullptr;
			m_Used = 0;
			return HYD_OK;
		}


		inline uint64 PoolSizeInBytes() { return (m_BucketCount * sizeof(Bucket<Type>)); }

		//Object-wise operations:

		//lvalue: gets copied
		Instance<Type> PushObject(const Type&  pObject)
		{
			//setup the instance
			std::pair<Ptr<Bucket<Type>>, uint64> BlockRef = FindEmptyBlock();

			Ptr<Block<Type>> CBlock = BlockRef.first->RetriveBlock(BlockRef.second);

			Ptr<Type> ptr = new(CBlock->Raw) Type();
			(*ptr) = pObject;

			Instance<Type> NewIns;
			NewIns.m_Bucket = BlockRef.first;
			NewIns.m_Index  = BlockRef.second;
			NewIns.m_Obj	= ptr;
			BlockRef.first->RiseRefCount(BlockRef.second);
			BlockRef.first->m_Used++;

			m_Used++;

			return std::move(NewIns);
		}
		
		//rvalue: gets moved
		Instance<Type> PushObject(Type&& pObject)
		{
			//setup the instance
			std::pair<Ptr<Bucket<Type>>, uint64> BlockRef = FindEmptyBlock();

			Ptr<Block<Type>> CBlock = BlockRef.first->RetriveBlock(BlockRef.second);

			Ptr<Type> ptr = new(CBlock->Raw) Type();
			(*ptr) = std::move(pObject);

			Instance<Type> NewIns;
			NewIns.m_Bucket = BlockRef.first;
			NewIns.m_Index  = BlockRef.second;
			NewIns.m_Obj    = &CBlock->Object;
			BlockRef.first->RiseRefCount(BlockRef.second);
			BlockRef.first->m_Used++;

			m_Used++;

			return std::move(NewIns);
		}

		//Returns an instance to an unused object
		Instance<Type> Resource() 
		{
			//setup the instance
			std::pair<Ptr<Bucket<Type>>, uint64> BlockRef = FindEmptyBlock();

			Ptr<Block<Type>> CBlock = BlockRef.first->RetriveBlock(BlockRef.second);

			Ptr<Type> ptr = new(CBlock->Raw) Type();

			Instance<Type>	  NewIns;
			NewIns.m_Bucket = BlockRef.first;
			NewIns.m_Index  = BlockRef.second;
			NewIns.m_Obj	= ptr;
			
			BlockRef.first->RiseRefCount(BlockRef.second);
			BlockRef.first->m_Used++;

			m_Used++;

			return std::move(NewIns);
		}


		inline Bucket<Type>& GetHead() { return *m_Head; }


		Iterator begin() { return Iterator(m_Head, m_Used, ((m_Head)? m_Head->m_Used : 0)); }
		Iterator end()   { return Iterator(nullptr, 0, 0); }


	private:


		std::pair<Ptr<Bucket<Type>>, uint64> FindEmptyBlock()
		{
			//Find an empty block:
			if (m_Unused.empty())
				PushBucket();



			std::pair<Ptr<Bucket<Type>>, uint64> BlockRef;
			BlockRef = m_Unused.front();
			m_Unused.pop_front();

			return BlockRef;
		}

		//Create a new bucket and attach to the chain
		Ptr<Bucket<Type>> PushBucket()
		{
			if (m_Head == nullptr)
			{
				m_Head = new Bucket<Type>(this);
				m_Tail = m_Head;
				m_BucketCount += 1;
				return m_Tail;
			}

			m_Tail->m_Next		   = new Bucket<Type>(this);
			m_Tail->m_Next->m_Prev = m_Tail;
			m_Tail		           = m_Tail->m_Next;
			m_BucketCount  += 1;
			return m_Tail;
		}

		//Destroy the last bucket from the chain
		void PopBucket()
		{
			Ptr<Bucket<Type>> NTail = m_Tail->m_Prev;
			delete m_Tail;
			m_Tail = NTail;
			m_BucketCount -= 1;
			if (m_Tail)
				m_Tail->m_Next = nullptr;
		}

		friend struct Instance<Type>;

	private:
		Ptr<Bucket<Type>> m_Head = nullptr;
		Ptr<Bucket<Type>> m_Tail = nullptr;


		uint64 m_BucketCount     = 0;
		uint64 m_Used			 = 0;

		std::deque<std::pair<Ptr<Bucket<Type>>, uint64>> m_Unused;

		friend struct Bucket<Type>;
	};


	/*
	
		Instance:
			An Instance object contains information for referencing and accessing an Resource Object.
			When an instance object gets created, it increases the refcount of the resource it points to by one &
			when destroyed it drops the refcount.
			when the refcount of an object reaches zero, it dies.

	*/

	template <typename Type>
	struct Instance
	{
	public:
		
		Instance() noexcept
		{

		}

	   ~Instance() noexcept
	    {
		   Reset();
	    }

	    Instance(const Instance& pOther) noexcept
	    {
			if (pOther.m_Bucket != nullptr)
			{
				m_Bucket = pOther.m_Bucket;
				m_Index  = pOther.m_Index;
				m_Obj    = pOther.m_Obj;


				m_Bucket->RiseRefCount(m_Index);
			}
		}

		Instance(Instance&& pOther) noexcept
		{
			m_Bucket = pOther.m_Bucket;
			m_Index  = pOther.m_Index;
			m_Obj    = pOther.m_Obj;

			pOther.m_Bucket = nullptr;
			pOther.m_Index  = 0;
			pOther.m_Obj    = nullptr; 
		}

		Instance& operator=(const Instance& pOther) noexcept
		{

			if (&pOther == this)
				return *this;

			if (pOther.m_Bucket != nullptr)
			{
				m_Bucket = pOther.m_Bucket;
				m_Index = pOther.m_Index;
				m_Obj = pOther.m_Obj;

				m_Bucket->RiseRefCount(m_Index);
			}
			return *this;
		}

		Instance& operator=(Instance&& pOther) noexcept
		{
			if (&pOther == this)
				return *this;

			m_Bucket = pOther.m_Bucket;
			m_Index  = pOther.m_Index;
			m_Obj    = pOther.m_Obj;

			pOther.m_Bucket = nullptr;
			pOther.m_Index  = 0;
			pOther.m_Obj    = nullptr;

			return *this;
		}


		Type& operator*()
		{
			return *m_Obj;
		}
		Ptr<Type> operator->()
		{
			return m_Obj;
		}

		Type&	  Get() { return   *m_Obj; }
		Ptr<Type> GetPtr() { return m_Obj; }

		inline bool IsNull() 
		{
			return (m_Obj == nullptr);
		}

		void Reinstance() noexcept
		{

			m_Bucket->DropRefCount(m_Index);

			std::pair<Ptr<Bucket<Type>>, uint64> BlockRef =  m_Bucket->m_Pool->FindEmptyBlock();
			Ptr<Block<Type>> CBlock = BlockRef.first->RetriveBlock(BlockRef.second);

			//memcpy((void*)&CBlock->Raw[0], &Type(), sizeof(Type));

			CBlock->Object = *m_Obj;

			m_Bucket = BlockRef.first;
			m_Index  = BlockRef.second;
			m_Obj	 = &CBlock->Object;
			
			m_Bucket->RiseRefCount(m_Index);
			
		}

		void Reset() noexcept
		{
			if (m_Bucket)
			{
				uint64 CurrentRef = m_Bucket->DropRefCount(m_Index);
				if (!CurrentRef)
				{
					m_Bucket->ResetObject(m_Index);
				}
				m_Bucket = nullptr;
				m_Obj	 = nullptr;
			}
		}

		//Dengerous
		void ResetWithoutRefDrop() noexcept
		{
			m_Bucket = nullptr;
			m_Obj    = nullptr;
		}

		inline uint64 Index() { return m_Index; }

	private:
		Ptr<
		Bucket<Type>> m_Bucket  = nullptr;
		Ptr<Type>     m_Obj     = nullptr;
		uint64		  m_Index;

		friend class ResourcePool<Type>;
	};

};