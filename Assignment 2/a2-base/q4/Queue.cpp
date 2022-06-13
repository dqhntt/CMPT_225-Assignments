#include "Queue.h"
#include <stdexcept>

constexpr size_t INITIAL_SIZE = 6;

// Desc:  Constructor
Queue::Queue()
    : size_ { 0 }
    , capacity_ { INITIAL_SIZE }
    , frontIndex_ { 0 }
    , backIndex_ { 0 }
    , arr_ { new int[INITIAL_SIZE] }
{ }

// Desc:  Destructor
Queue::~Queue() { delete[] arr_; }

// Desc:  Copy Constructor
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

void Queue::swap(Queue& other) {
    std::swap(arr_, other.arr_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(frontIndex_, other.frontIndex_);
    std::swap(backIndex_, other.backIndex_);
}

// Desc:  Assignment operator
Queue& Queue::operator=(const Queue& other) {
    if (this != &other) {
        if (size_ < other.size_) {
            Queue(other).swap(*this);
        } else {
            // Traverse and copy.
            for (size_t i = 0; i < size_; ++i) {
                arr_[i] = other.arr_[(other.frontIndex_ + i) % other.capacity_];
            }
            size_ = other.size_;
            frontIndex_ = 0;
            backIndex_ = size_;
        }
    }
    return *this;
}

// Pre: newCapacity >= size_
void Queue::resize(size_t newCapacity) {
    if (newCapacity < size_) {
        throw std::logic_error("New capacity (" + std::to_string(newCapacity)
            + ") can't be smaller than size (" + std::to_string(size_) + ").");
    }
    if (newCapacity == 0) {
        delete[] arr_;
        arr_ = nullptr;
        return;
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

// Desc:  Inserts element x at the back (O(1))
void Queue::enqueue(int x) {
    if (size_ >= capacity_) {
        if (sizeof(capacity_) > 4) { // > 32 bits.
            this->resize(2 * capacity_);
        } else {
            size_t newCapacity = INITIAL_SIZE;
            // Check for unsigned overflow.
            for (size_t n = capacity_; newCapacity <= capacity_; --n) {
                newCapacity = capacity_ + n;
                if (n <= 0) {
                    throw std::overflow_error("Fatal error when resizing queue.");
                }
            }
            this->resize(newCapacity);
        }
    }
    arr_[backIndex_] = x;
    backIndex_ = (backIndex_ + 1) % capacity_;
    size_++;
}

// Desc:  Removes the frontmost element (O(1))
//  Pre:  Queue not empty
void Queue::dequeue() {
    if (this->isEmpty()) {
        throw std::logic_error("Cannot dequeue an empty queue.");
    }
    size_--;
    frontIndex_ = (frontIndex_ + 1) % capacity_;
    if (size_ < capacity_ / 4 && size_ >= INITIAL_SIZE) {
        this->resize(capacity_ / 2);
    }
}

// Desc:  Returns a copy of the frontmost element (O(1))
//  Pre:  Queue not empty
int Queue::peek() const {
    if (this->isEmpty()) {
        throw std::logic_error("Cannot peek an empty queue.");
    }
    return arr_[frontIndex_];
}

// Desc:  Returns true if and only if queue empty (O(1))
bool Queue::isEmpty() const {
    return size_ == 0;
}
