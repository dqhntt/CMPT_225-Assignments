/**
 * @file PlayList.h
 * @author Hieu Duong
 * @date 2022-06-03
 */
// Write your name and date here
#include "PlayList.h"
#include <stdexcept>
using Node = PlayListNode;

namespace {
/**
 * @brief Insert `song` after `node` in the free store.
 *        Reconnect nodes automatically.
 */
inline void append(Node* node, const Song& song) {
    node->next = new Node(song, node->next);
}
/**
 * @see https://youtu.be/xnqTKD8uD64?t=4006
 */
inline void append(Node* node, Song&& song) {
    node->next = new Node(std::move(song), node->next);
}

/**
 * @brief Insert `with` (one node, not the whole chain) after `to`.
 *        Reconnect nodes automatically.
 */
inline void append(Node* to, Node* with) {
    if (with != nullptr) {
        with->next = to->next;
    }
    to->next = with;
}

/**
 * @brief Deallocate all linked nodes from `first` to but not including `last`.
 */
inline void destroy(Node* first, const Node* last) {
    Node* next = nullptr;
    while (first != last) {
        next = first->next;
        delete first;
        first = next;
    }
}

/**
 * @brief Move `node` up `n` elements.
 */
inline void advance(Node*& node, unsigned n) {
    while (n-- > 0) {
        node = node->next;
    }
}

/**
 * @brief Return `from` moved up `n` elements.
 *        e.g. next(head_, pos) returns the node at index `pos`.
 *             next(tail_) returns nullptr.
 * @pre 0 <= n <= (size - (position of `from` relative to `head_`))
 */
inline Node* next(Node* from, unsigned n = 1) {
    advance(from, n);
    return from;
}

/**
 * @brief Returns owning pointer to node after `node`.
 *        Reconnect nodes automatically.
 * @warning Caller is responsible for freeing the returned pointer.
 */
inline Node* extractNodeAfter(Node* node) {
    Node* const target = node->next;
    if (target != nullptr) {
        node->next = target->next;
    }
    return target;
}
} // namespace

// PlayList method implementations go here

PlayList::PlayList()
    : size_(0)
    , head_(nullptr)
    , tail_(nullptr)
{ }

PlayList::PlayList(const PlayList& other)
    : size_(other.size_)
    , head_(nullptr)
    , tail_(nullptr)
{
    if (other.size_ == 0) {
        return;
    }
    tail_ = head_ = new Node(other.head_->song);
    Node* curr = next(other.head_);
    while (curr != nullptr) {
        try {
            append(tail_, curr->song);
            advance(tail_, 1);
        } catch (...) {
            destroy(head_, nullptr);
            throw;
        }
        advance(curr, 1);
    }
}

PlayList::~PlayList() {
    destroy(head_, nullptr);
}

void PlayList::swap(PlayList& other) {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
}

/**
 * @note Copy-and-swap idiom.
 * 
 * @see http://gotw.ca/gotw/059.htm
 *      https://youtu.be/vLinb2fgkHk?t=2245
 */
PlayList& PlayList::operator=(PlayList other) {
    this->swap(other);
    return *this;
}

void PlayList::insert(Song song, unsigned pos) {
    if (pos > size_) {
        throw std::out_of_range("insert(song, " + std::to_string(pos) + ") out of bounds");
    }
    if (pos == 0) {
        head_ = new Node(std::move(song), head_);
        if (size_ == 0) {
            tail_ = head_;
        }
    } else if (pos == size_) {
        append(tail_, std::move(song));
        advance(tail_, 1);
    } else {
        append(next(head_, pos - 1), std::move(song));
    }
    size_++;
}

Song PlayList::remove(unsigned pos) {
    if (pos >= size_) {
        throw std::out_of_range("remove(" + std::to_string(pos) + ") out of bounds");
    }
    Song song;
    if (pos == 0) {
        Node* const old = head_;
        song = std::move(old->song);
        advance(head_, 1);
        delete old;
        if (size_ == 1) {
            tail_ = head_; // = nullptr
        }
    } else {
        Node* const prev = next(head_, pos - 1);
        song = std::move(prev->next->song);
        delete extractNodeAfter(prev);
        if (pos == size_ - 1) {
            tail_ = prev;
        }
    }
    size_--;
    return song;
}

void PlayList::swap(unsigned pos1, unsigned pos2) {
    if (pos1 >= size_ || pos2 >= size_) {
        throw std::out_of_range(
            "swap(" + std::to_string(pos1) + ", " + std::to_string(pos2) + ") out of bounds");
    }
    if (pos1 == pos2) {
        return;
    }
    const unsigned first = std::min(pos1, pos2);
    const unsigned second = std::max(pos1, pos2);
    Node* node1;
    Node* const prev2 = next(head_, second - 1);
    Node* const node2 = extractNodeAfter(prev2);
    if (first > 0) {
        Node* const prev1 = next(head_, first - 1);
        node1 = extractNodeAfter(prev1);
        append(prev1, node2);
    } else {
        node1 = head_;
        node2->next = node1->next;
        head_ = node2;
    }
    append(prev2 == node1 ? node2 : prev2, node1);
    if (node2 == tail_) {
        tail_ = node1;
    }
}

Song PlayList::get(unsigned pos) const {
    if (pos >= size_) {
        throw std::out_of_range("get(" + std::to_string(pos) + ") out of bounds");
    }
    return pos == size_ - 1 ? tail_->song : next(head_, pos)->song;
}

unsigned PlayList::size() const {
    return size_;
}
