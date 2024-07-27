# dsa: Data Structures and Algorithms in C++20

**dsa** is a library that reimplements classic data structures and algorithms using modern C++20 features. All implementations are provided in a header-only format within the dsa directory. The use of templates ensures that the algorithms are as general as possible, while the use of concepts adheres to the current C++ core guidelines. The combination of classic algorithms with concepts and templates is something rarely found. The hope is that this blend could offer a unique way to approach these problems compared to other online resources.

These implementations are primarily intended for educational purposes, although some can offer practical usage. For example, the binary search implementation provided here is more convenient than the one in the Standard Template Library (STL). Other implementations, such as those of data structures already present in the STL, are only meant to showcase their internal workings. Additionally, some algorithms are implemented in multiple styles (e.g. iterative vs recursive) to explore different problem-solving strategies.

A notable feature of this library is its use of C++20 concepts. While the standard has recently started promoting the use of concepts for all template arguments, practical examples are still scarce. This work features several usage examples of standard library concepts, along with new ones defined in the alg_concepts header. These custom concepts are primarily implemented by combining existing concepts from the C++ standard library using logical conditions. However, defining new concepts is far from trivial, so it is advisable to review them thoroughly before using them in serious applications.

The function interfaces in this library vary, with some using indices, others using iterators, and some operating on container references. his diversity reflects different design choices and can be easily adapted to other use cases. For those who do not require the flexibility of templates, the functions can be specialized with ease.

## Testing

All functions have been unit tested using the Google Test framework. However, I recommend double checking that the implementations are correct before using them in your programs.

## License

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.