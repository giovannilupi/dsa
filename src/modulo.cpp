#include <concepts>

using namespace std;

/**
 * Computes a mod m.
 * In number theory, the result is guaranteed to be in the renge [0, m).
 * In C++, when using % between operands of different type, 
 * the sign of the result is implementation-defined.
 * To enforce the mathematical definition, we can force the result to be positive. 
 */
template <integral T>
T mod(T a, T m) {
    T r = a % m;
    return (r < 0) ? (r + m) : r;
}
