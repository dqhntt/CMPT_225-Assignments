#pragma once
#include <algorithm>
// #define NDEBUG
#include <cassert>
#include <string>

// * * * replace with your own personal modulus * * * //
constexpr int M = 3061;
constexpr int SCALE = 225;

// Desc:  Dynamic Set of strings using a Hash Table (generic version)
//  Pre:  class T must have an attribute key of type string
template <class T>
class Set {
public:
    Set();
    Set(const Set&) = delete;
    Set& operator=(const Set&) = delete;

    int insert(T* x);
    T* search(const std::string& key) const;

private:
    T* arr_[M];

    // * * * Add attributes and helper methods here if you wish. * * * //

    unsigned size_; // * * * remove this * * * //
};

// Helper functions for Set<T>.
namespace impl {

// According to provided table.
int map_char(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    if (c >= 'a' && c <= 'z') {
        return c - 'a' + 10;
    }
    if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 36;
    }
    assert(c == '_' && "Unrecognized char mapping");
    return 62; // _
}

// Self-notes:
// https://math.stackexchange.com/a/2416125
// (n * m) % p = ((n % p) * (m % p)) % p
// = ((225 % M) * (base64 % M)) % M
// = (225 * (Horner's hash)) % M
// Because M > 225.
int hash(const std::string& str, int base = 64, int max_val = M) {
    assert(!str.empty());
    int hashed_val = 0;
    for (char c : str) {
        // Horner's method.
        hashed_val = (hashed_val * base + map_char(c)) % max_val;
    }
    return ((SCALE % max_val) * hashed_val) % max_val;
}

} // namespace impl

// Desc:  Default constructor.  Initialize table to nullptr.
template <class T>
Set<T>::Set() : size_(0) {
    std::fill(std::begin(arr_), std::end(arr_), nullptr);
}

// Desc:  Insert x into the Set, according to its ->key.
//        If ->key is already present, then replace it by x.
//        Collisions are resolved via quadratic probing sequence.
// Post:  returns the table index where x was inserted
//        returns -1 if x was not inserted
template <class T>
int Set<T>::insert(T* x) {
    if (size_ == M)
        return -1;
    for (int i = size_ - 1; i >= 0; i--) {
        if (arr_[i]->key == x->key) {
            arr_[i] = x;
            return i;
        }
    }
    arr_[size_] = x;
    size_++;
    return size_ - 1;
}

// Desc:  Returns T* x such that x->key == key, if and only if
//        key is in the Set.
//        Returns nullptr if and only if key is not in the Set.
// Post:  Set is unchanged
template <class T>
T* Set<T>::search(const std::string& key) const {
    for (int i = size_ - 1; i >= 0; i--) {
        if (arr_[i]->key == key)
            return arr_[i];
    }
    return nullptr;
}
