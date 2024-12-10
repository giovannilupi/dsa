#pragma once

#include <concepts>
#include "alg_concepts.hpp"
#include "common.hpp"

namespace alg {

/**
 * Generic implementation of the classic insertion sort algorithm.
 */
template <SortableContainer T>
void insertionSort(T& container) {
    const index sz = container.size();
    for (index i = 1; i < sz; ++i) {
        // We go backwards looking for the insert position
        for (index j = i; j && container[j-1] > container[j]; --j) {
            std::swap(container[j-1], container[j]);
        }
    }
}

/**
 * Insertion sort with a custom low and high index.
 * The high index is inclusive.
 */
template <SortableContainer T>
void insertionSortIdx(T& container, index low, index high) {
    for (index i = low + 1; i <= high; ++i) {
        for (index j = i; j > low && container[j-1] > container[j]; --j) {
            std::swap(container[j-1], container[j]);
        }
    }
}

/** 
 * Modern implementation of insertion sort using the STL.
 * Uses binary search to find the insert position and rotate to shift the elements. 
 */
template <SortableContainer T>
void insertionSortModern(T& container) {
    auto itb = container.begin(), ite = container.end();  
    for (auto it = itb; it != ite; ++it)
        rotate(upper_bound(itb, it, *it), it, next(it));
}

} // namespace alg
