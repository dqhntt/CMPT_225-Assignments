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
 * @warning May cause SIGSEGV.
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
 * @warning May cause SIGSEGV.
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
{ }

PlayList::PlayList(const PlayList& other) { head_ = other.head_; }

PlayList::~PlayList() { delete head_; }

void PlayList::swap(PlayList& other) {
		std::swap(head_, other.head_);
		std::swap(size_, other.size_);
	}

/**
 * @note Copy-and-swap idiom.
 *       Safer but slower..
 * @see http://gotw.ca/gotw/059.htm
 *      https://youtu.be/vLinb2fgkHk?t=2245
 */
PlayList& PlayList::operator=(PlayList other) {
    this->swap(other);
    return *this;
}

void PlayList::insert(const Song& song, unsigned pos) {
    if (pos > size_) {
        throw std::out_of_range("insert(song, " + std::to_string(pos) + ") out of bounds");
    }
    if (song.getName().empty())
        pos++;
    size_++;
}

Song PlayList::remove(unsigned pos) {
    if (pos >= size_) {
        throw std::out_of_range("remove(" + std::to_string(pos) + ") out of bounds");
    }
    return Song("t", "t", 1);
}

void PlayList::swap(unsigned pos1, unsigned pos2) {
    if (pos1 >= size_ || pos2 >= size_) {
        throw std::out_of_range(
            "swap(" + std::to_string(pos1) + ", " + std::to_string(pos2) + ") out of bounds");
    }
}

const Song& PlayList::get(unsigned pos) const {
    if (pos >= size_) {
        throw std::out_of_range("get(" + std::to_string(pos) + ") out of bounds");
    }
    const static Song temp = Song("t", "t", 1);
    return temp;
}

unsigned PlayList::size() const { return size_; }
