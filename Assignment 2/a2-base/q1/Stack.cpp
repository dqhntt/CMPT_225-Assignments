#include "Stack.h"
#include <stdexcept>

// NOTE: All exceptions thrown are derived from std::logic_error.

Stack::Stack()
    : size_(0)
{ }

void Stack::push(int x) {
    if (size_ >= STACKCAP_) {
        throw std::length_error("Stack full");
    }
    // Shift all elements up (right) by one.
    std::move_backward(arr_, arr_ + size_, arr_ + size_ + 1);
    arr_[0] = x;
    size_++;
}

int Stack::pop() {
    if (size_ <= 0) {
        throw std::out_of_range("Stack empty");
    }
    const int top = arr_[0];
    // Shift all but the first element down (left) by one.
    std::move(arr_ + 1, arr_ + size_, arr_);
    size_--;
    return top;
}

int Stack::peek() const {
    if (size_ <= 0) {
        throw std::out_of_range("Stack empty");
    }
    return arr_[0];
}

bool Stack::isEmpty() const {
    return size_ == 0;
}
