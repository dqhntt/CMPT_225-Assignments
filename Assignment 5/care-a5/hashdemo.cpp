#include "hash.h"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class pod {
public:
    string key;
    pod() = default;
    pod(string key) : key(move(key)) { }
};

int main() {
    Set<pod> set;
    vector<pod> vec;
    vec.push_back({ "hello" });
    vec.push_back({ "banana" });
    vec.push_back({ "apple" });
    vec.push_back({ "apricot" });
    vec.push_back({ "candy" });
    vec.push_back({ "carkey" });
    vec.push_back({ "godefroy" });
    vec.push_back({ "airy" });
    vec.push_back({ "hugs" });
    vec.push_back({ "can" });
    vec.push_back({ "hello" });
    const int vec_size = vec.size();
    vector<int> pos(vec_size);

    for (int i = 0; i < vec_size; i++) {
        pos[i] = set.insert(vec.data() + i);
        cout << pos[i] << ' ' << vec[i].key << '\n';
    }

    assert(pos[0] == pos[10]); // "hello"
    for (int i = 1; i < vec_size; i++) {
        assert(set.search(vec[i].key) == vec.data() + i);
    }
    cout << set.search("can") << '\n';
    cout << set.search("hello") << '\n';
    cout << set.search("kumquat") << '\n';
    cout << set.search("cand") << '\n';
} // main
