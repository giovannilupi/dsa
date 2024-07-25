#pragma once

#include "alg_concepts.hpp"
#include "heap.hpp"
#include <algorithm>
#include <functional>

namespace alg {

/**
 * Implementation of the classic heap sort algorithm.
 * This is a modern implementation using C++ 20 algorithm header features.
 * Complexity: O(n log n)
 * Memory usage: O(1)
 */
template <SortableContainer T>
void heapSort(T& container) {
    // Turn the input container into a max heap
    std::make_heap(container.begin(), container.end());
    // Progressively move the largest element to the end
    for (auto it = container.rbegin(); it != container.rend(); ++it) {
        // Moves the root of the heap to the back
        std::pop_heap(container.begin(), it.base());
    }
}

} // namespace alg
