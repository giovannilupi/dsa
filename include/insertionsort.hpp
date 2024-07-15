#ifndef INSERTIONSORT_H
#define INSERTIONSORT_H

#include "alg_concepts.hpp"
#include <concepts>

namespace alg {

/**
 * Generic implementation of the classic insertion sort algorithm.
 */
template <SortableContainer T>
void insertionSort(T& container) {
    for (int i = 1; i < container.size(); ++i) {
        // We go backwards looking for the insert position
        for (int j = i; j && container[j-1] > container[j]; --j) {
            std::swap(container[j-1], container[j]);
        }
    }
}

/**
 * Insertion sort with a custom low and high index.
 * The high index is inclusive.
 */
template <SortableContainer T, std::integral Q>
void insertionSort(T& container, Q low, Q high) {
    for (Q i = 1; i <= high; ++i) {
        for (Q j = i; j && container[j-1] > container[j]; --j) {
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

}

#endif // !INSERTIONSORT_H

