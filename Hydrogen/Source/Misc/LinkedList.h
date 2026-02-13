/*
	Linked List for Resource Storage

*/

#pragma once

#include "Common.h"
#include <exception>
namespace Hydrogen
{
	template <typename Type>
	struct Node
	{
		Node() = default;
		~Node() {
#ifndef DIST 
			std::cout << "Deleted Node:" << this << "\t Value:" << Data << '\n';
#endif
		
		}

		Type Data;
		Node* Prev = nullptr;
		Node* Next = nullptr;
	};


	template <class Type, uint32 Size = 0>
	class LinkedList
	{
	private:
		typedef Node<Type>* node;
		uint32 m_NodeNumbers = 0;
		node   m_Head = nullptr;
		node   m_Tail = nullptr;
	public:

		//Constructor: Initializes the List
		 LinkedList();
		//Destructor: Destroys the entire list
		 ~LinkedList();


		 LinkedList(const LinkedList& pOther);
		 LinkedList(LinkedList&& pOther);

		 LinkedList& operator=(const LinkedList& pOther);
		 LinkedList& operator=(LinkedList&& pOther);

		//Adds the new node between two existing node
		 node InsertAfter(const Type& pData,  node* pNode) noexcept;
		 node InsertAfter(Type&& pData, node* pNode) noexcept;

		 node InsertBefore(const Type& pData, node* pNode) noexcept;
		 node InsertBefore(Type&& pData, node* pNode) noexcept;


		//Adds the new node to the begining
		 node AddHead(const Type& pData) noexcept;
		 node AddHead(Type&& pData) noexcept; //Move

		//Adds the new node to the end
		 node AddTail(const Type& pData) noexcept;
		 node AddTail(Type&& pData) noexcept; //Move


		//Remove node
		 node Remove(Node<Type>** pNode) noexcept;
		//Remove the head
		 uint32 RemoveHead() noexcept;
		//Remove the tail
		 uint32 RemoveTail() noexcept;

		 uint32 Clear() noexcept;

		node operator[](uint32 pIndex)
		{
			return GetNodeByIndex(pIndex);
		}

		node GetNodeByIndex(uint32 pIndex)
		{
			if (pIndex >= m_NodeNumbers)
				throw std::exception::exception("Out Of Range");

			node Header = m_Head;
			for (int i = 0; i < pIndex && Header != nullptr; i++)
			{
				Header = Header->Next;
			}
			return Header;
		}

		inline const node GetHead() { return m_Head; }
		inline const node GetTail() { return m_Tail; }
		inline bool IsEmpty() { return (m_Head == nullptr); }
		inline uint32 GetSize() { return m_NodeNumbers; }

#ifndef DIST
		void PrintList()
		{
			node Header = m_Head;

			while (Header != nullptr)
			{
				std::cout << Header->Data << ',';
				Header = Header->Next;
			}
		}
#endif


	};




	template<class Type, uint32 Size>
	inline LinkedList<Type, Size>::LinkedList()
	{
		node* Header = &m_Head;
		node Prev = nullptr;
		for (int i = 0; i < Size; i++)
		{

			*Header = new Node<Type>();
			(*Header)->Prev = Prev;
			Prev = *Header;
			Header = &(*Header)->Next;
		}
		m_Tail = Prev;
		m_NodeNumbers = Size;
	}

	template<class Type, uint32 Size>
	inline LinkedList<Type, Size>::~LinkedList()
	{
		Clear();
	}

	//Copy Constructor
	template <class Type, uint32 Size>
	inline LinkedList<Type, Size>::LinkedList(const LinkedList& pOther)
	{
		//Free Space if it has been allocated
		Clear();

		node  ReadHeader = pOther.m_Head;
		node* WriteHeader = &m_Head;
		node Prev = nullptr;

		//Allocate memory and copy data into new node
		while (ReadHeader != nullptr)
		{

			(*WriteHeader) = new Node<Type>();
			(*WriteHeader)->Data = ReadHeader->Data;
			(*WriteHeader)->Prev = Prev;

			Prev = (*WriteHeader);
			(WriteHeader) = &((*WriteHeader)->Next);

			ReadHeader = ReadHeader->Next;
		}

		m_Tail = Prev;
		m_NodeNumbers = pOther.m_NodeNumbers;
	}

