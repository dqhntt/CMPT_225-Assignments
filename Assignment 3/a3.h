// a3.h
#pragma once
#include <iostream>
#include <string>

#define COUNT_ADD(n)     ops += n;
#define COUNT            COUNT_ADD(1)
#define COUNT_LOOP_BODY  COUNT_ADD(1)
#define COUNT_LOOP_END   COUNT_ADD(1)

////////////////////////////////////// Begin: Question 1 //////////////////////////////////////
// PARAM: arr is array to print the Cartesian product of, n is size of arr
void cartesianProduct(int arr[], int n, int& ops) {
    COUNT int i = 0;
    while (i < n) {
        COUNT_LOOP_BODY

        COUNT int j = 0;
        while (j < n) {
            COUNT_LOOP_BODY

            COUNT std::cout << "{" << arr[i] << "," << arr[j] << "}";
            COUNT j++;
            COUNT std::cout << " ";
        } // while
        COUNT_LOOP_END

        COUNT std::cout << '\n';
        COUNT i++;
    } // while
    COUNT_LOOP_END
}
/////////////////////////////////////// End: Question 1 ///////////////////////////////////////

////////////////////////////////////// Begin: Question 2 //////////////////////////////////////
void triangle(int x, int& ops) {
    COUNT int i = 0;
    while (i < x) {
        COUNT_LOOP_BODY

        COUNT int j = 0;
        while (j <= i) {
            COUNT_LOOP_BODY

            COUNT std::cout << j << " ";
            COUNT j++;
        } // while
        COUNT_LOOP_END

        COUNT std::cout << '\n';
        COUNT i++;
    } // while
    COUNT_LOOP_END

    while (i > 0) {
        COUNT_LOOP_BODY

        COUNT i--;
        COUNT int j = 0;
        while (j <= i) {
            COUNT_LOOP_BODY

            COUNT std::cout << j << " ";
            COUNT j++;
        } // while
        COUNT_LOOP_END

        COUNT std::cout << '\n';
    } // while
    COUNT_LOOP_END
}
/////////////////////////////////////// End: Question 2 ///////////////////////////////////////

////////////////////////////////////// Begin: Question 3 //////////////////////////////////////
// Returns the index of a 1d array representing a matrix
// given row (r) and column (c) values
int rcIndex(int r, int c, int columns, int& ops) { COUNT return r * columns + c; }

// PRE: m represents a square matrix of size rows * rows
// PARAM: rows represents the number of both rows and columns
// POST: Returns a pointer to an array (matrix) of the same size as m
// NOTE: values are indexed r0c0,r0c1,…,r0cn-1,r1c0,…
int* matrixSelfMultiply(int* m, int rows, int& ops) {
    // Create result array
    COUNT int columns = rows;
    COUNT int* result = new int[rows * columns];
    COUNT int r = 0;

    while (r < rows) {
        COUNT_LOOP_BODY

        COUNT int c = 0;
        while (c < columns) { // columns = rows
            COUNT_LOOP_BODY

            COUNT int next = 0;
            COUNT int iNext = 0;
            while (iNext < rows) {
                COUNT_LOOP_BODY

                COUNT next
                    += m[rcIndex(r, iNext, columns, ops)] * m[rcIndex(iNext, c, columns, ops)];
                COUNT iNext++;
            } // while
            COUNT_LOOP_END

            COUNT result[rcIndex(r, c, columns, ops)] = next;
            COUNT c++;
        } // while
        COUNT_LOOP_END

        COUNT r++;
    } // while
    COUNT_LOOP_END

    return result;
}
/////////////////////////////////////// End: Question 3 ///////////////////////////////////////

////////////////////////////////////// Begin: Question 4 //////////////////////////////////////
// PARAM: arr is array to be sorted, n is size of array, i should initially = 0
void ssort(int arr[], int n, int i, int& ops) {
    COUNT if (i < n - 1) {
        // Find and swap smallest remaining
        COUNT int next = i + 1;
        COUNT int smallest = i;
        while (next < n) {
            COUNT_LOOP_BODY

            COUNT if (arr[next] < arr[smallest]) { COUNT smallest = next; }
            COUNT next++;
        } // while
        COUNT_LOOP_END

        // Swap i with smallest
        COUNT int temp = arr[i];
        COUNT arr[i] = arr[smallest];
        COUNT arr[smallest] = temp;
        ssort(arr, n, i + 1, ops);
    } // if
}
/////////////////////////////////////// End: Question 4 ///////////////////////////////////////

////////////////////////////////////// Begin: Question 5 //////////////////////////////////////
// PRE: n is a power of 2 greater than zero.
// PRE: Initial call should be to i = 0
// e.g. pattern(8, 0)
void pattern(int n, int i, int& ops) {
    COUNT if (n > 0) {
        pattern(n / 2, i, ops);
        // Print i spaces
        COUNT std::cout << std::string(i, ' ');
        COUNT_ADD(3)

        // A loop to print n asterisks, each one followed by a space
        COUNT int ast = 0;
        while (ast < n) {
            COUNT_LOOP_BODY

            COUNT std::cout << "* ";
            COUNT ast++;
        } // while
        COUNT_LOOP_END

        COUNT std::cout << '\n';
        COUNT i += n;
        pattern(n / 2, i, ops);
    } // if
}
/////////////////////////////////////// End: Question 5 ///////////////////////////////////////

////////////////////////////////////// Begin: Question 6 //////////////////////////////////////
// Desc: Linear search.  Reports position if found, else -1
// Post:  Elements unchanged
int lsearch(int arr[], unsigned len, int target, int& ops) {
    COUNT if (len == 0) { return -1; }
    COUNT if (arr[0] == target) { return 0; }
    COUNT if (lsearch(arr + 1, len - 1, target, ops) == -1) { return -1; }
    COUNT return 1 + lsearch(arr + 1, len - 1, target, ops);
} // lsearch
/////////////////////////////////////// End: Question 6 ///////////////////////////////////////

////////////////////////////////////// Begin: Question 7 //////////////////////////////////////
unsigned pow(unsigned base, unsigned exp, int& ops) {
    COUNT unsigned ret = 1;
    while (exp > 0) {
        COUNT_LOOP_BODY

        COUNT if (exp & 1) { // Same as: if (exp % 2 == 1)
            COUNT ret *= base;
        }
        COUNT exp >>= 1; // Same as: exp /= 2;
        COUNT base *= base;
    } // while
    COUNT_LOOP_END

    return ret;
} // pow
/////////////////////////////////////// End: Question 7 ///////////////////////////////////////
