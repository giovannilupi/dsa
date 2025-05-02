#include "gtest/gtest.h"
#include <algorithm>
#include <functional>
#include <gtest/gtest.h>
#include <vector>
#include "heap.hpp"

namespace alg {
namespace {

using ::testing::Types;

const std::vector<std::vector<int>> testVectors = {
    {},
    {1},
    {7, 7, 7, 7, 7},
    {-5, -1, 0, 1, 5},
    {9, 7, 5, 3, 1},
    {1, 2, 3, 4, 5},
    {10, -20, 30, -40, 50},
};

template <typename HeapT>
struct HeapComparator;

template <>
struct HeapComparator<MaxHeap<int>> {
    using Comparator = std::less<int>;
};

template <>
struct HeapComparator<MinHeap<int>> {
    using Comparator = std::greater<int>;
};

/**
 * This is a complex test suite.
 * To test two objects with the same interface and requirments, we can use Typed Tests.
 * We'd like to test each object against multiple inputs, so it would be nice to use a Value-Parameterized Test.
 * However, gtest does not support a way to combine them.
 * https://stackoverflow.com/questions/8507385/google-test-is-there-a-way-to-combine-a-test-which-is-both-type-parameterized-a
 * The compromise is to set up a Typed test suite and use a loop in each test case to test all inputs.
 * Compared to a Value-Parameterized Test, the gtest output becomes less granular.
 */
template <typename HeapT>
class HeapTest : public testing::Test {
protected:
    // Verifies that the given heap respects the heap property
    bool isHeap(const HeapT& heap) const {
        const auto& vec = heap.toVector();
        return std::ranges::is_heap(vec, cmp);
    }
    typename HeapComparator<HeapT>::Comparator cmp;
};

using HeapTypes = Types<MaxHeap<int>, MinHeap<int>>;
TYPED_TEST_SUITE(HeapTest, HeapTypes);

TYPED_TEST(HeapTest, ContainerConstructor) {
    for (const auto& testVector : testVectors) {
        TypeParam heap(testVector);
        EXPECT_TRUE(this->isHeap(heap));
    }
}

TYPED_TEST(HeapTest, IteratorConstructor) {
    for (const auto& testVector : testVectors) {
        TypeParam heap(testVector);
        EXPECT_TRUE(this->isHeap(heap));
    }
}

TYPED_TEST(HeapTest, InitializerConstructor) {
    const TypeParam emptyHeap(std::initializer_list<int>{});
    EXPECT_TRUE(this->isHeap(emptyHeap));

    const TypeParam singleHeap(std::initializer_list<int>{1});
    EXPECT_TRUE(this->isHeap(singleHeap));

    const TypeParam simpleHeap(std::initializer_list<int>{3, 5, 1, 10, 2, 7});
    EXPECT_TRUE(this->isHeap(simpleHeap));
}

TYPED_TEST(HeapTest, InsertElements) {
    static constexpr std::array inserts = {3, 5, 1, 10, 2, 7};
    for (const auto& testVector : testVectors) {
        TypeParam heap(testVector);
        for (const auto& insert : inserts) {
            heap.insert(insert);
            EXPECT_TRUE(this->isHeap(heap));
        }
    }
}

TYPED_TEST(HeapTest, ExtractElements) {
    for (const auto& testVector : testVectors) {
        TypeParam heap(testVector);
        while (heap.size()) {
            auto expected = heap.peek();
            auto val = heap.extract();
            EXPECT_EQ(val, expected);
            EXPECT_TRUE(this->isHeap(heap));
        }
    }
}

}  // namespace
}  // namespace alg
