#pragma once

#include <__ranges/concepts.h>
#include <iterator>
#include <iostream>
#include <ranges>

namespace alg {

/**
 * Sortable container type.
 * A container is sortable if it's a sequence with a sortable iterator type.
 * This definition follows the style of other std lib concepts (e.g. ranges::input_range).
 */ 
template <typename T, typename Cmp = std::ranges::less>
concept SortableContainer =
    // Type must be a range
    std::ranges::range<T> &&
    // Cmp must be a valid comparator for the value type
    std::predicate<Cmp, std::ranges::range_value_t<T>, std::ranges::range_value_t<T>> &&
    // The iterator type of the range must be sortable
    std::sortable<std::ranges::iterator_t<T>, Cmp>;

/**
 * Printable element type.
 * A type that can be printed to stdout using the << operator.
 */ 
template <typename T>
concept PrintableElement = requires(T a) {
    { std::cout << a } -> std::same_as<std::ostream&>;
};

/**
 * Printable container type.
 * A container is printable if it's a sequence whose value type is printable.
 */ 
template <typename T>
concept PrintableContainer =
    // Must be a range
    std::ranges::range<T> &&
    // The range must contain printable elements
    PrintableElement<std::ranges::range_value_t<T>>;

/**
 * A container that can be palindromic.
 */
template <typename T>
concept PalindromicContainer = 
    // Must provide random access to elements
    std::ranges::random_access_range<T> && 
    // Must provide constant time size operation
    std::ranges::sized_range<T> && 
    // The value type must be equality comparable
    std::equality_comparable<std::ranges::range_value_t<T>>;

/**
 * A matrix type.
 */
template <typename T>
concept Matrix =
    // Must provide random access to elements
    std::ranges::random_access_range<T> && 
    // Must provide constant time size operation
    std::ranges::sized_range<T> &&
    // Must contain a random access container
    std::ranges::random_access_range<std::ranges::range_value_t<T>> &&
    // Must contain a container with constant time size operation
    std::ranges::sized_range<std::ranges::range_value_t<T>>;

} // namespace alg
