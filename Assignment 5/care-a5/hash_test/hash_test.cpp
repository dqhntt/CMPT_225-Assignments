#include "../hash.h"
#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
#include <boost/multiprecision/cpp_int.hpp>
using boost::multiprecision::cpp_int;

unordered_map<char, int> char_map() {
    unordered_map<char, int> map;
    map.reserve(62);
    int i = 0;
    for (char c = '0'; c <= '9'; c++) {
        map[c] = i;
        i++;
    }
    for (char c = 'a'; c <= 'z'; c++) {
        map[c] = i;
        i++;
    }
    for (char c = 'A'; c <= 'Z'; c++) {
        map[c] = i;
        i++;
    }
    map['_'] = i;
    return map;
}

const auto MAP = char_map();

vector<cpp_int> n_pows(int n, unsigned max_pow) {
    vector<cpp_int> pows;
    pows.reserve(max_pow);
    pows.emplace_back(1); // n^0
    const cpp_int num = n;
    for (unsigned i = 1; i <= max_pow; i++) {
        pows.emplace_back(pows.back() * num);
    }
    return pows;
}

const auto POWS_64_UP_TO_12 = n_pows(64, 12);

// DO NOT COPY!
int slow_hash(const string& s) {
    const auto length = s.length();
    const auto& pows_64 = (length <= 13u) ? POWS_64_UP_TO_12 : n_pows(64, length);
    cpp_int result = 0;
    for (unsigned i = 0; i < length; i++) {
        result += (MAP.at(s[i]) * pows_64[length - i - 1]);
    }
    result = (225 * result) % M;
    return result.convert_to<int>();
}

class pod {
public:
    string key;
};

int main() {
    cout << "M = " << M << "\n"
         << "No failed assert == OK.\n"
            "OK != No bugs.\n\n";

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
    vec.push_back({ "can90" });
    vec.push_back({ "hello" });
    vec.push_back({ "longstringg" });
    vec.push_back({ "LargeStrinGAZ" });
    vec.push_back({ "_Odd_Str_0954" });
    const int vec_size = vec.size();
    vector<int> pos(vec_size);

    for (int i = 0; i < vec_size; i++) {
        pos[i] = set.insert(vec.data() + i);
        cout << pos[i] << ' ' << vec[i].key << '\n';

        const int expected_pos = slow_hash(vec[i].key);
        if (pos[i] != expected_pos) {
            cout << "\nERROR: Expecting: " << expected_pos << " for " << vec[i].key << '\n';
            assert(false && "Incorrect hash.");
        }
    }

    // "hello" updated.
    assert(pos[0] == pos[10]);

    // search.
    for (int i = 1; i < vec_size; i++) {
        assert(set.search(vec[i].key) == vec.data() + i);
    }

    // keys not found.
    for (unsigned i = 0; i < M; i++) {
        assert(set.search(to_string(i)) == nullptr);
    }

    cout << '\n';
} // main
