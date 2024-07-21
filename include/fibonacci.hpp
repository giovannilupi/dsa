#pragma once

#include <concepts>

namespace alg {

namespace detail {

/**
 * Helper function for tail recursive implementation of fibonacci sequence.
 */
inline uint64_t fibTailRec(uint64_t n, uint64_t a = 0, uint64_t b = 1) {
    if (n == 0) return a;
    if (n == 1) return b;
    // b becomes the previous number in the sequence
    return fibTailRec(n-1, b, a+b); 
}

}


/**
 * Naive recursive implementation.
 * The number of recursive calls grows exponentially with n.
 * This apporach becomes quickly unpractical even for a relatively small n.
 */ 
inline uint64_t fibRec(uint64_t n) {
    if (n == 0 || n == 1) return n;
    return fibRec(n-1) + fibRec(n-2);
}

/**
 * Tail recursive implementation.
 * The extra parameters form a size two window over the sequence.
 * The function computes the next number in the sequence and shifts the window by one at every iteration.
 */
inline uint64_t fibTailRec(uint64_t n) {
    return detail::fibTailRec(n);
}

/** 
 * Iterative implementation.
 * This uses a dynamic programming approach called tabulation.
 * We proceed bottom-up storing the result of each subproblem.
 */
inline uint64_t fibIter(uint64_t n) {
    if (n == 0 || n == 1) return n;
    uint64_t prev = 0, curr = 1;
    for (uint64_t i = 2; i <= n; ++i)  {
        uint64_t temp = curr;
        curr += prev;
        prev = temp;
    }
    return curr;
}

}
