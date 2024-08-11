#pragma once

#include <__concepts/relation.h>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <vector>
#include <iostream>

#include "alg_concepts.hpp"

namespace alg {

namespace detail {

template <typename T, typename Cmp>
class Heap {
public:

    /**
     * Constructs an empty heap.
     * It can then be filled using insert.
     * Constructing a heap this way has the same complexity as creating the heap from a container.
     */
    Heap() = default;

    /**
     * Constructs a heap from a container.
     * Complexity: O(n)
     */
    template <std::ranges::random_access_range Container>
    Heap(const Container& container) : vec(container.begin(), container.end()) {
        buildHeap();
    }

    /**
     * Constructs a heap from an iterator range.
     * Complexity: O(n)
     */
    template <std::random_access_iterator Iter>
    Heap(Iter first, Iter last) : vec(first, last) {
        buildHeap();
    }

    /**
     * Constructs a heap from an initializer list.
     * Complexity: O(n)
     */
    Heap(std::initializer_list<T> list) : vec(list) {
        buildHeap();
    }

    /**
     * Inserts an element into the heap.
     * Complexity: O(log n)
     */
    void insert(const T& value) { 
        vec.push_back(value);
        heapifyUpIter(vec.size() - 1);
    }

    /**
     * Removes the root of the heap and returns it.
     * The behavior is undefined if the heap is empty.
     * Complexity: O(log n)
     */
    T extract() {
        T result = peek();
        remove();
        return result;
    }

    /**
     * Removes the root of the heap.
     * The behavior is undefined if the heap is empty.
     * Complexity: O(log n)
     */
    void remove() { 
        vec.front() = vec.back();
        vec.pop_back();
        heapifyDownIter(0);
     }

    /**
     * Returns the root of the heap.
     * The behavior is undefined if the heap is empty.
     */
    const T& peek() const { return vec.front(); } 

    /**
     * Returns true if the heap is empty.
     */
    bool empty() const { return vec.empty(); }

    /**
     * Returns the size of the heap.
     */
    size_t size() const { return vec.size(); }

    /**
     * Returns the vector representation of the heap.
     */
    const std::vector<T>& toVector() const { return vec; }

    /**
     * Prints a tree representation of the heap.
     * The representation is far from perfect, and is not intended to be.
     * It is intended to be used for debugging purposes.
     * The class template does not check for the availability of this operation.
     * This is intended, as it is not a proopery required to construct a heap.
     * Concepts are not meant to be used for merely checking syntactic constraints.
     */
    friend std::ostream& operator<<(std::ostream& os, const Heap& heap) {
        if (heap.vec.empty()) return os;

        int height = std::log2(heap.vec.size()) + 1;
        int index = 0;

        for (int i = 0; i < height; ++i) {
            int levelSize = std::pow(2, i);
            int spacing = std::pow(2, height - i) - 1;

            // Print nodes at the current level
            for (int j = 0; j < levelSize && index < heap.vec.size(); ++j) {
                // Print leading spaces
                for (int k = 0; k < spacing; ++k) {
                    os << " ";
                }
                // Print the node value
                os << heap.vec[index];
                ++index;

                // Print trailing spaces
                for (int k = 0; k < spacing; ++k) {
                    os << " ";
                }
            }
            os << std::endl;
        }
        return os;
    }

private:
    /** The heap is implemented as vector, operations are performed with arithmetics on indices. */
    std::vector<T> vec;
    /** Comparison function for the heap type. */
    Cmp cmp = Cmp();

    /**
     * Build-heap procedure to convert a container into a max-heap.
     * Complexity: O(n)
     */
    void buildHeap() {
        for (int i = (vec.size() / 2) - 1; i >= 0; --i) heapifyDownIter(i);
    }

    /**
     * Recursive implementation of the heapify down procedure.
     * This floats elements down the heap to maintain the heap property.
     * Complexity: O(log n)
     */
    void heapifyDownRec(int idx) {
        int lt = left(idx);
        int rt = right(idx);
        int sz = size();
        int largest = idx;
        // Find the largest element between current node and its children
        if (lt < sz && cmp(vec[lt], vec[largest])) largest = lt;
        if (rt < sz && cmp(vec[rt], vec[largest])) largest = rt;
        // If largest is not the current node, swap and recurse
        if (largest != idx) {
            std::swap(vec[idx], vec[largest]);
            heapifyDownRec(largest);
        }
    }

    /**
     * Iterative implementation of the heapify down procedure.
     * This floats elements down the heap to maintain the heap property.
     */
    void heapifyDownIter(int idx) {
        int sz = vec.size();
        while (true) {
            int largest = idx;
            int lt = left(largest);
            int rt = right(largest);
            // Find the largest element between current node and its children
            if (lt < sz && cmp(vec[lt], vec[largest])) largest = lt;
            if (rt < sz && cmp(vec[rt], vec[largest])) largest = rt;
            // If the current node is the largest, the tree is a max-heap
            if (largest == idx) break;
            // Otherwise, swap and continue
            std::swap(vec[idx], vec[largest]);
            idx = largest;
        }
    }

    /**
     * Iterative implementation of the heapify up procedure.
     * This floats elements up the heap to maintain the heap property.
     */
    void heapifyUpIter(int idx) {
        while (idx > 0) {
            int p = parent(idx);
            if (cmp(vec[idx], vec[p])) { 
                std::swap(vec[idx], vec[p]); 
                idx = p; 
            }
            else break;
        }
    }

    /**
     * Recursive implementation of the heapify up procedure.
     * This floats elements up the heap to maintain the heap property.
     */
    void heapifyUpRec(int idx) {
        if (idx > 0) {
            int p = parent(idx);
            if (cmp(vec[idx], vec[p])) {
                std::swap(vec[idx], vec[p]);
                heapifyUpRec(p);
            }
        }
    }

    /**
     * Returns the index of the parent node of the node at the given index.
     */
    inline int parent(int idx) const { return (idx - 1) >> 1; }
    /**
     * Returns the index of the left child of the node at the given index.
     */
    inline int left(int idx) const { return (idx << 1) + 1; }
    /**
     * Returns the index of the right child of the node at the given index.
     */
    inline int right(int idx) const { return (idx << 1) + 2; }
};

} // namespace detail

/**
 * Comparison function for max heap.
 */
template <typename T>
using MaxHeapCmp = std::greater<T>;

/**
 * Comparison function for min heap.
 */
template <typename T>
using MinHeapCmp = std::less<T>;

/**
 * Implementation of a max-heap.
 * Max-heap property: A[i] >= A[left(i)] && A[i] >= A[right(i)]
 */
template <typename T>
    requires std::strict_weak_order<MaxHeapCmp<T>, T, T>
using MaxHeap = detail::Heap<T, MaxHeapCmp<T>>;

/**
 * Implementation of a min-heap.
 * Min-heap property: A[i] <= A[left(i)] && A[i] <= A[right(i)]
 */
template <typename T>
    requires std::strict_weak_order<MinHeapCmp<T>, T, T>
using MinHeap = detail::Heap<T, MinHeapCmp<T>>;

} // namespace alg
