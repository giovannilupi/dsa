#pragma once

#include "alg_concepts.hpp"
#include <concepts>
#include <iterator>

namespace alg {

namespace detail {

template <SortableContainer T, std::integral Q>
int bSearchRecHelp(const T& container, const typename T::value_type& val, Q low, Q high) {
    if (low > high) return -1;
    Q mid = low + (high - low) / 2;
    if (container[mid] == val) return mid;
    if (container[mid] > val) return bSearchRecHelp(container, val, low, mid-1);
    else return bSearchRecHelp(container, val, mid+1, high);
}

}

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
template <SortableContainer T>
int bSearchRec(const T& container, const typename T::value_type& val) {
    int low = 0, high = container.size() - 1;
    return detail::bSearchRecHelp(container, val, low, high); 
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
template <std::forward_iterator T>
T bSearchLibIt (T first, T last, const typename T::value_type& val) {
    T it = lower_bound(first, last, val);
    return (it != last && !(val < *it)) ? it : last;
}

}
