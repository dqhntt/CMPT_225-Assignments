#include "a3.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <string_view>
#include <vector>
using namespace std;

namespace {
void addHeader(ofstream& file, string_view header) { file << header; }
void addHeader(ofstream& file) { addHeader(file, "input size (n), operation count\n"); }

void ssortOutputSample(string_view fileName) {
    ofstream file(fileName.data());
    addHeader(file);
    vector<int> v;
    for (int trials = 50; trials >= 0; trials--) {
        int cost = 0;
        v.resize(rand() % 20'000);
        iota(v.rbegin(), v.rend(), 0);
        ssort(v.data(), v.size(), 0, cost);
        file << v.size() << ", " << cost << '\n';
    }
}

void patternOutputSample(string_view fileName) {
    ofstream file(fileName.data());
    addHeader(file);
    for (int trials = 0; trials <= 30; trials++) { // Bigger -> int overflow.
        int cost = 0;
        const int n = pow(2, trials);
        pattern(n, 0, cost);
        file << n << ", " << cost << '\n';
    }
}

void lsearchOutputSample(string_view fileName) {
    ofstream file(fileName.data());
    addHeader(file);
    vector<int> v(29); // Bigger -> int overflow.
    for (int trials = 50; trials >= 0; trials--) {
        int cost = 0;
        v.resize(rand() % 29 + 1);
        iota(v.begin(), v.end(), 0);
        lsearch(v.data(), v.size(), v.back(), cost);
        file << v.size() << ", " << cost << '\n';
    }
}

// WARNING: Very unstable operation count, not sure why.
// Need to take care of heteroscedasticity.
void powOutputSample(string_view fileName) {
    ofstream file(fileName.data());
    addHeader(file);
    const int INT_MAX = 2'147'483'647;
    for (int trials = 1; trials < 10'000; trials++) {
        int cost = 0;
        pow(0, trials, cost);
        file << trials << ", " << cost << '\n';
    }
}
} // namespace

int main() {
    srand(time(0));
    ssortOutputSample("ssort.csv");
    patternOutputSample("pattern.csv");
    lsearchOutputSample("lsearch.csv");
    powOutputSample("pow.csv");
}
