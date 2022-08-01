#include "hash.h"
#include <cassert>
#include <iostream>
#include <string>
using namespace std;

class pod {
public:
    string key;
    pod() = default;
    pod(string key) : key(move(key)) { }
};

int main() {
    for (char c = '0'; c <= '9'; c++) {
        cout << c << " -- " << impl::mapChar(c) << "\n";
    }
    for (char c = 'a'; c <= 'z'; c++) {
        cout << c << " -- " << impl::mapChar(c) << "\n";
    }
    for (char c = 'A'; c <= 'Z'; c++) {
        cout << c << " -- " << impl::mapChar(c) << "\n";
    }
    char c = '_';
    cout << c << " -- " << impl::mapChar(c) << "\n";

    return 0;

    Set<pod> set;
    pod a[10];
    a[0] = pod("hello");
    a[1] = pod("banana");
    a[2] = pod("apple");
    a[3] = pod("apricot");
    a[4] = pod("candy");
    a[5] = pod("carkey");
    a[6] = pod("godefroy");
    a[7] = pod("airy");
    a[8] = pod("hugs");
    a[9] = pod("can");
    int pos[10];

    for (int i = 0; i < 10; i++) {
        pos[i] = set.insert(a + i);
        cout << pos[i] << ' ' << a[i].key << endl;
    }

    for (int i = 0; i < 10; i++) {
        assert(set.search(a[i].key) == a + i);
    }
    cout << set.search("kumquat") << endl;
    cout << set.search("cand") << endl;
} // main
