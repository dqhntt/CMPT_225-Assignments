#include "a3.h"
#include <iostream>
#include <cmath>
#include <string>
using namespace std;

void cartesianProductTest(int arr[], int n);
void triangleTest(int n);
void matrixSelfMultiplyTest(int m[], int n);
void ssortTest(int arr[], int n);
void patternTest(int n);                          // parameter should be a power of 2
void lsearchTest(int arr[], int len, int target); // first element in array
void powTest(int base, int exp);

// -------------------------------------------------------- //
// -------------------- COST FUNCTIONS -------------------- //
// -------------------------------------------------------- //
int cartesianProductCost(int n) { return 4 * n * n + 5 * n + 2; }
int triangleCost(int n) { return 3 * n * n + 13 * n + 3; }
int matrixSelfMultiplyCost(int n) { return 5 * n * n * n + 7 * n * n + 4 * n + 4; }
int ssortCost(int n) { return (7 / 4.0) * n * n + (11 / 2.0) * n - 6; }
int patternCost(int n) { return 3 * n * log2(n) + 23 * n - 9; }
int lsearchCost(int n) { return 3 * pow(2, n) - 4; }
int powCost(int n) { return 5 * log2(n + 1) + 4; }

// -------------------------------------------------------- //
// -------------------- MAIN FUNCTION --------------------- //
// -------------------------------------------------------- //
int main() {
    int i;
    int zero = 0;

    /* Array and matrix shared by some tests */
    const int N = 5;
    int arr[N] = {};
    int matrix[N * N] = {};
    for (i = N - 1; i >= 0; i--) {
        arr[i] = i;
        matrix[i] = i;
    }

    /* Tests with inputs of size 0 to N-1 */
    for (i = 0; i < N; i++) {
        cartesianProductTest(arr, i);
    }
    for (i = 0; i < N; i++) {
        triangleTest(i);
    }
    for (i = 0; i < N; i++) {
        matrixSelfMultiplyTest(matrix, i);
    }
    for (i = 0; i < N; i++) {
        ssortTest(arr, i);
    }
    for (i = 0; i < N; i++) {
        patternTest(pow(2, i, zero));
    } // parameter should be a power of 2

    /* Linear search with different cases */
    const int len = 3;
    int L[len] = {};
    for (i = 0; i < len; i++) {
        L[i] = i;
    }
    lsearchTest(L, len, 0);       // first element in array
    lsearchTest(L, len, len);     // not in array
    lsearchTest(L, len, len - 1); // at end of array

    /* Power function with different bases (default: 2-4) and powers (default: 0-4) */
    for (i = 2; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            powTest(i, j);
        }
    }
}

// -------------------------------------------------------- //
// ----------------------- TESTS -------------------------- //
// -------------------------------------------------------- //

void cartesianProductTest(int arr[], int n) {
    cout << "\n--------------\n";
    cout << "CARTESIAN PRODUCT TEST\n";

    int cost = 0;
    cartesianProduct(arr, n, cost);

    const int solution = cartesianProductCost(n);
    cout << "| cost: " << cost;
    cout << "\t| t(function): " << solution;
    if (cost == solution) {
        cout << "\t**MATCH**";
    } else {
        cout << "\t**MISMATCH**";
    }
    cout << "\n--------------\n";
}

void triangleTest(int n) {
    cout << "\n--------------\n";
    cout << "TRIANGLE TEST\n";

    int cost = 0;

    triangle(n, cost);

    const int solution = triangleCost(n);
    cout << "\n| cost: " << cost;
    cout << "\t| t(function): " << solution;
    if (cost == solution) {
        cout << "\t**MATCH**";
    } else {
        cout << "\t**MISMATCH**";
    }
    cout << "\n--------------\n";
}

void matrixSelfMultiplyTest(int m[], int n) {
    cout << "\n--------------\n";
    cout << "MATRIX SELF MULTIPLY TEST\n";

    cout << "matrix = { ";
    for (int i = 0; i < n; i++) {
        cout << m[i] << " ";
    }
    cout << "}\n";
    int cost = 0;

    delete[] matrixSelfMultiply(m, n, cost);

    cout << "matrix = { ";
    for (int i = 0; i < n; i++) {
        cout << m[i] << " ";
    }
    cout << "}\n";

    const int solution = matrixSelfMultiplyCost(n);
    cout << "| cost: " << cost;
    cout << "\t| t(function): " << solution;
    if (cost == solution) {
        cout << "\t**MATCH**";
    } else {
        cout << "\t**MISMATCH**";
    }
    cout << "\n--------------\n";
}

void ssortTest(int arr[], int n) {
    cout << "\n--------------\n";
    cout << "SSORT TEST\n";

    cout << "array = { ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << "}\n";

    int cost = 0;

    ssort(arr, n, 0, cost);
    cout << "ssort = { ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << "}";

    const int solution = ssortCost(n);
    cout << "\n| cost: " << cost;
    cout << "\t| t(function): " << solution;
    if (cost == solution) {
        cout << "\t**MATCH**";
    } else {
        cout << "\t**MISMATCH**";
    }
    cout << "\n--------------\n";
}

void patternTest(int n) {
    cout << "\n--------------\n";
    cout << "PATTERN TEST: n=" << n << '\n';

    int i = 0;
    int cost = 0;

    pattern(n, i, cost);

    const int solution = patternCost(n);
    cout << "| cost: " << cost;
    cout << "\t| t(function): " << solution;
    if (cost == solution) {
        cout << "\t**MATCH**";
    } else {
        cout << "\t**MISMATCH**";
    }
    cout << "\n--------------\n";
}

void lsearchTest(int arr[], int len, int target) {
    cout << "\n--------------\n";
    cout << "LSEARCH TEST: ";

    int cost = 0;
    cout << "search for " << target << " in { ";
    for (int i = 0; i < len; i++) {
        cout << arr[i] << " ";
    }
    cout << "} found at index ";
    const int index = lsearch(arr, len, target, cost);
    cout << index;

    const int solution = lsearchCost(len);
    cout << "\t| cost: " << cost;
    cout << "\t| t(function): " << solution;
    if (cost == solution) {
        cout << "\t**MATCH**";
    } else {
        cout << "\t**MISMATCH**";
    }
    cout << "\n--------------\n";
}

void powTest(int base, int exp) {
    cout << "\n--------------\n";
    cout << "POW TEST: ";

    int cost = 0;

    const unsigned result = pow(base, exp, cost);
    cout << base << "^" << exp << " = " << result;

    const int solution = powCost(exp);
    cout << "\t| cost: " << cost;
    cout << "\t| t(function): " << solution;
    if (cost == solution) {
        cout << "\t**MATCH**";
    } else {
        cout << "\t**MISMATCH**";
    }

    cout << "\n--------------\n";
}
