#include "alg_concepts.hpp"

using namespace std;

/**
 * Checks if the input container is a palindrome using recursion.
 * This is tail recursive.
 * We use a helper function that receives a pre-processed input.
 */
template <PalindromicContainer T, integral Q>
bool isPalindromeRecHelper(const T& container, Q beg, Q end) {
    if (beg >= end) return true;
    if (container[beg] != container[end]) return false;
    return isPalindromeRecHelper(container, beg+1, end-1); 
}
template <PalindromicContainer T>
bool isPalindromeRec(const T& container) {
    integral auto end = container.size() - 1;
    decltype(end) beg = 0;
    return isPalindromeRecHelper(container, beg, end);
}

/**
 * Checks if the input container is a palindrom using the STL.
 */
template <PalindromicContainer T>
bool isPalindromeIter(const T& container) {
    return equal(container.begin(), container.begin() + (container.size()/2), container.rbegin());
}
