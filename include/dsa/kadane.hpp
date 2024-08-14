#pragma once

#include <concepts>
#include <algorithm>
#include <climits>
#include <iostream>
#include <limits>
#include <stdexcept>
#include "common.hpp"

namespace alg {

namespace detail {

template <std::ranges::range T, typename Cmp>
    requires std::predicate<Cmp, typename T::value_type, typename T::value_type>
typename T::value_type getExtremeSubArray(const T& container, Cmp cmp, const typename T::value_type& limit) {
    if (container.empty()) throw std::invalid_argument("Container is empty");
    typename T::value_type res = limit;
    typename T::value_type sum = 0;
    for (const auto& el : container) {
        sum += el;
        res = std::max(sum, res, cmp);
        if (cmp(sum, 0)) sum = 0;
    }
    return res;
}

template <std::ranges::random_access_range T, typename Cmp>
    requires std::predicate<Cmp, typename T::value_type, typename T::value_type>
std::pair<index, index> getExtremeSubArrayIdx(const T& container, Cmp cmp, const typename T::value_type& limit) {
    if (container.empty()) throw std::invalid_argument("Container is empty");
    index maxL = 0, maxR = 0, currL = 0;
    typename T::value_type currMax = limit;
    typename T::value_type currSum = 0;
    std::unsigned_integral auto sz = container.size();
    for (index currR = 0; currR != sz; ++currR) {
        if (cmp(currSum, 0)) {
            currSum = 0;
            currL = currR;
        }
        currSum += container[currR];
        if (cmp(currMax, currSum)) {
            currMax = currSum;
            maxL = currL;
            maxR = currR;
        }
    }
    return {maxL, maxR};
}

}

/**
 * Returns the mainimum sum in a contiguous subarray in the given container.
 */
template <std::ranges::range T>
    requires std::signed_integral<std::ranges::range_value_t<T>>
typename T::value_type getMinSubArray(const T& container) {
    typename T::value_type limit = std::numeric_limits<typename T::value_type>::max();
    return detail::getExtremeSubArray(container, std::greater<typename T::value_type>(), limit);
}

/**
 * Returns the maximum sum in a contiguous subarray in the given container.
 */
template <std::ranges::range T>
    requires std::signed_integral<std::ranges::range_value_t<T>>
typename T::value_type getMaxSubArray(const T& container) {
    typename T::value_type limit = std::numeric_limits<typename T::value_type>::min();
    return detail::getExtremeSubArray(container, std::less<typename T::value_type>(), limit);
}

/**
 * Returns the indices delimiting the minimum subarray in the given container.
 * The indeces are inclusive.
 */
template <std::ranges::random_access_range T>
    requires std::signed_integral<std::ranges::range_value_t<T>>
std::pair<index, index> getMinSubArrayIdx(const T& container) {
    typename T::value_type limit = std::numeric_limits<typename T::value_type>::max();
    return detail::getExtremeSubArrayIdx(container, std::greater<typename T::value_type>(), limit);
}

/**
 * Returns the indices delimiting the maximum subarray in the given container.
 * The indeces are inclusive.
 */
template <std::ranges::random_access_range T>
    requires std::signed_integral<std::ranges::range_value_t<T>>
std::pair<index, index> getMaxSubArrayIdx(const T& container) {
    typename T::value_type limit = std::numeric_limits<typename T::value_type>::min();
    return detail::getExtremeSubArrayIdx(container, std::less<typename T::value_type>(), limit);
}

}
