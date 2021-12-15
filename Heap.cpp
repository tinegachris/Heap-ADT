

#ifndef HEAP_CPP
#define HEAP_CPP

#include "Heap.h"

using namespace std;

#define PARENT(child) (child - 1) / 2   //! Gets the index of the parent

#define LEFT(parent) 2 * parent + 1     //! Gets the index of the left child

#define RIGHT(parent) 2 * parent + 2    //! Gets the index of the right child

//--------------------------------------------------------------------

// Constructor

template < typename DataType, typename KeyType, typename Comparator >
Heap<DataType,KeyType,Comparator>:: Heap ( int maxNumber )

// Creates an empty heap. Allocates enough memory for maxNumber
// data items.

{
    maxSize = maxNumber;
    size = 0;
    dataItems = new DataType[maxSize];
}

// Copy Constructor

template < typename DataType, typename KeyType, typename Comparator >
Heap<DataType,KeyType,Comparator>:: Heap ( const Heap& other )

    : maxSize{ other->maxSize }
    , size{ other->size }
    , dataItems{ new DataType[other->maxSize] }

// Copy constructor
{
    std::copy(other->dataItems, other->dataItems + other->maxSize, dataItems);
}

//--------------------------------------------------------------------

// Assignment Operator

template < typename DataType, typename KeyType, typename Comparator >
Heap<DataType,KeyType,Comparator>& Heap<DataType,KeyType,Comparator>:: operator= ( const Heap& other )

{
    if (this == &other) return *this;   // Self assignment check.

    // Copy data.
    maxSize = other->maxSize;
    size = other->size;

    delete[] dataItems;
    dataItems = new DataType[maxSize];
    std::copy(other->dataItems, other->dataItems + other->maxSize, dataItems);

    return *this;
}

//--------------------------------------------------------------------

// The Destructor

template < typename DataType, typename KeyType, typename Comparator >
Heap<DataType,KeyType,Comparator>:: ~Heap ()

// Frees the memory used by a heap.

{
    clear();
    delete[] dataItems;
}

//--------------------------------------------------------------------

// Inserts newDataItem

template < typename DataType, typename KeyType, typename Comparator >
void Heap<DataType,KeyType,Comparator>:: insert ( const DataType &newDataItem ) throw ( logic_error )

// Inserts newDataItem into a heap. This data item is initially
// inserted as the bottom rightmost data item in the heap. It is then
// moved upward until a valid heap is produced.

{
    if (isFull())
    {
        throw std::logic_error("The heap is full.");
    }

    int hole = size++;

    // While the hole is not the top item 
    // and newDataItem has a priorty greater than or equal than the hole's parent.
    while (hole > 0 && comparator(dataItems[PARENT(hole)].getPriority(), newDataItem.getPriority()))
    {
        // Move the parent into the hole.
        dataItems[hole] = std::move(dataItems[PARENT(hole)]);

        // Set the parent to be the new hole.
        hole = PARENT(hole);
    }

    // Move the newDataItem into the final hole.
    dataItems[hole] = newDataItem;
}

//--------------------------------------------------------------------

// Removes Highest Priority dataItem

template < typename DataType, typename KeyType, typename Comparator >
DataType Heap<DataType,KeyType,Comparator>:: remove () throw ( logic_error )

// Removes the data item with the highest priority (the root) from a
// heap and returns it. This data item is replaced with the bottom
// rightmost data item, which is moved downward until a valid heap is
// produced.

