#include "Stack.h"
#include <array>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>

void reverseStackTest(Stack& st) {
    std::vector<int> revTestVals;
    Stack revStack;
    while (!st.isEmpty()) {
        const int pop = st.pop();
        revTestVals.push_back(pop);
        revStack.push(pop);
    }
    assert(st.isEmpty());
    for (int j = revTestVals.size() - 1; j >= 0; --j) {
        assert(revStack.pop() == revTestVals[j]);
    }
}

int main() {
    srand(time(0));
    const int trials = 1e2;
    std::array<int, 8> testVals;

    std::cout << "Begin testing...\n";

    for (int i = 0; i < trials; ++i) {

        std::cout << "Trial #" << i << " ..";
        for (int& v : testVals) {
            v = rand();
        }

        const int randSize = rand() % testVals.size();
        for (int j = 0; j < randSize; ++j) {
            const int n = testVals[j];
            Stack st;
            assert(st.isEmpty());
            st.push(n);
            assert(!st.isEmpty());
            assert(st.peek() == n);
            assert(st.pop() == n);
            assert(st.isEmpty());
        }

        Stack st;
        for (int n : testVals) {
            st.push(n);
        }
        reverseStackTest(st);

        std::cout << "passed.\n";
    }

    std::cout << "...testing completed.\n";
}
