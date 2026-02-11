/*
	Linked List for Resource Storage

*/

#pragma once

#include "Common.h"

namespace Hydrogen
{

	typedef intptr_t Id;
	template <class Type, uint32 Size = 0>
	class LinkedList
	{
	private:
		struct Node
		{
			Type Data;
			Node* Prev = nullptr;
			Node* Next = nullptr;
		};
		typedef Node* node;
		uint32 m_NodeNumbers = 0;
		node   m_Head = nullptr;
		node   m_Tail = nullptr;
	public:

		//Constructor: Initializes the List
		 LinkedList();
		//Destructor: Destroys the entire list
		 ~LinkedList();

		//Adds the new node to the begining
		 Id AddHead(Type pData);
		 Id AddHead(Type&& pData); //Move
		//Adds the new node to the end
		 Id AddTail(Type pData);
		 Id AddTail(Type&& pData); //Move

		//Adds the new node between two existing node
		 uint32 Insert(Type pData, node* pPrevNode);
		 uint32 Insert(Type pData, Id pPrevNode);

		//Remove node
		 uint32 Remove(Id* pNode);
		//Remove the head
		 uint32 RemoveHead();
		//Remove the tail
		 uint32 RemoveTail();

		 uint32 Clear();


		 const node GetNodeByIndex(uint32 pIndex)
		{
			if (pIndex > m_NodeNumbers - 1)
				return nullptr;

			node Header = m_Head;
			for (int i = 0; i < pIndex && Header != nullptr; i++)
			{
				Header = Header->Next;
			}
			return Header;
		}

		 Type& GetDataByID(intptr_t pID)
		{
			node Node = reinterpret_cast<node>(pID);
			return Node->Data;
		}

		 constexpr bool IsEmpty() { return (m_Head == nullptr); }
		 inline const node GetHead() { return m_Head; }
		 inline const node GetTail() { return m_Tail; }
		 inline uint32 GetSize() { return m_NodeNumbers; }




	};

	template<class Type, uint32 Size>
	inline LinkedList<Type, Size>::LinkedList()
	{
		node* Header = &m_Head;
		node Prev = nullptr;
		for (int i = 0; i < Size; i++)
		{

			*Header = new Node();
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

	template<class Type, uint32 Size>
	inline Id LinkedList<Type, Size>::AddHead(Type pData)
	{
		if (m_Head == nullptr)
		{
			m_Head = new Node();
			m_Head->Data = pData;

			m_Tail = m_Head;
			m_NodeNumbers++;
			return reinterpret_cast<intptr_t>(m_Head);
		}

		node New = new Node();
		New->Data = pData;
		New->Prev = nullptr;
		New->Next = m_Head;
		m_Head->Prev = New;

		m_Head = New;
		return reinterpret_cast<intptr_t>(m_Head);
	}

	template<class Type, uint32 Size>
	inline Id LinkedList<Type, Size>::AddTail(Type pData)
	{
		//If List is empty, Create the list and add data to Head:
		if (m_Head == nullptr)
		{
			m_Head = new Node();
			m_Head->Data = std::move(pData);

			m_Tail = m_Head;
			m_NodeNumbers++;
			return reinterpret_cast<intptr_t>(m_Head);
		}

		node Prev = m_Tail;
		m_Tail->Next = new Node();
		m_Tail = m_Tail->Next;

		m_Tail->Data = pData;
		m_Tail->Prev = Prev;
		m_Tail->Next = nullptr;

		m_NodeNumbers++;
		return reinterpret_cast<intptr_t>(m_Tail);
	}

	template<class Type, uint32 Size>
	inline Id LinkedList<Type, Size>::AddTail(Type&& pData)
	{
		//If List is empty, Create the list and add data to Head:
		if (m_Head == nullptr)
		{
			m_Head = new Node();
			m_Head->Data = std::move(pData);

			m_Tail = m_Head;
			m_NodeNumbers++;
			return reinterpret_cast<intptr_t>(m_Head);
		}

		node Prev = m_Tail;
		m_Tail->Next = new Node();
		m_Tail = m_Tail->Next;

		m_Tail->Data = std::move(pData);
		m_Tail->Prev = Prev;
		m_Tail->Next = nullptr;

		m_NodeNumbers++;
		return reinterpret_cast<intptr_t>(m_Tail);
	}



	template<class Type, uint32 Size>
	inline uint32 LinkedList<Type, Size>::Insert(Type pData, node* pPrevNode)
	{
		node New = new Node();
		New->Data = std::move(pData);
		New->Next = (*pPrevNode)->Next;
		New->Prev = (*pPrevNode);
		(*pPrevNode)->Next = New;
		m_NodeNumbers++;
		return HYD_OK;
	}

	template<class Type, uint32 Size>
	inline uint32 LinkedList<Type, Size>::Insert(Type pData, Id pPrevNode)
	{
		node* pNode = reinterpret_cast<node*>(pPrevNode);

		node New = new Node();
		New->Data = std::move(pData);
		New->Next = (*pNode)->Next;
		New->Prev = (*pNode);
		(*pNode)->Next = New;
		m_NodeNumbers++;
		return HYD_OK;
	}

	template<class Type, uint32 Size>
	inline uint32 LinkedList<Type, Size>::Remove(Id* pNode)
	{
		node sNode = reinterpret_cast<node> (*pNode);
		if (sNode == nullptr)
			return 1;

		if (sNode == m_Head)
		{
			RemoveHead();
		}
		else if (sNode == m_Tail)
		{
			RemoveTail();
		}
		else
		{
			(sNode)->Prev->Next = (sNode)->Next;
			(sNode)->Next->Prev = (sNode)->Prev;
			delete sNode;
			m_NodeNumbers--;
		}
		*pNode = 0;
		return HYD_OK;
	}

	template<class Type, uint32 Size>
	inline uint32 LinkedList<Type, Size>::RemoveHead()
	{
		if (m_Tail == nullptr)
			return 1;
		node Next = m_Head->Next;
		delete m_Head;
		m_Head = Next;
		m_Head->Prev = nullptr;

		m_NodeNumbers--;
		return HYD_OK;
	}

	template<class Type, uint32 Size>
	inline uint32 LinkedList<Type, Size>::RemoveTail()
	{
		if (m_Tail == nullptr)
			return 1;

		node prev = m_Tail->Prev;
		delete m_Tail;
		m_Tail = prev;
		m_Tail->Next = nullptr;

		m_NodeNumbers--;
		return 0;
	}

	template<class Type, uint32 Size>
	inline uint32 LinkedList<Type, Size>::Clear()
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
