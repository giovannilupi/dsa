#include <concepts>

using namespace std;

/**
 * Naive recursive implementation.
 * The number of recursive calls grows exponentially with n.
 * This apporach becomes quickly unpractical even for a relatively small n.
 */ 
template <integral T>
T fibRec(T n) {
    if (n == 0 || n == 1) return n;
    return fibRec(n-1) + fibRec(n-2);
}

/**
 * Tail recursive implementation.
 * The extra parameters form a size two window over the sequence.
 * The function computes the next number in the sequence and shifts the window by one at every iteration.
 */
template <integral T>
T fibTailRec(T n, T a = 0, T b = 1) {
    if (n == 0) return a;
    if (n == 1) return b;
    // b becomes the previous number in the sequence
    return fibTailRec(n-1, b, a+b); 
}

/** 
 * Iterative implementation.
 * This uses a dynamic programming approach called tabulation.
 * We proceed bottom-up storing the result of each subproblem.
 */
template <integral T>
T fibIter(T n) {
    if (n == 0 || n == 1) return n;
    T prev = 0, curr = 1;
    for (T i = 2; i != n; ++i)  {
        curr += prev;
        // prev becomes what was in curr before its increment
        prev ^= curr;
    }
    return curr;
}

