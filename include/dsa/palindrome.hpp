#pragma once

#include <algorithm>
#include "alg_concepts.hpp"
#include "common.hpp"

namespace alg {

namespace detail {

template <PalindromicContainer T>
bool isPalindromeRecHelper(const T& container, index beg, index end) {
    if (beg >= end) return true;
    if (container[beg] != container[end]) return false;
    return isPalindromeRecHelper(container, beg+1, end-1); 
}

} // namespace detail

/**
 * Checks if the input container is a palindrome using recursion.
 * This is tail recursive.
 * We use a helper function that receives a pre-processed input.
 */
template <PalindromicContainer T>
bool isPalindromeRec(const T& container) {
    index end = container.size() - 1;
    index beg = 0;
    return detail::isPalindromeRecHelper(container, beg, end);
}

/**
 * Checks if the input container is a palindrom using the STL.
 */
template <PalindromicContainer T>
bool isPalindromeIter(const T& container) {
    return std::equal(container.begin(), container.begin() + (container.size()/2), container.rbegin());
}

} // namespace alg
