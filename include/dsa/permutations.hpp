#pragma once

#include <concepts>
#include <vector>

namespace alg {

namespace detail {

/**
 * Helper function for the permutations algorithm.
 * It fixes the first element of the container and recursively calls itself.
 * In this case all solutions are valid, so no pruning occurs.
 * Hence, the approach is just a structured way to perform an exhaustive search.
 */
template<typename T>
void permutationsHelper(std::vector<T>& container, std::vector<std::vector<T>>& res, std::size_t idx) {
    if (idx == container.size()) {
        res.push_back(container);
        return;
    }
    for (std::size_t i = idx; i != container.size(); ++i) {
        // Fix one element by swapping it with the current index
        std::swap(container[idx], container[i]);
        // Call recursively on the rest of the elements
        permutationsHelper(container, res, idx + 1);
        // Undo the swap
        std::swap(container[idx], container[i]);
    }
}

} // namespace detail

/**
 * Returns all possible permutations of the elements in the given container.
 * This uses a backtracking approach.
 * Complexity: O(n!)
 */
template<std::ranges::range TCont>
std::vector<std::vector<typename TCont::value_type>> permutations(const TCont& container) {
    using T = typename TCont::value_type;
    std::vector<std::vector<T>> res;
    std::vector<T> vec(container.cbegin(), container.cend());
    detail::permutationsHelper(vec, res, 0);
    return res;
}

} // namespace alg
