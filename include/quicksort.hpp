#pragma once

#include <random>
#include "alg_concepts.hpp"
#include "common.hpp"
#include "insertionsort.hpp"

namespace alg {

namespace detail {

/**
 * Partition function for quick sort.
 */
template <SortableContainer T>
index partition(T& container, index low, index high) {
    index j = low;
    typename T::value_type& pivot = container[high];
    while (low < high) {
        if (container[low] <= pivot) 
            std::swap(container[low], container[j++]);
        ++low;
    }
    std::swap(container[j], pivot);
    return j;
}

/**
 * Randomized partition function for quick sort.
 * This is a wrapper for the normal partition procedure.
 * It chooses a random pivot, and swaps it with the last element in the container.
 */
template <SortableContainer T>
index partitionRandom(T& container, index low, index high) {
    // Pick a random element as pivot using a uniform distribution
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<index> dist(low, high);
    index pivot_idx = dist(gen);
    // Swap the chosen element with the last element in the container
    std::swap(container[pivot_idx], container[high]);
    // Call the partition procedure with the pivot as the last element
    return partition(container, low, high);
}

/**
 * Helper function for quick sort.
 */
template <SortableContainer T>
void quickSortHelper(T& container, index low, index high) {
    if (low < high) {
        index pos = partition(container, low, high);
        quickSortHelper(container, low, pos-1);
        quickSortHelper(container, pos+1, high);
    }
}

/**
 * Helper function for randomized quick sort.
 */
template <SortableContainer T>
void quickSortRandomHelper(T& container, index low, index high) {
    if (low < high) {
        index pos = partitionRandom(container, low, high);
        quickSortRandomHelper(container, low, pos-1);
        quickSortRandomHelper(container, pos+1, high);
    }
}

/**
 * Helper function for coarse quick sort.
 * We sort each subarray directly when its size is <= 10.
 */
template <SortableContainer T>
void quickSortCoarseHelper(T& container, index low, index high) {
    static const int k = 10;
    if (low < high) {
        if (high - low < k) insertionSortIdx(container, low, high);
        else {
            index pos = partitionRandom(container, low, high);
            quickSortCoarseHelper(container, low, pos-1);
            quickSortCoarseHelper(container, pos+1, high);
        }
    }
}

}

/**
 * Generic implementation of the classic quick sort algorithm.
 * Worst case complexity: \Theta(n^2)
 * This occurs when the array is already sorted (in ascending or descending order).
 */
template <SortableContainer T>
void quickSort(T& container) {
    index high = container.size() - 1;
    index low = 0;
    detail::quickSortHelper(container, low, high);
}

/**
 * Randomized implementation of quick sort algorithm.
 * The pivot is chosen uniformly at random.
 * This reduces the likelihood of having an unbalanced recursion tree.
 */
template <SortableContainer T>
void quickSortRandom(T& container) {
    index high = container.size() - 1;
    index low = 0;
    detail::quickSortRandomHelper(container, low, high);
}

/**
 * Quick sort implementation using coarsening to limit the depth of recursion.
 * We solve a subproblem directly when its size is <= k.
 */
template <SortableContainer T>
void quickSortCoarse(T& container) {
    index high = container.size() - 1;
    index low = 0;
    detail::quickSortCoarseHelper(container, low, high);
}

}
