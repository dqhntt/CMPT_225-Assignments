#pragma once
// #define NDEBUG
#include <cassert>
#include <string>
#include <type_traits>

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
    return 62;
}

// Self-notes:
// https://math.stackexchange.com/a/2416125
// (n * m) % p = ((n % p) * (m % p)) % p
// = ((225 % M) * (base64 % M)) % M
// = (225 * (Horner's hash)) % M
// Because M > 225.
int hash(const std::string& str, int base = 64, int max_val = M) {
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
Set<T>::Set() {
    static_assert(std::is_convertible<decltype(T::key), std::string>::value,
        "T needs to have a string `key` attribute");
    std::fill(arr_, arr_ + M, nullptr);
}

// Desc:  Insert x into the Set, according to its ->key.
//        If ->key is already present, then replace it with x.
//        Collisions are resolved via quadratic probing sequence.
// Post:  returns the table index where x was inserted
//        returns -1 if x was not inserted
template <class T>
int Set<T>::insert(T* x) {
    static_assert(M > 1, "Array with min size = 2 is needed for this function's loop");
    if (x != nullptr) {
        const int init_index = impl::hash(x->key);
        int index = init_index;
        for (int i = 1; i < M; i++) {
            if (arr_[index] == nullptr || arr_[index]->key == x->key) {
                arr_[index] = x;
                return index;
            }
            const int next_index = (init_index + i * i) % M;
            if (next_index == index) {
                return -1;
            }
            index = next_index;
        }
    }
    return -1;
}

// Desc:  Returns T* x such that x->key == key, if and only if
//        key is in the Set.
//        Returns nullptr if and only if key is not in the Set.
// Post:  Set is unchanged
template <class T>
T* Set<T>::search(const std::string& key) const {
    static_assert(M > 1, "Array with min size = 2 is needed for this function's loop");
    const int init_index = impl::hash(key);
    int index = init_index;
    for (int i = 1; i < M; i++) {
        if (arr_[index] == nullptr || arr_[index]->key == key) {
            return arr_[index];
        }
        const int next_index = (init_index + i * i) % M;
        if (next_index == index) {
            return nullptr;
        }
        index = next_index;
    }
    return nullptr; // Will never get here: https://www.desmos.com/calculator/xrtpnpbgzr
}
