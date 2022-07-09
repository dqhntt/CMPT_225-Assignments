#include "a3.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>
using namespace std;

void ssortHelper(vector<int> copy, int& ops) { ssort(copy.data(), copy.size(), 0, ops); }

ostream& operator<<(ostream& os, const vector<int>& v) {
    os << "{ ";
    for (int n : v) {
        os << n << ' ';
    }
    os << "}";
    return os;
}

int main() {
    const int inputSize = 4;
    vector<int> v(inputSize);
    iota(v.begin(), v.end(), 1); // Fill ascending.
    map<int, vector<int>> stat;
    do {
        int cost = 0;
        ssortHelper(v, cost);
        cout << v << " :: " << cost << '\n';
        stat[cost] = v;
    } while (next_permutation(v.begin(), v.end()));

    for (const auto& [cost, vec] : stat) {
        cout << cost << " :: " << vec << '\n';
    }
}