#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

template <typename Datatype> class DListNode;
template <typename Datatype> class DLinkedList;
template <typename Datatype> class DListIterator;

template <typename Datatype>
class DListNode
{
public:
  Datatype m_data;
  DListNode<Datatype> *m_next;
  DListNode<Datatype> *m_previous;

  void Delink()
  {
    if (m_previous != 0)
      m_previous->m_next = m_next;

    if (m_next != 0)
      m_next->m_previous = m_previous;
  }

  void InsertAfter(Datatype p_data)
  {
    DListNode<Datatype> *newNode = new DListNode<Datatype>;
    newNode->m_data= p_data;

    newNode->m_next = m_next;
    newNode->m_previous = this;

    if (m_next != 0)
    {
      m_next->m_previous = newNode;
    }

    m_next = newNode;
  }

  void InsertBefore(Datatype p_data)
  {
    DListNode<Datatype> *newNode = new DListNode<Datatype>;
    newNode->m_data = p_data;

    newNode->m_next = this;
    newNode->m_previous = m_previous;

    if (m_previous != 0)
    {
      m_previous->m_next = newNode;
    }

    m_previous = newNode;
  }
};

template <typename Datatype>
class DLinkedList
{
public:
  DLinkedList()
  {
    m_head = 0;
    m_tail = 0;
    m_count = 0;
  }

  ~DLinkedList() 
  {
    DListNode<Datatype> *node = m_head;
    DListNode<Datatype> *next;

    while (node != 0)
      {
        next = node->m_next;
        delete node;

        node = next;
      }
  }

  void Append(Datatype p_data)
  {
    if (m_head == 0)
      {
        m_head = m_tail = new DListNode<Datatype>;
        m_head->m_data = p_data;
        m_head->next = 0;
        m_head->m_previous = 0;
      }
    else
      {
        m_tail->InsertAfter(p_data);
        m_tail = m_tail->m_next;
      }
    m_count++;
  }

  //Addes a new node to the beginning of a list
  void Prepend(Datatype p_data)
  {
    if (m_head == 0)
      {
        m_head = m_tail = new DListNode<Datatype>;
        m_head->m_data = p_data;
        m_head->m_next = 0;
        m_head->m_previous = 0;
      }
    else
      {
        //insert a new node before the head, and reset the head.
        m_head->InsertBefore(p_data);
        m_head =  m_head->m_previous;
      }
    m_count++;
  }

  void RemoveHead()
  {
    DListNode<Datatype> *node = 0;
    if (m_head != 0)
      {
        node = m_head->m_next;

        delete m_head;
        m_head = node;

        //if the head is null, then we've just deleted the only node
        //in the list. set the tail to 0. if not, set the previous
        //pointer to -
        if (m_head == 0)
          {
            m_tail = 0;
          }
        else
          {
            m_head->m_previous = 0;
          }
        m_count--;
      }
  }

  void RemoveTail()
  {
    DListNode<Datatype> *node = 0;
    if (m_tail != 0)
      {
        node = m_tail->m_previous;

        delete m_tail;
        m_tail = node;

        //if the tail is null, then we've just deleted the only node
        //in the list.set the head to 0. if not , set the next pointer
        //to 0.
        if (m_tail == 0)
          {
            m_head = 0;
          }
        else
          {
            m_tail->m_next = 0;
          }
        m_count--;
      }
  }

  void InsertAfter(DListIterator<Datatype> &p_iterator, Datatype p_data)
  {
    if (p_iterator.m_node != 0)
      {
        p_iterator.m_node->InsertAfter(p_data);

        if (p_iterator.m_node == m_tail)
          {
            m_tail = m_tail->m_next;
          }

        m_count++;
      }
    else
      {
        Append(p_data);
      }
  }

  void InsertBefore( DListIterator<Datatype>& p_iterator, Datatype p_data )
    {
        if( p_iterator.m_node != 0 )
        {
            // insert the data before the iterator
            p_iterator.m_node->InsertBefore( p_data );

            // if the iterator was the head of the list,
            // reset the head pointer.
            if( p_iterator.m_node == m_head )
                m_head = m_head->m_previous;

            // increment the count
            m_count++;
        }
        else
        {
            Prepend( p_data );
        }
    }


