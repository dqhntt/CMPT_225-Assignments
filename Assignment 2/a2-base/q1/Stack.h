#pragma once

// Desc:  Implementation of an int sequence with push/pop in a LIFO order
class Stack {
public:
    // Desc:  Object constructor
    // Post:  An empty stack is created.
    Stack();

    // Desc:  Insert element x to the top of the stack.
    //  Pre:  Stack is not full.
    // Post:  x is inserted at the top of the stack.
    // Algo:  Right shift all elements by 1 and put x at the front.
    void push(int x);

    // Desc:  Remove and return element at the top of the stack.
    //  Pre:  Stack is not empty.
    // Post:  Element at the top of the stack is removed and returned.
    // Algo:  Left shift all but the front elements by 1.
    int pop();

    // Desc:  Return the topmost element of the stack.
    //  Pre:  Stack is not empty.
    // Post:  Element at the top of the stack is returned.
    int peek() const;

    // Desc:  Check whether the stack is currently empty.
    // Post:  Return true if the stack is empty, false otherwise.
    // Algo:  Check if internal size is 0.
    bool isEmpty() const;

private:
    // Desc:  arr_ = static array of capacity STACKCAP_
    //        size_ = the number of elements presently in the stack
    //  Inv:  Follows the A2 Spec, namely that the stack elements
    //        are in order within A[0..size_-1], where the top of
    //        the stack is A[0].
    static const unsigned STACKCAP_ = 8;
    int arr_[STACKCAP_];
    unsigned size_;
};
