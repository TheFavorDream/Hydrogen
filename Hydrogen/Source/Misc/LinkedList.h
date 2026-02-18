/*
	Linked List for Resource Storage

*/

#pragma once

#include "Common.h"
#include <exception>
namespace Hydrogen
{
	

	template <class Type>
	struct Node
	{
		Type Data;
		Node* Prev = nullptr;
		Node* Next = nullptr;
	};

	template <class NodeType>
	class LinkedList
	{
	private:
		typedef NodeType* node;
		uint32 m_NodeNumbers = 0;
		node   m_Head = nullptr;
		node   m_Tail = nullptr;
	public:

		//Constructor: Initializes the List
		 LinkedList();
		//Destructor: Destroys the entire list
		 ~LinkedList();


		 LinkedList(const LinkedList& pOther) = delete;
		 LinkedList(LinkedList&& pOther);

		 LinkedList& operator=(const LinkedList& pOther) = delete;
		 LinkedList& operator=(LinkedList&& pOther);

		//Adds the new node between two existing node
		 node InsertAfter(node pNewNode,  node* pPrevNode) noexcept;
		 node InsertBefore(node pNewNode, node* pNextNode) noexcept;



		//Adds the new node to the begining
		 node AddHead(node pNewNode) noexcept;
		//Adds the new node to the end
		 node AddTail(node pNewNode) noexcept;


		//Remove node
		 node Remove(node* pNode) noexcept;
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




	template<class NodeType>
	inline LinkedList<NodeType>::LinkedList()
	{

	}

	template<class NodeType>
	inline LinkedList<NodeType>::~LinkedList()
	{
		Clear();
	}

	//Move Constructor
	template <class Type>
	inline LinkedList<Type>::LinkedList(LinkedList&& pOther)
	{
		m_Head = pOther.m_Head;
		m_Tail = pOther.m_Tail;
		m_NodeNumbers = pOther.m_NodeNumbers;

		pOther.m_Head = nullptr;
		pOther.m_Tail = nullptr;
		pOther.m_NodeNumbers = 0;
	}

	
	template <class Type>
	inline LinkedList<Type>& LinkedList<Type>::operator=(LinkedList&& pOther)
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

	template<class NodeType>
	inline NodeType* LinkedList<NodeType>::InsertAfter(NodeType* pNewNode, NodeType** pNode) noexcept
	{
		if (*pNode == nullptr)
			return nullptr;
		
		pNewNode->Next = (*pNode)->Next;
		pNewNode->Prev = (*pNode);
		
		if ((*pNode)->Next != nullptr)
			(*pNode)->Next->Prev = pNewNode;
		(*pNode)->Next = pNewNode;
		
		m_NodeNumbers++;
		return pNewNode;
	}

	template<class NodeType>
	inline NodeType* LinkedList<NodeType>::InsertBefore(NodeType* pNewNode, NodeType** pNode) noexcept
	{
		if (*pNode == nullptr)
			return nullptr;
		
		pNewNode->Next = (*pNode);
		pNewNode->Prev = (*pNode)->Prev;
		
		if ((*pNode)->Prev != nullptr)
			(*pNode)->Prev->Next = pNewNode;
		(*pNode)->Prev = pNewNode;
		
		m_NodeNumbers++;
		return pNewNode;
	}

//-------------------------Add To Head-------------------------------
	template<class NodeType>
	inline NodeType* LinkedList<NodeType>::AddHead(NodeType* pNewNode) noexcept
	{
		if (IsEmpty())
		{
			m_Head = pNewNode;

			m_Tail = m_Head;
			m_NodeNumbers++;
			return m_Head;
		}

		m_Head = InsertBefore(pNewNode, &m_Head);
		return m_Head;
	}

//--------------------------Add To Tail(End)-------------------------
	template<class NodeType>
	inline NodeType* LinkedList<NodeType>::AddTail(NodeType* pNewNode) noexcept
	{
		//If List is empty, Create the list and add data to Head:
		if (IsEmpty())
		{
			m_Head = pNewNode;
			m_Tail = m_Head;
			m_NodeNumbers++;
			return m_Tail;
		}

		m_Tail = InsertAfter(pNewNode, &m_Tail);
		return m_Tail;
	}


	template<class NodeType>
	inline NodeType* LinkedList<NodeType>::Remove(NodeType** pNode) noexcept
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

			if (Prev != nullptr)
				Prev->Next = Next;
			if (Next != nullptr)
				Next->Prev = Prev;

			if (Prev != nullptr && Next != nullptr)
				delete (*pNode);
			(*pNode) = nullptr;
		}

		m_NodeNumbers-=1;
		return m_Tail;
	}

	template<class NodeType>
	inline uint32 LinkedList<NodeType>::RemoveHead() noexcept
	{
		if (IsEmpty())
			return 1;
		Remove(&m_Head);
		return HYD_OK;
	}

	template<class NodeType>
	inline uint32 LinkedList<NodeType>::RemoveTail() noexcept
	{
		if (IsEmpty())
			return 1;
		Remove(&m_Tail);
		return 0;
	}

	template<class NodeType>
	inline uint32 LinkedList<NodeType>::Clear() noexcept
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
