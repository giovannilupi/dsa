#include "alg_concepts.hpp"

using namespace std;

/**
 * Bubble sort with small optimizations.
 */
template <SortableContainer T>
void bubbleSort(T& container) {
    using SzType = decltype(container.size());
    SzType sz = container.size();
    // Loop at most n-1 times
    for (SzType i = sz-1; i; --i) {
        bool swapped = false;
        for (SzType j = 0; j < i; ++j) {
            if (container[j] > container[j+1]) { 
                swap(container[j], container[j+1]); 
                swapped = true;
            }
        }
        // If no swap is performed, we can break from the procedure
        if (!swapped) break;
    }
}
