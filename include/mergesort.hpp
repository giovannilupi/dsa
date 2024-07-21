#pragma once

#include "alg_concepts.hpp"
#include "insertionsort.hpp"
#include <concepts>
#include <vector>

namespace alg {

namespace detail {

/**
 * Merge function for divide and conquer implementation of merge sort.
 * Merges two sorted subarrays into a single sorted array.
 */
template <SortableContainer T, std::integral Q>
void merge(T& container, Q low, Q mid, Q high) {
    Q i = low, j = mid+1;
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
    for (Q idx = low; idx <= high; ++idx) container[idx] = tmp[idx-low]; 
}

/**
 * Helper function for divide and conquer implementation of merge sort.
 */
template <SortableContainer T, std::integral Q>
void mergeSortHelper(T& container, Q low, Q high) {
    if (low < high) {
        Q mid = low + (high - low) / 2;
        mergeSortHelper(container, low, mid);
        mergeSortHelper(container, mid+1, high);
        merge(container, low, mid, high);
    }
}

/**
 * Helper function for coarse merge sort.
 * We sort each subarray directly when its size is <= 10.
 */
template <SortableContainer T, std::integral Q>
void mergeSortCoarseHelper(T& container, Q low, Q high) {
    static const Q k = 10;
    // The problem is simple enough to solve directly
    if ((high - low) < k) insertionSort(container, low, high);
    // Otherwise, divide and conquer
    else {
        Q mid = low + (high - low) / 2;
        mergeSortHelper(container, low, mid);
        mergeSortHelper(container, mid+1, high);
        merge(container, low, mid, high);
    }
}

}

/**
 * Generic implementation of the classic merge sort algorithm.
 * Recurrence relation: T(n) = 2T(n/2) + \Theta(n)
 * Complexity: O(n log n)
 */
template <SortableContainer T>
void mergeSort(T& container) {
    std::integral auto sz = container.size();
    if (!sz) return;
    decltype(sz) high = container.size() - 1;
    decltype(sz) low = 0;
    detail::mergeSortHelper(container, low, high);
}

/**
 * Merge sort implementation using coarsening to limit the depth of recursion.
 * We solve a subproblem directly when its size is <= k.
 */
template <SortableContainer T>
void mergeSortCoarse(T& container) {
    std::integral auto sz = container.size();
    if (!sz) return;
    decltype(sz) high = container.size() - 1;
    decltype(sz) low = 0;
    detail::mergeSortCoarseHelper(container, low, high);
}

}
