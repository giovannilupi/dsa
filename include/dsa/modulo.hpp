#pragma once

#include <concepts>
#include <stdexcept>

namespace alg {

/**
 * Computes a mod m.
 * The result is guaranteed to be in the renge [0, m), as in number theory.
 * In C++, when using % between operands of different sign, 
 * the sign of the result is implementation-defined.
 * To enforce the mathematical definition, we force the result to be positive. 
 */
template <std::integral T>
T mod(T a, T m) {
    if (m < 1) throw std::invalid_argument("Divisor must be >= 1.");
    T r = a % m;
    return (r < 0) ? (r + m) : r;
}

} // namespace alg