    void Remove( DListIterator<Datatype>& p_iterator )
    {
        // temporary node pointer.
        DListNode<Datatype>* node;

        // if node is invalid, do nothing.
        if( p_iterator.m_node == 0 )
            return;


        // save the pointer to the node we want to delete.
        node = p_iterator.m_node;

        // if the node we want to remove is the head or the tail
        // nodes, then move the head or tail to the next or
        // previous node.
        if( node == m_head )
        {
            m_head = m_head->m_next;
        }
        else if( node == m_tail )
        {
            m_tail = m_tail->m_previous;
        }

        // move the iterator forward to the next valid node
        p_iterator.Forth();

        // delink and delete the node.
        node->Delink();
        delete node;

        // if the head is 0, then set the tail to 0 as well.
        if( m_head == 0 )
            m_tail = 0;
        
        m_count--;
    }

    DListIterator<Datatype> GetIterator()
    {
        return DListIterator<Datatype>( this, m_head );
    }

    int Size()
    {
        return m_count;
    }

    bool SaveToDisk( char* p_filename )
    {
        FILE* outfile = 0;
        DListNode<Datatype>* itr = m_head;

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
  DListNode<Datatype> *m_head;
  DListNode<Datatype> *m_tail;
  int m_count;
};


template<class Datatype>
class DListIterator
{
public:


// ----------------------------------------------------------------
//  Name:           DListIterator
//  Description:    Constructor, creates an iterator that points
//                  to the given list and node. 
//  Arguments:      p_list: pointer to the list the iterator belongs
//                          to.
//                  p_node: pointer to the current node.
//  Return Value:   None.
// ----------------------------------------------------------------
    DListIterator( DLinkedList<Datatype>* p_list = 0,
                   DListNode<Datatype>* p_node = 0 )
    {
        m_list = p_list;
        m_node = p_node;
    }


// ----------------------------------------------------------------
//  Name:           Start
//  Description:    Resets the iterator to the beginning of the 
//                  list
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
    void Start()
    {
        if( m_list != 0 )
            m_node = m_list->m_head;
    }

// ----------------------------------------------------------------
//  Name:           End
//  Description:    Resets the iterator to the end of the list
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
    void End()
    {
        if( m_list != 0 )
            m_node = m_list->m_tail;
    }


// ----------------------------------------------------------------
//  Name:           Forth
//  Description:    Moves the iterator forward by one position
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
    void Forth()
    {
        if( m_node != 0 )
            m_node = m_node->m_next;
    }


// ----------------------------------------------------------------
//  Name:           Back
//  Description:    Moves the iterator backward by one position.
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
    void Back()
    {
        if( m_node != 0 )
            m_node = m_node->m_previous;
    }


// ----------------------------------------------------------------
//  Name:           Item
//  Description:    Gets the item that the iterator is pointing to.
//  Arguments:      None.
//  Return Value:   Reference to the data in the node.
// ----------------------------------------------------------------
    Datatype& Item()
    {
        return m_node->m_data;
    }


// ----------------------------------------------------------------
//  Name:           Valid
//  Description:    Determines if the node is valid.
//  Arguments:      None.
//  Return Value:   true if valid
// ----------------------------------------------------------------
    bool Valid()
    {
        return (m_node != 0);
    }


// ----------------------------------------------------------------
//  Name:           operator==
//  Description:    Determines if two iterators point to the same
//                  node.
//  Arguments:      None.
//  Return Value:   true if they point to the same node.
// ----------------------------------------------------------------
    bool operator==( DListIterator<Datatype>& p_rhs )
    {
        if( m_node == p_rhs.m_node && m_list == p_rhs.m_list )
        {
            return true;
        }
        return false;
    }



// ----------------------------------------------------------------
//  Name:           m_node
//  Description:    pointer to the current node
// ----------------------------------------------------------------
    DListNode<Datatype>* m_node;


// ----------------------------------------------------------------
//  Name:           m_list
//  Description:    pointer to the current list.
// ----------------------------------------------------------------
    DLinkedList<Datatype>* m_list;
};

#endif
