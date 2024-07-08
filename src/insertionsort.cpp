#include "alg_concepts.hpp"

using namespace std;

/**
 * Generic implementation of the classic insertion sort algorithm.
 */
template <SortableContainer T>
void insertionSort(T& container) {
    for (int i = 1; i < container.size(); ++i) {
        // We go backwards looking for the insert position
        for (int j = i; j && container[j-1] > container[j]; --j) {
            swap(container[j-1], container[j]);
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

