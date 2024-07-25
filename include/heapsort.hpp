#pragma once

#include "alg_concepts.hpp"
#include "heap.hpp"

namespace alg {

/**
 * Implementation of the classic heap sort algorithm.
 * Notice this does not sort the container in place,
 * as constructing the heap from the container copies the container.
 * To sort the container in place, one could change the implementation
 * of Heap to be a container adapter, or implement the heap functions
 * accepting a container as a parameter.
 * Complexity: O(n log n)
 */
template <SortableContainer T>
void heapSort(T& container) {
    MinHeap<typename T::value_type> heap(container);
    for (typename T::value_type &el : container) {
        el = heap.extract();
    }
}

} // namespace alg
