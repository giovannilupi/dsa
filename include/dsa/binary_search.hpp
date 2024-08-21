#pragma once

#include <concepts>
#include <iterator>
#include "alg_concepts.hpp"
#include "common.hpp"

namespace alg {

namespace detail {

template <std::ranges::random_access_range T>
    requires std::totally_ordered<typename T::value_type>
index bSearchRecHelp(const T& container, const typename T::value_type& val, index low, index high) {
    if (low > high) return -1;
    index mid = low + (high - low) / 2;
    if (container[mid] == val) return mid;
    if (container[mid] > val) return bSearchRecHelp(container, val, low, mid-1);
    else return bSearchRecHelp(container, val, mid+1, high);
}

} // namespace detail

/**
* Iterative binary search algorithm.
*/ 
template <std::ranges::random_access_range T>
    requires std::totally_ordered<typename T::value_type>
index bsearch(const T& container, const typename T::value_type& val) {
    index low = 0, high = container.size() - 1;
    while (low <= high) {
        index mid = low + (high - low) / 2;
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
template <std::ranges::random_access_range T>
    requires std::totally_ordered<typename T::value_type>
index bSearchRec(const T& container, const typename T::value_type& val) {
    index low = 0, high = container.size() - 1;
    return detail::bSearchRecHelp(container, val, low, high); 
}

/**
 * Binary search on a matrix.
 * The matrix must be sorted in row-major order.
 * This means the flattened matrix is a sorted array.
 * Returns the row and column of the searched value.
 */
template <Matrix T>
    requires SortableContainer<typename T::value_type>
std::pair<index, index> bsearchMatrix(const T& matrix, const typename T::value_type::value_type& val) {
    index rows = matrix.size();
    if (!rows) return {-1, -1};
    index cols = matrix[0].size();
    index low = 0;
    index high = rows * cols - 1;
    while (low <= high) {
        index mid = low + (high - low) / 2;
        index row = mid / cols;
        index col = mid % cols;
        if (val < matrix[row][col]) high = mid - 1;
        else if (matrix[row][col] < val) low = mid + 1;
        else return {row, col};
    }
    return {-1, -1};
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
 * Using C++ STL and iterators in input.
 * The iterators must reference a type that can be compared using the < operator.
 */ 
template <std::forward_iterator T>
    requires std::indirect_strict_weak_order<std::ranges::less, T>
T bSearchLibIt (T first, T last, const typename T::value_type& val) {
    T it = lower_bound(first, last, val);
    return (it != last && !(val < *it)) ? it : last;
}

} // namespace alg
