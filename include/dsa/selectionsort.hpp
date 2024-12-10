#pragma once

#include "alg_concepts.hpp"
#include "common.hpp"

namespace alg {

/**
 * Generic implementation of the classic selection sort algorithm.
 */ 
template <SortableContainer T>
void selectionSort(T& container) {
    const index sz = container.size();
    for (index i = 0; i < sz; ++i) {
        index minidx = i;
        for (index j = i+1; j < sz; ++j) {
            if (container[j] < container[minidx]) minidx = j;
        }
        std::swap(container[i], container[minidx]);
    }
}

} // namespace alg
