#include "alg_concepts.hpp"
#include <algorithm>
#include <concepts>
#include <iterator>

using namespace std;

/**
* Iterative binary search algorithm.
*/ 
template <SortableContainer T>
int bsearch(const T& container, const typename T::value_type& val) {
    int low = 0, high = container.size() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (container[mid] > val) high = mid - 1;
        else if (container[mid] < val) low = mid + 1;
        else return mid;
    }
    return -1;
}

/**
* Tail recursive algorithm.
* Could alternatively be implemented using iterators instead of integral indexes.
*/ 
template <SortableContainer T, integral Q>
int bSearchRecHelp(const T& container, Q low, Q high, const typename T::value_type& val) {
    if (low > high) return -1;
    Q mid = low + (high - low) / 2;
    if (container[mid] == val) return mid;
    if (container[mid] > val) return bSearchRecHelp(container, low, mid-1, val);
    else return bSearchRecHelp(container, mid+1, high, val);
}
template <SortableContainer T>
int bSearchRec(const T& container, const typename T::value_type& val) {
    integral auto high = container.size() - 1;
    decltype(high) low = 0;
    return bSearchRecHelp(container, low, high, val); 
}

/**
 * Using C++ STL.
 * The closest thing in STL to binary search is lower_bound.
 * However, in case the searched item does not exist, lower_bound does not necessarily return the end iterator.
 */
template <SortableContainer T>
typename T::const_iterator bSearchLib(const T& container, const typename T::value_type& val) {
    using CIter = typename T::const_iterator;
    CIter itb = container.cbegin(), ite = container.cend();
    CIter itp = lower_bound(itb, ite, val);
    // Lower bound only requires the sequence to have a < operation defined
    // Since elements might not be equality comparable, it's better to avoid using *itp == val
    // We can do the same comparison using !(val < *itp)
    return (itp != ite && !(val < *itp)) ? itp : ite;
}


/**
 * Using C++ STL and iterators in input
 */ 
template <forward_iterator T>
T bSearchLibIt (T first, T last, const typename T::value_type& val) {
    T it = lower_bound(first, last, val);
    return (it != last && !(val < *it)) ? it : last;
}