	//Move Constructor
	template <class Type, uint32 Size>
	inline LinkedList<Type, Size>::LinkedList(LinkedList&& pOther)
	{
		m_Head = pOther.m_Head;
		m_Tail = pOther.m_Tail;
		m_NodeNumbers = pOther.m_NodeNumbers;

		pOther.m_Head = nullptr;
		pOther.m_Tail = nullptr;
		pOther.m_NodeNumbers = 0;
	}

	template <class Type, uint32 Size>
	inline LinkedList<Type, Size>& LinkedList<Type, Size>::operator=(const LinkedList& pOther)
	{
		//Free Space if it has been allocated
		Clear();

		node  ReadHeader = pOther.m_Head;
		node* WriteHeader = &m_Head;
		node Prev = nullptr;

		//Allocate memory and copy data into new node
		while (ReadHeader != nullptr)
		{

			(*WriteHeader) = new Node<Type>();
			(*WriteHeader)->Data = ReadHeader->Data;
			(*WriteHeader)->Prev = Prev;
			 
			Prev = (*WriteHeader);
			(WriteHeader) = &((*WriteHeader)->Next);

			ReadHeader = ReadHeader->Next;
		}

		m_Tail = Prev;
		m_NodeNumbers = pOther.m_NodeNumbers;
		return *this;
	}

	template <class Type, uint32 Size>
	inline LinkedList<Type, Size>& LinkedList<Type, Size>::operator=(LinkedList&& pOther)
	{
		if (this != &pOther)
		{
			m_Head		  = pOther.m_Head;
			m_Tail		  = pOther.m_Tail;
			m_NodeNumbers = pOther.m_NodeNumbers;
		
			pOther.m_Head        = nullptr;
			pOther.m_Tail        = nullptr;
			pOther.m_NodeNumbers = 0;
		}
		return *this;
	}

//------------------Insert Functionality--------------------------------------------

	template<class Type, uint32 Size>
	inline Node<Type>* LinkedList<Type, Size>::InsertAfter(const Type& pData, Node<Type>** pNode) noexcept
	{
		if (*pNode == nullptr)
			return nullptr;

		node NewNode = new Node<Type>();
		NewNode->Data = pData;

		NewNode->Next = (*pNode)->Next;
		NewNode->Prev = (*pNode);

		if ((*pNode)->Next != nullptr)
			(*pNode)->Next->Prev = NewNode;
		(*pNode)->Next = NewNode;

		m_NodeNumbers++;
		return NewNode;
	}

	template<class Type, uint32 Size>
	inline Node<Type>* LinkedList<Type, Size>::InsertAfter(Type&& pData, Node<Type>** pNode) noexcept
	{
		if (*pNode == nullptr)
			return nullptr;

		node NewNode = new Node<Type>();
		NewNode->Data = std::move(pData);

		NewNode->Next = (*pNode)->Next;
		NewNode->Prev = (*pNode);

		if ((*pNode)->Next != nullptr)
			(*pNode)->Next->Prev = NewNode;
		(*pNode)->Next = NewNode;

		m_NodeNumbers++;
		return NewNode;
	}


	template<class Type, uint32 Size>
	inline Node<Type>* LinkedList<Type, Size>::InsertBefore(const Type& pData, Node<Type>** pNode) noexcept
	{
		if (*pNode == nullptr)
			return nullptr;

		node NewNode = new Node<Type>();
		NewNode->Data = pData;

		NewNode->Next = (*pNode);
		NewNode->Prev = (*pNode)->Prev;

		if ((*pNode)->Prev != nullptr)
			(*pNode)->Prev->Next = NewNode;
		(*pNode)->Prev = NewNode;

		m_NodeNumbers++;
		return NewNode;
	}