{
    if (isEmpty())
    {
        throw std::logic_error("Empty list.");
    }

    // Move the highest priority data item into our return variable.
    DataType ret = std::move(dataItems[0]);

    // Move the bottom right most data item into a temp variable.
    DataType temp = std::move(dataItems[--size]);

    // Heapify
    int hole = 0;

    // While hole has children
    while (LEFT(hole) < size)
    {
        // Set child to left child
        int child = LEFT(hole);

        // Does hole have a right child?
        if (child + 1 < size)
        {
            // Does right child have a greater priority than left child?
            if (comparator(dataItems[child].getPriority(), dataItems[child + 1].getPriority()))
            {
                // Set child to right child.
                child++;
            }
        }

        // Does the child have a greater than or equal priority than the temp DataType?
        if (comparator(temp.getPriority(), dataItems[child].getPriority()))
        {
            // Move the child DataType into the hole.
            dataItems[hole] = std::move(dataItems[child]);

            // Set the hole to be the child index.
            hole = child;
        }
        else
        {
            break;
        }
    }

    dataItems[hole] = std::move(temp);

    return ret;
}

//--------------------------------------------------------------------

// Clear all data items

template < typename DataType, typename KeyType, typename Comparator >
void Heap<DataType,KeyType,Comparator>:: clear ()

// Removes all the data items from a heap.

{
    delete[] dataItems;
    dataItems = new DataType[maxSize];
    size = 0;
}

//--------------------------------------------------------------------

// True if isEmpty

template < typename DataType, typename KeyType, typename Comparator >
bool Heap<DataType,KeyType,Comparator>:: isEmpty () const

// Returns true if a heap is empty. Otherwise, returns false.

{
    return size == 0;
}

//--------------------------------------------------------------------

// True if isFull

template < typename DataType, typename KeyType, typename Comparator >
bool Heap<DataType,KeyType,Comparator>:: isFull () const

// Returns true if a heap is full. Otherwise, returns false.

{
    return size == maxSize;
}

//--------------------------------------------------------------------

template < typename DataType, typename KeyType, typename Comparator >
void Heap<DataType,KeyType,Comparator>:: showStructure () const

// Outputs the priorities of the data items in a heap in both array
// and tree form. If the heap is empty, outputs "Empty heap". This
// operation is intended for testing/debugging purposes only.

{
    int j;   // Loop counter

    cout << endl;
    if ( size == 0 )
       cout << "Empty heap" << endl;
    else
    {
       cout << "size = " << size << endl;       // Output array form
       for ( j = 0 ; j < maxSize ; j++ )
           cout << j << "\t";
       cout << endl;
       for ( j = 0 ; j < size ; j++ )
           cout << dataItems[j].getPriority() << "\t";
       cout << endl << endl;
       showSubtree(0,0);                        // Output tree form
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template < typename DataType, typename KeyType, typename Comparator >
void Heap<DataType,KeyType,Comparator>:: showSubtree ( int index, int level ) const

// Helper function for the showStructure() function. Outputs the
// subtree (subheap) whose root is stored in dataItems[index]. Argument
// level is the level of this dataItems within the tree.

{
     int j;   // Loop counter

     if ( index < size )
     {
        showSubtree(2*index+2,level+1);        // Output right subtree

        for ( j = 0 ; j < level ; j++ )        // Tab over to level
            cout << "\t";

        cout << " " << dataItems[index].getPriority();   // Output dataItems's priority

        if ( 2*index+2 < size )                // Output "connector"
           cout << "<";
        else if ( 2*index+1 < size )
           cout << "\\";
        cout << endl;

        showSubtree(2*index+1,level+1);        // Output left subtree
    }
}

//--------------------------------------------------------------------
//
//                       Take-home operations
//
//--------------------------------------------------------------------

template < typename DataType, typename KeyType, typename Comparator >
void Heap<DataType,KeyType,Comparator>:: writeLevels () const

// Outputs the data items in a heap in level order, one level per line.
// Only outputs the priority for each data item.

{
    int levelCount = 1;
    int printCount = 0;
    for (int i = 0; i < size; i++, printCount++)
    {
        if (printCount == levelCount)
        {
            cout << endl;
            printCount = 0;
            levelCount *= 2;
        }
        cout << dataItems[i].getPriority() << " ";
    }
    cout << endl;
}


#endif	// #ifndef HEAP_CPP
