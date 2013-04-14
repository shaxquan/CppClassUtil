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
	
	~SLinkedList()
	{
		SListNode<Datatype> *itr = m_head;
		SListNode<Datatype> *next;

		while (itr != 0)
		{
			next = itr->m_next;
			delete itr;
			itr = next;
		}
	}

	void Append(Datatype p_data)
	{
		if (m_head == 0)
		{
			m_head = m_tail = new SListNode<Datatype>;
			m_head->m_data = p_data;
		}
		else
		{
			m_tail->InsertAfter(p_data);
			m_tail = m_tail->m_next;
		}
		m_count++;
	}

	//adds data to the beginning of the list
	void Prepend(Datatype p_data)
	{
		SListNode<Datatype> *newNode = new SListNode<Datatype>;
		newNode->m_data = p_data;
		newNode->m_next = m_head;

		m_head = newNode;
		if (m_tail == 0)
		{
			m_tail = m_head;
		}
		m_count++;
	}

	void Insert(SListIterator<Datatype> &p_iterator, Datatype p_data)
	{
		if (p_iterator.m_list != this)
		{
			return;
		}

		if (p_iterator.m_node != 0)
		{
			p_iterator.m_node->InsertAfter(p_data);

			if (p_iterator.m_node == m_tail)
			{
				m_tail = p_iterator.m_node->m_next;
			}
			m_count++;
		}
		else
		{
			Append(p_data);
		}
	}

	//removes the node that the iterator points to
	void Remove(SListIterator<Datatype> &p_iterator)
	{
		SListNode<Datatype> *node = m_head;

		if (p_iterator.m_list != this)
		{
			return;
		}

		//if node is invalid, do nothing
		if (p_iterator.m_node == 0)
		{
			return;
		}

		if (p_iterator.m_node == m_head)
		{
			p_iterator.Forth();
			RemoveHead();
		}
		else
		{
			while (node->m_next != p_iterator.m_node)
			{
				node = node->m_next;
			}

			//move the iterator forward.
			p_iterator.Forth();

			if (node->m_next == m_tail)
			{
				m_tail = node;
			}
			delete node->m_next;

			node->m_next = p_iterator.m_node;
		}
		m_count--;
	}

	    void RemoveHead()
    {
        SListNode<Datatype>* node = 0;

        if( m_head != 0 )
        {
            // make node point to the next node.
            node = m_head->m_next;

            // then delete the head, and make the pointer
            // point to node.
            delete m_head;
            m_head = node;

            // if the head is null, then we've just deleted the only node
            // in the list. set the tail to 0.
            if( m_head == 0 )
                m_tail = 0;

            m_count--;
        }
    }

    void RemoveTail()
    {
        SListNode<Datatype>* node = m_head;

        // if the list isn't empty, then remove a node.
        if( m_head != 0 )
        {
            // if the head is equal to the tail, then 
            // the list has 1 node, and we are removing it.
            if( m_head == m_tail )
            {
                // delete the node, and set both pointers
                // to 0.
                delete m_head;
                m_head = m_tail = 0;
            }
            else
            {
                // skip ahead until we find the node
                // right before the tail node
                while( node->m_next != m_tail )
                    node = node->m_next;

                // make the tail point to the node before the
                // current tail, and delete the old tail.
                m_tail = node;
                delete node->m_next;
                node->m_next = 0;
            }
            m_count--;
        }
    }


// ----------------------------------------------------------------
//  Name:           GetIterator
//  Description:    gets an iterator pointing to the beginning of
//                  the list
//  Arguments:      None
//  Return Value:   an iterator pointing to the beginning of the
//                  list.
// ----------------------------------------------------------------
    SListIterator<Datatype> GetIterator()
    {
        return SListIterator<Datatype>( this, m_head );
    }


// ----------------------------------------------------------------
//  Name:           Size
//  Description:    gets the size of the list
//  Arguments:      None
//  Return Value:   size of the list
// ----------------------------------------------------------------
    int Size()
    {
        return m_count;
    }


