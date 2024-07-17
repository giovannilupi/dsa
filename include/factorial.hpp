#ifndef FACTORIAL_H
#define FACTORIAL_H

#include <__concepts/arithmetic.h>
#include <concepts>
#include <cstdint>

namespace alg {

/**
 * Naive recursive implementation.
 * The size of the call stack grows linearly with the size of n. 
 */
template <std::unsigned_integral T>
uint64_t factRec(T n) {
    if (n == 0 || n == 1) return 1;
    return n * factRec(n-1);
}

/**
 * Tail recursive implementation.
 * This uses a second parameter to accumulate the factorial value.
 */
template <std::unsigned_integral T>
uint64_t factRecTail(T n, T accum = 1) {
    if (n == 0 || n == 1) return accum;
    return factRecTail(n-1, n * accum);
}

/**
 * Iterative implementation.
 */ 
template <std::unsigned_integral T>
uint64_t factIter(T n) {
    uint64_t res = 1;
    while(n) res *= n--;
    return res;
} 

}

#endif // !FACTORIAL_H

