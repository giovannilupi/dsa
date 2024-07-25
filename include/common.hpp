#pragma once

#include <cstddef>

namespace alg {
    /**
     * Index type for subscripting containers.
     * As recommended by the C++ core guidelines, to avoid issues with 
     * signed/unsigned mixtures, it is better to use signed integers of sufficient size.
     * The current implementation of the index type from the guidelines is a simple type alias.
     */
    using index = std::ptrdiff_t;
} // namespace alg