// ----------------------------------------------------------------
//  Name:           SaveToDisk
//  Description:    saves the list to disk
//  Arguments:      p_filename: name of the file to save to
//  Return Value:   true if successful
// ----------------------------------------------------------------
    bool SaveToDisk( char* p_filename )
    {
        FILE* outfile = 0;
        SListNode<Datatype>* itr = m_head;

        // open the file
        outfile = fopen( p_filename, "wb" );

        // return if it couldn't be opened
        if( outfile == 0 )
            return false;

        // write the size of the list first
        fwrite( &m_count, sizeof( int ), 1, outfile );

        // now loop through and write the list.
        while( itr != 0 )
        {
            fwrite( &(itr->m_data), sizeof( Datatype ), 1, outfile );
            itr = itr->m_next;
        }

        fclose( outfile );

        // return success.
        return true;
    }


// ----------------------------------------------------------------
//  Name:           ReadFromDisk
//  Description:    reads a list from disk
//  Arguments:      p_filename: name of the file
//  Return Value:   true if successful
// ----------------------------------------------------------------
    bool ReadFromDisk( char* p_filename )
    {
        FILE* infile = 0;
        Datatype buffer;
        int count = 0;

        // open the file
        infile = fopen( p_filename, "rb" );

        // return if it couldn't be opened
        if( infile == 0 )
            return false;

        // read the size of the list first
        fread( &count, sizeof( int ), 1, infile );

        // now loop through and read the list.
        while( count != 0 )
        {
            fread( &buffer, sizeof( Datatype ), 1, infile );
            Append( buffer );
            count--;
        }

        fclose( infile );

        // return success.
        return true;
    }
private:
	SListNode<Datatype > *m_head;
	SListNode<Datatype > *m_tail;
	int m_count;
};


template<class Datatype>
class SListIterator
{
public:


// ----------------------------------------------------------------
//  Name:           SListIterator
//  Description:    Constructor, creates an iterator with a given
//                  list and node.
//  Arguments:      p_list: the list the iterator points to
//                  p_node: the node the iterator points to
//  Return Value:   None
// ----------------------------------------------------------------
    SListIterator( SLinkedList<Datatype>* p_list = 0,
                   SListNode<Datatype>* p_node = 0 )
    {
        m_list = p_list;
        m_node = p_node;
    }


// ----------------------------------------------------------------
//  Name:           Start
//  Description:    moves the iterator to the start of the list.
//  Arguments:      None
//  Return Value:   None
// ----------------------------------------------------------------
    void Start()
    {
        if( m_list != 0 )
            m_node = m_list->m_head;
    }


// ----------------------------------------------------------------
//  Name:           Forth
//  Description:    Moves the iterator forward
//  Arguments:      None
//  Return Value:   None
// ----------------------------------------------------------------
    void Forth()
    {
        if( m_node != 0 )
            m_node = m_node->m_next;
    }


// ----------------------------------------------------------------
//  Name:           Item
//  Description:    gets the item the iterator points to
//  Arguments:      None
//  Return Value:   a reference to the item.
// ----------------------------------------------------------------
    Datatype& Item()
    {
        return m_node->m_data;
    }


// ----------------------------------------------------------------
//  Name:           Valid
//  Description:    determines if the iterator is valid or not
//  Arguments:      None
//  Return Value:   true if valid
// ----------------------------------------------------------------
    bool Valid()
    {
        return (m_node != 0);
    }

private:
// ----------------------------------------------------------------
//  Name:           m_node
//  Description:    the current node
// ----------------------------------------------------------------
    SListNode<Datatype>* m_node;

// ----------------------------------------------------------------
//  Name:           m_list
//  Description:    the current list
// ----------------------------------------------------------------
    SLinkedList<Datatype>* m_list;
};

#endif