	template<class Type, uint32 Size>
	inline Node<Type>* LinkedList<Type, Size>::InsertBefore(Type&& pData, Node<Type>** pNode) noexcept
	{
		if (*pNode == nullptr)
			return nullptr;

		node NewNode = new Node<Type>();
		NewNode->Data = std::move(pData);

		NewNode->Next = (*pNode);
		NewNode->Prev = (*pNode)->Prev;

		if ((*pNode)->Prev != nullptr)
			(*pNode)->Prev->Next = NewNode;
		(*pNode)->Prev = NewNode;

		m_NodeNumbers++;
		return NewNode;
	}

//--------------------------------------------------------------------------------------



//-------------------------Add To Head-------------------------------
	template<class Type, uint32 Size>
	inline Node<Type>* LinkedList<Type, Size>::AddHead(const Type& pData) noexcept
	{
		if (IsEmpty())
		{
			m_Head = new Node<Type>();
			m_Head->Data = pData;

			m_Tail = m_Head;
			m_NodeNumbers++;
			return m_Head;
		}

		m_Head = InsertBefore(pData, &m_Head);
		return m_Head;
	}

	template<class Type, uint32 Size>
	inline Node<Type>* LinkedList<Type, Size>::AddHead(Type&& pData) noexcept
	{
		if (IsEmpty())
		{
			m_Head = new Node<Type>();
			m_Head->Data = pData;

			m_Tail = m_Head;
			m_NodeNumbers++;
			return m_Head;
		}

		m_Head = InsertBefore(std::move(pData), &m_Head);
		return m_Head;
	}

//-------------------------------------------------------------------

//--------------------------Add To Tail(End)-------------------------
	template<class Type, uint32 Size>
	inline Node<Type>* LinkedList<Type, Size>::AddTail(const Type& pData) noexcept
	{
		//If List is empty, Create the list and add data to Head:
		if (IsEmpty())
		{
			m_Head = new Node<Type>();
			m_Head->Data = pData;

			m_Tail = m_Head;
			m_NodeNumbers++;
			return m_Tail;
		}

		m_Tail = InsertAfter(pData, &m_Tail);
		return m_Tail;
	}

	template<class Type, uint32 Size>
	inline Node<Type>* LinkedList<Type, Size>::AddTail(Type&& pData) noexcept
	{
		//If List is empty, Create the list and add data to Head:
		if (IsEmpty())
		{
			m_Head = new Node<Type>();
			m_Head->Data = pData;

			m_Tail = m_Head;
			m_NodeNumbers++;
			return m_Tail;
		}

		m_Tail = InsertAfter(std::move(pData), &m_Tail);
		return m_Tail;
	}
//-------------------------------------------------------------------



	template<class Type, uint32 Size>
	inline  Node<Type>* LinkedList<Type, Size>::Remove(Node<Type>** pNode) noexcept
	{
		if ((*pNode) == m_Head && (*pNode) != m_Tail)
		{
			node Next = m_Head->Next;
			if (Next != nullptr)
			{
				Next->Prev = nullptr;
			}
			else
				m_Tail = nullptr;

			delete m_Head;
			m_Head = Next;

			(*pNode) = nullptr;
		}

		else if ((*pNode) == m_Tail)
		{
			node Prev = (*pNode)->Prev;
			if (Prev != nullptr)
				Prev->Next = nullptr;
			else
				m_Head = nullptr; // We Delete the Entire List

			delete m_Tail;
			m_Tail = Prev;

			(*pNode) = nullptr;
		}

		else
		{
			node Prev = (*pNode)->Prev;
			node Next = (*pNode)->Next;

			Prev->Next = Next;
			Next->Prev = Prev;

			delete (*pNode);
			(*pNode) = nullptr;
		}

		m_NodeNumbers-=1;
		return m_Tail;
	}

	template<class Type, uint32 Size>
	inline uint32 LinkedList<Type, Size>::RemoveHead() noexcept
	{
		if (IsEmpty())
			return 1;
		Remove(&m_Head);
		return HYD_OK;
	}

	template<class Type, uint32 Size>
	inline uint32 LinkedList<Type, Size>::RemoveTail() noexcept
	{
		if (IsEmpty())
			return 1;
		Remove(&m_Tail);
		return 0;
	}

	template<class Type, uint32 Size>
	inline uint32 LinkedList<Type, Size>::Clear() noexcept
	{
		node Header = m_Tail;
		while (Header != nullptr)
		{
			node Prev = (Header)->Prev;
			delete Header;
			Header = Prev;
		}
		m_Head = nullptr;
		m_Tail = nullptr;
		m_NodeNumbers = 0;
		return HYD_OK;
	}
	};
