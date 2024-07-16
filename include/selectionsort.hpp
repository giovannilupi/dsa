#ifndef SELECTIONSORT_H
#define SELECTIONSORT_H

#include <climits>
#include "alg_concepts.hpp"

namespace alg {

/**
 * Generic implementation of the classic selection sort algorithm.
 */ 
template <SortableContainer T>
void selectionSort(T& container) {
    using SzType = decltype(container.size());
    SzType sz = container.size();
    for (SzType i = 0; i < sz; ++i) {
        SzType minidx = i;
        for (SzType j = i+1; j < sz; ++j) {
            if (container[j] < container[minidx]) minidx = j;
        }
        std::swap(container[i], container[minidx]);
    }
}

}

#endif // !SELECTIONSORT_H
