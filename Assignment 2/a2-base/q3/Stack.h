#pragma once

// Desc:  The usual fare.  Descriptions, invariants, pre and post are
//        omitted because they are duplicated elsewhere, namely Assignment 1.
template <class T>
class Stack {
public:
    Stack();
    ~Stack();
    void push(T x);
    T pop();
    const T& peek() const;
    bool isEmpty() const;

private:
    struct Node {
        T data;
        Node* next;
    };
    Node* head_;
};

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

template <class T>
Stack<T>::Stack()
    : head_(nullptr)
{ }

template <class T>
void Stack<T>::push(T x) {
    head_ = new Node { std::move(x), head_ };
}

template <class T>
T Stack<T>::pop() {
    T ret = std::move(head_->data);
    Node* const old_head = head_;
    head_ = head_->next;
    delete old_head;
    return ret;
}

template <class T>
const T& Stack<T>::peek() const {
    return head_->data;
}

template <class T>
bool Stack<T>::isEmpty() const {
    return (head_ == nullptr);
}

template <class T>
Stack<T>::~Stack() {
    Node* p = head_;
    while (p != nullptr) {
        head_ = head_->next;
        delete p;
        p = head_;
    }
}
