#pragma once

#include <concepts>
#include <cstddef>
#include <queue>
#include <type_traits>
#include <vector>
#include "common.hpp"

namespace alg {

namespace detail {

/**
 * Helper function for the naive recursive implementation of the knapsack algorithm.
 * It proceeds backward from the last item to the first one.
 * This simulates the classic problem formulation:
 * to solve the problem for n items, we need to know the solution for n-1 items.
 */
template <std::ranges::random_access_range TCont>
    requires std::signed_integral<std::ranges::range_value_t<TCont>>
typename TCont::value_type knapsackRecHelper(typename TCont::value_type capacity, 
                                             const TCont& weights, 
                                             const TCont& values,
                                             index n) {
    if (n == 0 || capacity == 0) return 0;
    // The weight is greater than the capacity, the item cannot be included
    if (weights[n - 1] > capacity) return knapsackRecHelper(capacity, weights, values, n - 1);
    // Maximum between this item included and not included
    return std::max(values[n - 1] + knapsackRecHelper(capacity - weights[n - 1], weights, values, n - 1), 
                    knapsackRecHelper(capacity, weights, values, n - 1));
}

/**
 * Helper function for the memoized recursive implementation of the knapsack algorithm.
 */
template <std::ranges::random_access_range TCont>
    requires std::signed_integral<std::ranges::range_value_t<TCont>>
typename TCont::value_type knapsackMemoizedHelper(typename TCont::value_type capacity, 
                                                  const TCont& weights, 
                                                  const TCont& values,
                                                  index n,
                                                  std::vector<std::vector<typename TCont::value_type>>& dp) {
    if (n == 0 || capacity == 0) return 0;
    if (dp[n][capacity] != -1) return dp[n][capacity];
    if (weights[n-1] > capacity) return dp[n][capacity] = knapsackMemoizedHelper(capacity, weights, values, n - 1, dp);
    return dp[n-1][capacity] = std::max(values[n-1] + knapsackMemoizedHelper(capacity - weights[n-1], weights, values, n - 1, dp), 
                                             knapsackMemoizedHelper(capacity, weights, values, n - 1, dp));
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
    std::vector<std::vector<typename TCont::value_type>> dp(sz+1, std::vector<typename TCont::value_type>(capacity + 1, -1));
    return detail::knapsackMemoizedHelper(capacity, weights, values, sz, dp);
}

/**
 * Returns the maximum value that can be stored in the knapsack with the given capacity.
 * This uses a bottom-up dynamic programming approach.
 * Complexity: O(n * capacity).
 */
template <std::ranges::random_access_range TCont>
    requires std::signed_integral<std::ranges::range_value_t<TCont>>
typename TCont::value_type knapsackDP(typename TCont::value_type capacity, 
                                      const TCont& weights, 
                                      const TCont& values) {
    std::size_t sz = weights.size();
    if (sz != values.size()) throw std::invalid_argument("Weights and values have different sizes");
    std::vector<std::vector<typename TCont::value_type>> dp(sz+1, std::vector<typename TCont::value_type>(capacity + 1, 0));
    // Iterate over each item
    for (std::size_t i = 1; i <= sz; ++i) {
        // Iterate over each capacity
        for (std::size_t j = 1; j <= capacity; ++j) {
            dp[i][j] = (weights[i-1] > j)
                // This item cannot be included
                ? dp[i-1][j]
                // Take the max between this item included and not included
                : std::max(values[i-1] + dp[i-1][j - weights[i-1]], dp[i-1][j]);
        }
    }
    return dp[sz][capacity];         
}

/**
 * Returns the maximum value that can be stored in the knapsack with the given capacity.
 * This uses a memory-optimized bottom-up dynamic programming approach.
 * Notice we only need to store the last row of the table.
 * Complexity: O(n * capacity).
 */
template <std::ranges::random_access_range TCont>
    requires std::signed_integral<std::ranges::range_value_t<TCont>>
typename TCont::value_type knapsackDPOptimized(typename TCont::value_type capacity, 
                                               const TCont& weights, 
                                               const TCont& values) {
    std::size_t sz = weights.size();
    if (sz != values.size()) throw std::invalid_argument("Weights and values have different sizes");
    std::vector<typename TCont::value_type> dp(capacity + 1, 0);
    // Iterate over each item
    for (std::size_t i = 0; i < sz; ++i) {
        // Iterate backwards over the dp row to avoid overwriting values we need
        for (typename TCont::value_type w = capacity; w >= weights[i]; --w) {
            dp[w] = std::max(dp[w], values[i] + dp[w - weights[i]]);
        }
    }
    return dp[capacity];
}

/**
 * Returns the maximum value that can be stored in the knapsack with the given capacity.
 * This uses the fractional knapsack algorithm,
 * meaning we are allowed to take fractions of each item.
 * This uses a greedy approach.
 * Complexity: O(n log n).
 */
template <std::ranges::random_access_range TCont>
    requires std::signed_integral<std::ranges::range_value_t<TCont>>
double knapsackFractional(double capacity,
                          const TCont& weights,
                          const TCont& values) {
    std::size_t sz = weights.size();
    if (sz != values.size()) throw std::invalid_argument("Weights and values have different sizes");
    // Fast retrieval of item with largest value-to-weight ratio and its index in the weights container
    std::priority_queue<std::pair<double, index>> heap;
    // Fill the heap
    for (std::size_t i = 0; i != sz; ++i) {
        if (!weights[i]) throw std::invalid_argument("Weight cannot be zero");
        double ratio = static_cast<double>(values[i]) / static_cast<double>(weights[i]);
        heap.emplace(ratio, i);
    }
    double res = 0.0;
    while (capacity > 0.0 && !heap.empty()) {
        auto [ratio, idx] = heap.top();
        heap.pop();
        // If the current item fits in the knapsack, we add it completely
        if (weights[idx] <= capacity) {
            res += values[idx];
            capacity -= weights[idx];
        }
        // Otherwise, we add the biggest possible fraction of it until the knapsack is full
        else {
            res += ratio * capacity;
            break;
        }
    }
    return res;
}

/**
 * Returns the maximum value that can be stored in the knapsack with the given capacity.
 * This uses the unbounded knapsack algorithm, 
 * meaning we're allowed to use unlimited number of instances of an item.
 * Complexity: O(n * capacity).
 */
template <std::ranges::random_access_range TCont>
    requires std::signed_integral<std::ranges::range_value_t<TCont>>
typename TCont::value_type knapsackUnbounded(typename TCont::value_type capacity, 
                                             const TCont& weights, 
                                             const TCont& values) {
    std::size_t sz = weights.size();
    if (sz != values.size()) throw std::invalid_argument("Weights and values have different sizes");
    std::vector<typename TCont::value_type> dp(capacity + 1, 0);
    // Iterate over each item
    for (std::size_t i = 1; i <= sz; ++i) {
        // Notice the order of traversal is opposite to the 0-1 knapsack
        for (std::size_t j = 1; j <= capacity; ++j) {
            // Compared to the 0-1 knapsack, if we pick an item we stay on its same row
            if (weights[i - 1] <= j) dp[j] = std::max(dp[j], dp[j - weights[i - 1]] + values[i - 1]);
        }
    }
    return dp[capacity];
}

} // namespace alg
