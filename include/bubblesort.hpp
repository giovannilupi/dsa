#pragma once

#include "alg_concepts.hpp"

namespace alg {

/**
 * Bubble sort with small optimizations.
 */
template <SortableContainer T>
void bubbleSort(T& container) {
    using SzType = decltype(container.size());
    SzType sz = container.size();
    // This check is needed because the loop index is unsigned
    if (!sz) return;
    // Loop at most n-1 times
    for (SzType i = sz-1; i; --i) {
        bool swapped = false;
        for (SzType j = 0; j < i; ++j) {
            if (container[j] > container[j+1]) { 
                std::swap(container[j], container[j+1]); 
                swapped = true;
            }
        }
        // If no swap is performed, we can break from the procedure
        if (!swapped) break;
    }
}

}
