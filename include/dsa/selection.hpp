#pragma once

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <concepts>
#include <functional>
#include "alg_concepts.hpp"
#include "quicksort.hpp"
#include "common.hpp"

namespace alg {

namespace detail {

/**
 * Returns the extreme value in the container according to the given comparator.
 * Throws an exception if the container is empty.
 * The problem has an inherent lower bound of n-1 comparisons.
 * Complexity: O(n)
 */
template <typename TCont, typename Cmp>
    requires SortableContainer<TCont, Cmp>
typename TCont::value_type extremeValue(const TCont& container, Cmp cmp) {
    if (container.empty()) throw std::invalid_argument("Container is empty");
    typename TCont::value_type res = *container.begin();
    for (const auto& el : container) if (cmp(el, res)) res = el;
    return res;
}

/**
 * Returns the nth extreme value in the container according to the given comparator.
 * Throws an exception if the container is empty or k is out of range.
 * This sorts the container and returns the element at index k.
 * Complexity: O(n log n)
 */
template <typename TCont, typename Cmp>
    requires SortableContainer<TCont, Cmp>
typename TCont::value_type selectNthExtreme(TCont& container, std::size_t k, Cmp cmp) {
    if (container.empty()) throw std::invalid_argument("Container is empty");
    if (k >= container.size()) throw std::out_of_range("Index out of range");
    sort(container.begin(), container.end(), cmp);
    return *std::next(container.begin(), k);
}

/**
 * Randomized quick sort partition function using the STL partition function.
 */
template <typename TCont, typename Cmp>
    requires SortableContainer<TCont, Cmp>
index partition(TCont& container, index low, index high, Cmp cmp) {
    // Pick a random element as pivot using a uniform distribution
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<index> dist(low, high);
    index pivot_idx = dist(gen);
    // Swap the chosen element with the last element in the container
    std::swap(container[pivot_idx], container[high]);    
    // Partition the container around the pivot
    auto it = std::partition(std::next(container.begin(), low), 
                               std::next(container.begin(), high),
                               [&](const auto& el) { return cmp(el, container[high]); });
    // Move the pivot to its final position
    pivot_idx = std::distance(container.begin(), it);
    std::swap(container[pivot_idx], container[high]);
    return pivot_idx;
}

/**
 * Returns the nth extreme value in the container using the quick select algorithm.
 * This uses an iterative version of the quick sort scheme.
 * Average complexity: O(n)
 * Worst complexity: O(n^2)
 */
template <typename TCont, typename Cmp>
    requires SortableContainer<TCont, Cmp>
typename TCont::value_type quickSelectHelper(TCont& container, std::size_t k, Cmp cmp) {
    if (container.empty()) throw std::invalid_argument("Container is empty");
    if (k >= container.size()) throw std::out_of_range("Index out of range");
    index low = 0, high = container.size() - 1;
    while (low < high) {
        index pivot_idx = detail::partition(container, low, high, cmp);
        if (pivot_idx == k) return container[pivot_idx];
        // Discard elements that are on the wrong side of the pivot
        else if (k < pivot_idx) high = pivot_idx - 1;
        else low = pivot_idx + 1;
    }
    return container[low];
}

template <typename TCont, typename Cmp>
    requires SortableContainer<TCont, Cmp>
typename TCont::value_type heapSelectHelper(TCont& container, std::size_t k, Cmp cmp) {
    if (container.empty()) throw std::invalid_argument("Container is empty");
    if (k >= container.size()) throw std::out_of_range("Index out of range");
    std::make_heap(container.begin(), container.end(), cmp);
    auto it = container.rbegin();
    auto ite = std::next(container.rbegin(), k+1);
    while (it != ite) {
        std::pop_heap(container.begin(), it.base(), cmp);
        ++it;
    }
    return *std::prev(it);
}

/**
 * Returns the nth extreme value in the container using the STL.
 * This runs in linear time.
 */
template <typename TCont, typename Cmp>
    requires SortableContainer<TCont, Cmp>
typename TCont::value_type selectModernHelper(TCont& container, std::size_t k, Cmp cmp) {
    if (container.empty()) throw std::invalid_argument("Container is empty");
    if (k >= container.size()) throw std::out_of_range("Index out of range");
    auto it = std::next(container.begin(), k);
    std::nth_element(container.begin(), it, container.end(), cmp);
    return *it;
}

} // namespace detail

/**
 * Returns the smallest element in the container.
 */
template <typename T>
    requires SortableContainer<T, std::less<typename T::value_type>>
typename T::value_type getMin(const T& container) {
    return detail::extremeValue(container, std::less<typename T::value_type>());
}

/**
 * Returns the largest element in the container.
 */
template <typename T>
    requires SortableContainer<T, std::greater<typename T::value_type>>
typename T::value_type getMax(const T& container) {
    return detail::extremeValue(container, std::greater<typename T::value_type>());
}

/**
 * Naive algorithm for extracting minimum and maximum elements from the container.
 */
template <typename T>
    requires SortableContainer<T, std::less<typename T::value_type>> && 
             SortableContainer<T, std::greater<typename T::value_type>>
std::pair<typename T::value_type, typename T::value_type> getMinMax(const T& container) {
    return {getMin(container), getMax(container)};
}

/**
 * Optimized algorithm for extracting minimum and maximum elements from the container.
 * This reduces the number of comparisons from 2n-2 to 3(n/2).
 */
template <typename T>
    requires SortableContainer<T, std::less<typename T::value_type>> && 
             SortableContainer<T, std::greater<typename T::value_type>>
std::pair<typename T::value_type, typename T::value_type> getMinMaxOpt(const T& container) {
    std::integral auto sz = container.size();
    if (!sz) throw std::invalid_argument("Container is empty");
    typename T::value_type minv, maxv;
    auto it = container.cbegin();
    if (sz & 1) maxv = minv = *it++;
    else { 
        std::tie(minv, maxv) = std::minmax(*it, *(std::next(it))); 
        std::advance(it, 2); 
    }
    while (it != container.end()) {
        auto [first, second] = std::minmax(*it, *(std::next(it)));
        minv = std::min(minv, first); 
        maxv = std::max(maxv, second);
        std::advance(it, 2);
    }
    return {minv, maxv};
}

/**
 * Returns the kth smallest element in the container using the naive algorithm.
 * The value of k starts from 0 (which corresponds to the smallest element).
 */
template <typename T>
    requires SortableContainer<T, std::less<typename T::value_type>>
typename T::value_type selectNthMin(T& container, std::size_t k) {
    return detail::selectNthExtreme(container, k, std::less<typename T::value_type>());
}

/**
 * Returns the kth largest element in the container using the naive algorithm.
 * The value of k starts from 0 (which corresponds to the largest element).
 */
template <typename T>
    requires SortableContainer<T, std::greater<typename T::value_type>>
typename T::value_type selectNthMax(T& container, std::size_t k) {
    return detail::selectNthExtreme(container, k, std::greater<typename T::value_type>());
}

/**
 * Returns the nth smallest element in the container using the quick select algorithm.
 * The value of k starts from 0 (which corresponds to the smallest element).
 */
template <typename T>
    requires SortableContainer<T, std::less<typename T::value_type>>
typename T::value_type quickSelectMin(T& container, std::size_t k) {
    return detail::quickSelectHelper(container, k, std::less<typename T::value_type>());
}

/**
 * Returns the nth largest element in the container using the quick select algorithm.
 * The value of k starts from 0 (which corresponds to the largest element).
 */
template <typename T>
    requires SortableContainer<T, std::greater<typename T::value_type>>
typename T::value_type quickSelectMax(T& container, std::size_t k) {
    return detail::quickSelectHelper(container, k, std::greater<typename T::value_type>());
}

/**
 * Returns the nth smallest element in the container using the heap select algorithm.
 * The value of k starts from 0 (which corresponds to the smallest element).
 */
template <typename T>
    requires SortableContainer<T, std::less<typename T::value_type>>
typename T::value_type heapSelectMin(T& container, std::size_t k) {
    return detail::heapSelectHelper(container, k, std::greater<typename T::value_type>());
}

/**
 * Returns the nth largest element in the container using the heap select algorithm.
 * The value of k starts from 0 (which corresponds to the largest element).
 */
template <typename T>
    requires SortableContainer<T, std::greater<typename T::value_type>>
typename T::value_type heapSelectMax(T& container, std::size_t k) {
    return detail::heapSelectHelper(container, k, std::less<typename T::value_type>());
}

/**
 * Returns the kth smallest element in the container using the STL library's nth_element function.
 * The value of k starts from 0 (which corresponds to the smallest element).
 */
template <typename T>
    requires SortableContainer<T, std::less<typename T::value_type>>
typename T::value_type selectMinModern(T& container, std::size_t k) {
    return detail::selectModernHelper(container, k, std::less<typename T::value_type>());
}

/**
 * Returns the kth smallest element in the container using the STL library's nth_element function.
 * The value of k starts from 0 (which corresponds to the largest element).
 */
template <typename T>
    requires SortableContainer<T, std::greater<typename T::value_type>>
typename T::value_type selectMaxModern(T& container, std::size_t k) {
    return detail::selectModernHelper(container, k, std::greater<typename T::value_type>());
}

/**
 * Returns the median of the container using the STL library's nth_element function.
 * Average complexity: O(n)
 */
template <std::ranges::range T>
    requires std::integral<std::ranges::range_value_t<T>>
double getMedian(T& container) {
    if (container.empty()) throw std::invalid_argument("Container is empty");
    auto sz = container.size();
    auto it1 = std::next(container.begin(), sz / 2);
    std::nth_element(container.begin(), it1, container.end());
    if (sz & 1) return *it1;
    // Even length
    auto it2 = std::next(container.begin(), sz / 2 - 1);
    std::nth_element(container.begin(), it2, it1);
    return static_cast<double>(*it1 + *it2) / 2.0;
}

} // namespace alg
