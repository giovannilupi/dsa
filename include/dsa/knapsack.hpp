#pragma once

#include <concepts>
#include <cstddef>
#include <type_traits>
#include <vector>
#include "common.hpp"

namespace alg {

namespace detail {

/**
 * Helper function for the naive recursive implementation of the knapsack algorithm.
 */
template <std::ranges::random_access_range TCont>
    requires std::signed_integral<std::ranges::range_value_t<TCont>>
typename TCont::value_type knapsackRecHelper(typename TCont::value_type capacity, 
                                             const TCont& weights, 
                                             const TCont& values,
                                             index idx) {
    if (idx == 0 || capacity == 0) return 0;
    // The weight is greater than the capacity, the item cannot be included
    if (weights[idx - 1] > capacity) return knapsackRecHelper(capacity, weights, values, idx - 1);
    // Maximum between this item included and not included
    return std::max(values[idx - 1] + knapsackRecHelper(capacity - weights[idx - 1], weights, values, idx - 1), 
                    knapsackRecHelper(capacity, weights, values, idx - 1));
}

/**
 * Helper function for the memoized recursive implementation of the knapsack algorithm.
 */
template <std::ranges::random_access_range TCont>
    requires std::signed_integral<std::ranges::range_value_t<TCont>>
typename TCont::value_type knapsackMemoizedHelper(typename TCont::value_type capacity, 
                                                  const TCont& weights, 
                                                  const TCont& values,
                                                  index idx,
                                                  std::vector<std::vector<typename TCont::value_type>>& table) {
    if (idx == 0 || capacity == 0) return 0;
    if (table[idx][capacity] != -1) return table[idx][capacity];
    if (weights[idx-1] > capacity) return table[idx][capacity] = knapsackMemoizedHelper(capacity, weights, values, idx - 1, table);
    return table[idx-1][capacity] = std::max(values[idx-1] + knapsackMemoizedHelper(capacity - weights[idx-1], weights, values, idx - 1, table), 
                                             knapsackMemoizedHelper(capacity, weights, values, idx - 1, table));
}

} // namespace detail

/**
 * Returns the maximum value that can be stored in the knapsack with the given capacity.
 * This uses the naive recursive implementation.
 * Complexity: O(2^n).
 */
template <std::ranges::random_access_range TCont>
    requires std::signed_integral<std::ranges::range_value_t<TCont>>
typename TCont::value_type knapsackRec(typename TCont::value_type capacity, 
                                       const TCont& weights, 
                                       const TCont& values) {
    std::size_t sz = weights.size();
    if (sz != values.size()) throw std::invalid_argument("Weights and values have different sizes");
    return detail::knapsackRecHelper(capacity, weights, values, sz);
}

/**
 * Returns the maximum value that can be stored in the knapsack with the given capacity.
 * This uses the memoized recursive implementation.
 * Complexity: O(n * capacity).
 */
template <std::ranges::random_access_range TCont>
    requires std::signed_integral<std::ranges::range_value_t<TCont>>
typename TCont::value_type knapsackMemoized(typename TCont::value_type capacity, 
                                            const TCont& weights, 
                                            const TCont& values) {
    std::size_t sz = weights.size();
    if (sz != values.size()) throw std::invalid_argument("Weights and values have different sizes");
    std::vector<std::vector<typename TCont::value_type>> table(sz+1, std::vector<typename TCont::value_type>(capacity + 1, -1));
    return detail::knapsackMemoizedHelper(capacity, weights, values, sz, table);
}

/**
 * Returns the maximum value that can be stored in the knapsack with the given capacity.
 * This uses bottom-up dynamic programming.
 * Notice we only need to store the last row of the table.
 * Complexity: O(n * capacity).
 */
template <std::ranges::random_access_range TCont>
    requires std::signed_integral<std::ranges::range_value_t<TCont>>
typename TCont::value_type knapsackDP(typename TCont::value_type capacity, 
                                      const TCont& weights, 
                                      const TCont& values) {
    std::size_t sz = weights.size();
    if (sz != values.size()) throw std::invalid_argument("Weights and values have different sizes");
    std::vector<typename TCont::value_type> table(capacity + 1, 0);
    // Iterate over each item
    for (std::size_t i = 0; i < sz; ++i) {
        // Iterate backwards over the table row to avoid overwriting values we need
        for (typename TCont::value_type w = capacity; w >= weights[i]; --w) {
            table[w] = std::max(table[w], values[i] + table[w - weights[i]]);
        }
    }
    return table[capacity];
}

} // namespace alg
