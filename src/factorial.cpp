#include <concepts>

using namespace std;

/**
 * Naive recursive implementation.
 * The size of the call stack grows linearly with the size of n. 
 */
template <integral T>
long factRec(T n) {
    if (n == 0 || n == 1) return 1;
    return n * factRec(n-1);
}

/**
 * Tail recursive implementation.
 * This uses a second parameter to accumulate the factorial value.
 */
template <integral T>
long factRecTail(T n, T accum = 1) {
    if (n == 0 || n == 1) return accum;
    return factRecTail(n-1, n * accum);
}

/**
 * Iterative implementation.
 */ 
template <integral T>
long factIter(T n) {
    long res = 1;
    while(n) res *= n--;
    return res;
} 
