#include "Queue.h"
#include <stdexcept>

// NOTE: All exceptions thrown are derived from std::logic_error.

constexpr size_t INITIAL_SIZE = 6;

Queue::Queue()
    : size_ { 0 }
    , capacity_ { INITIAL_SIZE }
    , frontIndex_ { 0 }
    , backIndex_ { 0 }
    , arr_ { new int[INITIAL_SIZE] }
{ }

Queue::~Queue() { delete[] arr_; }

Queue::Queue(const Queue& other)
    : size_ { other.size_ }
    , capacity_ { other.capacity_ }
    , frontIndex_ { other.frontIndex_ }
    , backIndex_ { other.backIndex_ }
    , arr_ { new int[other.capacity_] }
{
    for (size_t i = 0; i < size_; ++i) {
        const size_t realIndex = (frontIndex_ + i) % capacity_;
        arr_[realIndex] = other.arr_[realIndex];
    }
}

Queue& Queue::operator=(const Queue& other) {
    if (this != &other) {
        // Resize if needed.
        if (capacity_ < other.size_) {
            delete[] arr_;
            arr_ = new int[other.size_];
            capacity_ = other.size_;
        }
        // Flatten circular array and copy over.
        for (size_t i = 0; i < other.size_; ++i) {
            arr_[i] = other.arr_[(other.frontIndex_ + i) % other.capacity_];
        }
        size_ = other.size_;
        frontIndex_ = 0;
        backIndex_ = size_;
    }
    return *this;
}

void Queue::resize(size_t newCapacity) {
    if (newCapacity < size_) {
        throw std::invalid_argument("New capacity (" + std::to_string(newCapacity)
            + ") can't be smaller than current size (" + std::to_string(size_) + ").");
    }
    int* const newArr = new int[newCapacity];
    for (size_t i = 0; i < size_; ++i) {
        newArr[i] = arr_[(frontIndex_ + i) % capacity_];
    }
    delete[] arr_;
    arr_ = newArr;
    capacity_ = newCapacity;
    frontIndex_ = 0;
    backIndex_ = size_;
}

void Queue::enqueue(int x) {
    if (size_ >= capacity_) {
        this->resize(2 * capacity_);
    }
    arr_[backIndex_] = x;
    backIndex_ = (backIndex_ + 1) % capacity_;
    size_++;
}

void Queue::dequeue() {
    if (this->isEmpty()) {
        throw std::out_of_range("Cannot dequeue an empty queue.");
    }
    size_--;
    frontIndex_ = (frontIndex_ + 1) % capacity_;
    if (size_ < capacity_ / 4 && size_ >= INITIAL_SIZE) {
        this->resize(capacity_ / 2);
    }
}

int Queue::peek() const {
    if (this->isEmpty()) {
        throw std::out_of_range("Cannot peek an empty queue.");
    }
    return arr_[frontIndex_];
}

bool Queue::isEmpty() const {
    return size_ == 0;
}
