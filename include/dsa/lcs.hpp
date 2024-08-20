#pragma once

#include <concepts>
#include <optional>
#include <string>
#include <vector>
#include "common.hpp"

namespace alg {

namespace detail {

/**
 * Helper function for the naive recursive implementation of the longest common subsequence.
 */
template <std::ranges::random_access_range T>
    requires std::equality_comparable<std::ranges::range_value_t<T>>
index lcsRecHelper(const T& c1, const T& c2, index i, index j) {
    if (i == 0 || j == 0) return 0;
    if (c1[i - 1] == c2[j - 1]) return 1 + lcsRecHelper(c1, c2, i - 1, j - 1);
    return std::max(lcsRecHelper(c1, c2, i - 1, j), lcsRecHelper(c1, c2, i, j - 1));
}

/**
 * Helper function for the top-down DP implementation of the longest common subsequence.
 */
template <std::ranges::random_access_range T>
    requires std::equality_comparable<std::ranges::range_value_t<T>>
index lcsMemoizedHelper(const T& c1, const T& c2, index i, index j, std::vector<std::vector<index>>& table) {
    if (i == 0 || j == 0) return 0;
    if (table[i][j] != -1) return table[i][j];
    if (c1[i - 1] == c2[j - 1]) return table[i][j] = 1 + lcsMemoizedHelper(c1, c2, i - 1, j - 1, table);
    return table[i][j] = std::max(lcsMemoizedHelper(c1, c2, i - 1, j, table), lcsMemoizedHelper(c1, c2, i, j - 1, table));
}

} // namespace detail

/**
 * Returns the length of the longest common subsequence of two containers.
 * This uses the naive recursive implementation.
 * Complexity: O(2^(min(n, m)))
 */
template <std::ranges::random_access_range T>
    requires std::equality_comparable<std::ranges::range_value_t<T>>
index lcsRec(const T& c1, const T& c2) {
    return detail::lcsRecHelper(c1, c2, c1.size(), c2.size());
}

/**
 * Returns the length of the longest common subsequence of two containers.
 * This uses top-down dynamic programming.
 * Complexity: O(n * m)
 */
template <std::ranges::random_access_range T>
    requires std::equality_comparable<std::ranges::range_value_t<T>>
index lcsMemoized(const T& c1, const T& c2) {
    auto sz1 = c1.size();
    auto sz2 = c2.size();
    std::vector<std::vector<index>> table(sz1 + 1, std::vector<index>(sz2 + 1, -1));
    return detail::lcsMemoizedHelper(c1, c2, c1.size(), c2.size(), table);
}

/**
 * Returns the length of the longest common subsequence of two containers.
 * This uses bottom-up dynamic programming.
 * Notice we only need to store the last row of the table.
 * Complexity: O(n * m)
 */
template <std::ranges::random_access_range T>
    requires std::equality_comparable<std::ranges::range_value_t<T>>
index lcsDP(const T& c1, const T& c2) {
    auto sz1 = c1.size();
    auto sz2 = c2.size();
    std::vector<index> table(sz2 + 1, 0);
    for (index i = 1; i <= sz1; ++i) {
        // We may need to access table[j-1], which is overwritten by the previous iteration
        index prev = 0;
        for (index j = 1; j <= sz2; ++j) {
            index curr = table[j];
            table[j] = (c1[i - 1] == c2[j - 1]) 
                ? 1 + prev 
                : std::max(table[j], table[j - 1]);
            prev = curr;
        }
    }
    return table[sz2];
}

} // namespace dsa
