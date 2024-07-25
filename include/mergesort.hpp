#pragma once

#include <concepts>
#include <vector>
#include "alg_concepts.hpp"
#include "insertionsort.hpp"
#include "common.hpp"

namespace alg {

namespace detail {

/**
 * Merge function for divide and conquer implementation of merge sort.
 * Merges two sorted subarrays into a single sorted array.
 */
template <SortableContainer T>
void merge(T& container, index low, index mid, index high) {
    index i = low, j = mid+1;
    // Temporary vector where to store the sorted unified container
    std::vector<typename T::value_type> tmp;
    while (i <= mid && j <= high) {
        if (container[j] < container[i]) tmp.push_back(container[j++]);
        else tmp.push_back(container[i++]);
    }
    // Add the remaining elements to the result (only one subarray can have elements left)
    while (i <= mid) tmp.push_back(container[i++]);
    while (j <= high) tmp.push_back(container[j++]);
    // Copy tmp to the original container
    for (index idx = low; idx <= high; ++idx) container[idx] = tmp[idx-low]; 
}

/**
 * Helper function for divide and conquer implementation of merge sort.
 */
template <SortableContainer T>
void mergeSortHelper(T& container, index low, index high) {
    if (low < high) {
        index mid = low + (high - low) / 2;
        mergeSortHelper(container, low, mid);
        mergeSortHelper(container, mid+1, high);
        merge(container, low, mid, high);
    }
}

/**
 * Helper function for coarse merge sort.
 * We sort each subarray directly when its size is <= 10.
 */
template <SortableContainer T>
void mergeSortCoarseHelper(T& container, index low, index high) {
    static const index k = 10;
    // The problem is simple enough to solve directly
    if ((high - low) < k) insertionSortIdx(container, low, high);
    // Otherwise, divide and conquer
    else {
        index mid = low + (high - low) / 2;
        mergeSortHelper(container, low, mid);
        mergeSortHelper(container, mid+1, high);
        merge(container, low, mid, high);
    }
}

} // namespace detail

/**
 * Generic implementation of the classic merge sort algorithm.
 * Recurrence relation: T(n) = 2T(n/2) + \Theta(n)
 * Complexity: O(n log n)
 */
template <SortableContainer T>
void mergeSort(T& container) {
    index high = container.size() - 1;
    index low = 0;
    detail::mergeSortHelper(container, low, high);
}

/**
 * Merge sort implementation using coarsening to limit the depth of recursion.
 * We solve a subproblem directly when its size is <= k.
 */
template <SortableContainer T>
void mergeSortCoarse(T& container) {
    index high = container.size() - 1;
    index low = 0;
    detail::mergeSortCoarseHelper(container, low, high);
}

} // namespace alg
