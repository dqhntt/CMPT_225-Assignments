#pragma once
#include <cstddef>

// Desc:  Implementation of an int sequence with enqueue/dequeue in FIFO order
class Queue {
public:
    // Desc:  Constructor
    Queue();

    // Desc:  Destructor
    ~Queue();

    // Desc:  Copy Constructor
    Queue(const Queue& other);

    // Desc:  Assignment operator
    Queue& operator=(const Queue& other);

    // Desc:  Inserts element x at the back (O(1))
    void enqueue(int x);

    // Desc:  Removes the frontmost element (O(1))
    //  Pre:  Queue not empty
    void dequeue();

    // Desc:  Returns a copy of the frontmost element (O(1))
    //  Pre:  Queue not empty
    int peek() const;

    // Desc:  Returns true if and only if queue empty (O(1))
    bool isEmpty() const;

private:
    size_t size_;       // number of elements in the queue
    size_t capacity_;   // number of slots in the array
    size_t frontIndex_; // index the topmost element
    size_t backIndex_;  // index where the next element will be placed
    int* arr_;

    void resize(size_t newCapacity);
    void swap(Queue& other);
};
