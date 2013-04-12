#ifndef SLINKEDLIST_H
#define SLINKEDLIST_H

#include <stdio.h>

template <typename Datatype> class SListNode;
template <typename Datatype> class SLinkedList;
template <typename Datatype> class SListIterator;

template <typename Datatype>
class SListNode
{
public:

	//data
	Datatype m_data;

	//next pointer
	SListNode<Datatype> *m_next;

	SListNode()
	{
		m_next = 0;
	}

	//Insert a new node after the current node
	void InsertAfter(Datatype p_data)
	{
		SListNode<Datatype> newNode = new SLinkedList<Datatype>;
		newNode->m_data = p_data;

		//make the new node point to the next node
		newNode->m_next = m_next;

		//make the previous node point to the new node
		m_next = newNode;
	}
};

template <typename Datatype>
class SLinkedList
{
public:
	SLinkedList()
	{
		m_head = 0;
		m_tail = 0;
		m_count = 0;
	}
	
	~SLinkedList();

	/* data */
};

#endif