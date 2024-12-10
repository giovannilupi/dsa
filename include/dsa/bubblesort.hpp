#pragma once

#include "alg_concepts.hpp"
#include "common.hpp"

namespace alg {

/**
 * Bubble sort with small optimizations.
 */
template <SortableContainer T>
void bubbleSort(T& container) {
    const index sz = container.size();
    // Loop at most n-1 times
    for (index i = sz-1; i; --i) {
        bool swapped = false;
        for (index j = 0; j < i; ++j) {
            if (container[j] > container[j+1]) { 
                std::swap(container[j], container[j+1]); 
                swapped = true;
            }
        }
        // If no swap is performed, we can break from the procedure
        if (!swapped) break;
    }
}

} // namespace alg
