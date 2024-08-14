<div align="center">

# dsa 
Data Structures and Algorithms in C++20

![C++](https://img.shields.io/badge/-C++-blue?logo=cplusplus)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

</div>

**dsa** is a library that reimplements classic data structures and algorithms using modern C++20 features. All implementations are provided in a header-only format within the [dsa](./include/dsa/) directory. The use of templates ensures that the algorithms are as general as possible, while the use of concepts adheres to the current C++ core guidelines. The combination of classic algorithms with concepts and templates is something rarely found. The hope is that this blend could offer a unique way to approach these problems compared to other online resources.

## Features

The implementations hereby provided are primarily intended for educational purposes, although some can offer practical usage. For example, the binary search implementation provided here is more convenient than the one in the [Standard Template Library (STL)](https://en.cppreference.com/w/cpp/standard_library). Other implementations, such as those of data structures already present in the STL, are only meant to showcase their internal workings. Additionally, some algorithms are implemented in multiple styles (e.g. iterative vs recursive) to explore different problem-solving strategies.

A notable feature of this library is its use of C++20 concepts. While the standard has recently started promoting the use of concepts for all template arguments, practical examples are still scarce. This work features several usage examples of standard library concepts, along with new ones defined in the [alg_concepts](./include/dsa/alg_concepts.hpp) header. These custom concepts are primarily implemented by combining existing concepts from the C++ standard library using logical conditions. However, defining new concepts is far from trivial, so it is advisable to review them thoroughly before using them in serious applications.

The function interfaces in this library vary, with some using indices, others using iterators, and some operating on container references. his diversity reflects different design choices and can be easily adapted to other use cases. For those who do not require the flexibility of templates, the functions can be specialized with ease.

## Theory

The [notes](./notes) directory contains Markdown files summarizing the theory behind many of the data structures and algorithms found in this library. These notes are intended for personal convenience and are shared in the hope that they might be useful to others. They were originally written in [Joplin](https://github.com/laurent22/joplin) and exported in Markdown format.

Please note that GitHub currently has an [issue](https://github.com/github/markup/issues/1688) in rendering certain mathematical formulas.

For a complete reference, including formal mathematical demonstrations and complexity analysis, I recommend the following books:

- Introduction to Algorithms (4th Edition) by Cormen et al.
- The Algorithm Design Manual (3rd Edition) by Skiena

Complete beginners in theoretical comptuter science and programming might benefit from starting with a more intuitive book. In this case, I recommend Grokking Algorithms by Bhargava.

## Testing

All functions in the library have been unit tested using the [Google Test](https://github.com/google/googletest) framework. These test suites ensure that the library functions as intended and demonstrate advanced testing features such as:
- Fixtures
- Parameterized Tests
- Type-Parameterized Tests

The tests are designed to be comprehensive and to enhance the readability of the output. For instance, value-parameterized tests can handle multiple inputs and provide more granular results compared to traditional loops.

All tests are located in the [tests](./tests) directory.

## Getting Started

### Pre-requisites

- CMake (Version 3.10 or higher)
- Any C++ compiler with support for C++20 standard
- Git

### Clone the repository

```
git clone https://github.com/giovannilupi/dsa.git
cd dsa
```

### Build the project

1. Create a build directory
```
mkdir build
cd build
```

2. Configure the project
```
cmake ..
```

3. Build the project
```
make
```

### Run the tests

After building the project:

```
./runTests
```

## License

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.