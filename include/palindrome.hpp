#ifndef PALINDROME_H 
#define PALINDROME_H

#include "alg_concepts.hpp"
#include <algorithm>

namespace alg {

namespace detail {

template <PalindromicContainer T, std::integral Q>
bool isPalindromeRecHelper(const T& container, Q beg, Q end) {
    if (beg >= end) return true;
    if (container[beg] != container[end]) return false;
    return isPalindromeRecHelper(container, beg+1, end-1); 
}

}

/**
 * Checks if the input container is a palindrome using recursion.
 * This is tail recursive.
 * We use a helper function that receives a pre-processed input.
 */
template <PalindromicContainer T>
bool isPalindromeRec(const T& container) {
    std::integral auto sz = container.size();
    if (!sz) return true;
    decltype(sz) end = sz-1;
    decltype(sz) beg = 0;
    return detail::isPalindromeRecHelper(container, beg, end);
}

/**
 * Checks if the input container is a palindrom using the STL.
 */
template <PalindromicContainer T>
bool isPalindromeIter(const T& container) {
    return std::equal(container.begin(), container.begin() + (container.size()/2), container.rbegin());
}

}

#endif // PALINDROME_H

