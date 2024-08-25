#pragma once

#include <numeric>
#include <ranges>
#include <vector>
#include "common.hpp"

namespace alg {

/**
 * Returns the Levenshtein distance between two strings.
 * This uses a bottom-up space-optimal dynamic programming approach.
 * Time complexity: O(n * m)
 * Space complexity: O(m)
 */
template <std::ranges::random_access_range T>
    requires std::equality_comparable<std::ranges::range_value_t<T>>
index levenDist(const T& c1, const T& c2) {
    auto sz1 = c1.size();
    auto sz2 = c2.size();
    std::vector<index> table(sz2 + 1, 0);
    // Initialize first row
    std::iota(table.begin(), table.end(), 0);
    for (index i = 1; i <= sz1; ++i) {
        // We may need to access table[j-1], which is overwritten by the previous iteration
        index prev = table[0];
        // Initialize first column for each row
        table[0] = i;
        for (index j = 1; j <= sz2; ++j) {
            index curr = table[j];
            table[j] = (c1[i - 1] == c2[j - 1]) 
                ? prev  // no operation needed
                : 1 + std::min({
                    table[j - 1],  // deletion
                    table[j],      // insertion
                    prev});        // substitution
            prev = curr;
        }
    }
    return table.back();
}

} // namespace alg