#pragma once

#include <__concepts/arithmetic.h>
#include <concepts>
#include <cstdint>

namespace alg {

namespace detail {

/**
 * Helper function for tail recursive implementation of factorial.
 */
inline uint64_t factTailRecHelper(uint64_t n, uint64_t accum) {
    if (n == 0 || n == 1) return accum;
    return factTailRecHelper(n-1, n * accum);
}

}

/**
 * Naive recursive implementation.
 * The size of the call stack grows linearly with the size of n. 
 */
inline uint64_t factRec(uint64_t n) {
    if (n == 0 || n == 1) return 1;
    return n * factRec(n-1);
}

/**
 * Tail recursive implementation.
 * This uses a second parameter to accumulate the factorial value.
 */
inline uint64_t factTailRec(uint64_t n) {
    return detail::factTailRecHelper(n, 1);
}

/**
 * Iterative implementation.
 */ 
inline uint64_t factIter(uint64_t n) {
    uint64_t res = 1;
    while(n) res *= n--;
    return res;
} 

}
