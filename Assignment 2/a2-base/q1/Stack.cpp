#include "Stack.h"
#include <algorithm>
#include <stdexcept>

Stack::Stack()
    : size_(0)
{ }

void Stack::push(int x) {
    if (size_ >= STACKCAP_) {
        throw std::logic_error("Stack full");
    }
    std::move_backward(arr_, arr_ + size_, arr_ + size_ + 1);
    arr_[0] = x;
    size_++;
}

int Stack::pop() {
    if (size_ <= 0) {
        throw std::logic_error("Stack empty");
    }
    const int top = arr_[0];
    std::move(arr_ + 1, arr_ + size_, arr_);
    size_--;
    return top;
}

int Stack::peek() const {
    if (size_ <= 0) {
        throw std::logic_error("Stack empty");
    }
    return arr_[0];
}

bool Stack::isEmpty() const {
    return size_ == 0;
}
