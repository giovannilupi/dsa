#pragma once

#include <concepts>
#include <vector>
#include "alg_concepts.hpp"

namespace alg {

namespace detail {

/**
 * Helper function for the power set DFS algorithm.
 * For each element we can decide to include or exclude it.
 * This way a binary tree is created, and the leaves are the elements in the power set.
 */
template<std::ranges::random_access_range T>
void powerSetDFS(const T& container, std::vector<std::vector<typename T::value_type>>& res, std::vector<typename T::value_type>& curr, std::size_t index) {
    // No more elements to add
    if (index == container.size()) {
        res.push_back(curr);
        return;
    }
    // Include the current element
    curr.push_back(container[index]);
    powerSetDFS(container, res, curr, index + 1);
    // Exclude the current element
    curr.pop_back();
    powerSetDFS(container, res, curr, index + 1);
}

/**
 * Helper function for the power set backtracking algorithm.
 * This uses a standard backtracking technique.
 * In this case all solutions are valid, so no pruning occurs.
 * Hence, the approach is just a structured way to perform an exhaustive search.
 */
template<std::ranges::random_access_range T>
void powerSetBacktrack(const T& container, std::vector<std::vector<typename T::value_type>>& res, std::vector<typename T::value_type>& curr, std::size_t index) {
    while (index != container.size()) {
        curr.push_back(container[index]);
        res.push_back(curr);
        powerSetBacktrack(container, res, curr, index + 1);
        curr.pop_back();
        ++index;
    }
}

} // namespace detail

/**
 * Returns the power set of a given set of elements.
 * This uses a recursive depth-first approach.
 * The input container must be a valid set, i.e. it must contain unique elements.
 * If the container contains duplicates, the result is undefined.
 * Complexity: O(2^n)
 */
template<std::ranges::random_access_range T>
std::vector<std::vector<typename T::value_type>> powerSetDFS(const T& container) {
    std::vector<std::vector<typename T::value_type>> res;
    std::vector<typename T::value_type> curr;
    detail::powerSetDFS(container, res, curr, 0);
    return res;
}

/**
 * Returns the power set of a given set of elements.
 * This uses a backtracking approach.
 * The input container must be a valid set, i.e. it must contain unique elements.
 * If the container contains duplicates, the result is undefined.
 * Complexity: O(2^n)
 */
template<std::ranges::random_access_range T>
std::vector<std::vector<typename T::value_type>> powerSetBacktrack(const T& container) {
    std::vector<std::vector<typename T::value_type>> res = {{}};
    std::vector<typename T::value_type> curr;
    detail::powerSetBacktrack(container, res, curr, 0);
    return res;
}

/**
 * Returns the power set of a given set of elements.
 * This uses an iterative approach.
 * The input container must be a valid set, i.e. it must contain unique elements.
 * If the container contains duplicates, the result is undefined.
 * Complexity: O(2^n)
 */
template <std::ranges::forward_range T>
std::vector<std::vector<typename T::value_type>> powerSetIter(const T& container) {
    std::vector<std::vector<typename T::value_type>> res = {{}};
    for (const auto& el : container) {
        const std::size_t sz = res.size();
        for (std::size_t i = 0; i != sz; ++i) {
            res.push_back(res[i]);
            res.back().push_back(el);
        }
    }
    return res;
}

} // namespace alg