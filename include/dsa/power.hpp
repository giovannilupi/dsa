#pragma once

#include <concepts>
#include <cstdint>

namespace alg {

namespace detail {

/**
 * Helper function for tail recursive implementation of integer based power.
 */
template <std::integral T>
int64_t powTailRecHelper(T base, uint8_t exp, int64_t accum = 1) {
    if (exp == 0) return accum;
    return powTailRecHelper(base, exp-1, accum * base);
}

/**
 * Helper function for tail recursive integer based square and multiply.
 */ 
template <std::integral T>
int64_t powSquaringTailRecHelper(T base, uint8_t exp, T accum = 1) {
    if (exp == 0) return accum;
    if (exp & 1) return powSquaringTailRecHelper(base*base, (exp-1)>>1, accum*base);
    return powSquaringTailRecHelper(base*base, exp>>1, accum);
}

} // namespace detail

/**
 * Naive recursive implementation of integer based power.
 * There is no accurate control on the size of the operands, so the result can overflow.
 */
template <std::integral T>
int64_t powRec(T base, uint8_t exp) {
    if (exp == 0) return 1;
    return base * powRec(base, exp-1);
}

/**
 * Tail recursive implementation of integer based power.
 */ 
template <std::integral T>
int64_t powTailRec(T base, uint8_t exp) {
    return detail::powTailRecHelper(base, exp);
}

/**
 * Iterative implementation of integer based power.
 */
template <std::integral T>
int64_t powIter(T base, uint8_t exp) {
    double res = 1;
    while (exp) { res *= base; --exp; }
    return res;
}

/**
 * Implementation of exponentiation using squaring method.
 * Uses O(log n) multiplications instead 
 */
template <std::integral T>
int64_t powSquaring(T base, uint8_t exp) {
    int64_t res = 1;
    while (exp) {
        if (exp & 1) res *= base; 
        exp >>= 1;
        base *= base;
    }
    return res;
}

/**
 * Recursive square and multiply implementation of integer based power.
 */
template <std::integral T>
int64_t powSquaringRec(T base, uint8_t exp) {
    if (exp == 0) return 1;
    if (exp & 1) return base * powSquaringRec(base*base, (exp-1)>>1);
    return powSquaringRec(base*base, exp>>1);
}

/**
 * Tail recursive square and multiply implementation of integer based power.
 */
template <std::integral T>
int64_t powSquaringTailRec(T base, uint8_t exp) {
    return detail::powSquaringTailRecHelper(base, exp);
}

} // namespace alg
