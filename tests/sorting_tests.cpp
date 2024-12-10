#include <gtest/gtest.h>
#include <algorithm>
#include <string>
#include <format>
#include <vector>
#include <functional> 
#include "insertionsort.hpp"
#include "selectionsort.hpp"
#include "bubblesort.hpp"
#include "mergesort.hpp"
#include "heapsort.hpp"
#include "quicksort.hpp"

using ::testing::TestWithParam;
using ::testing::Combine;
using ::testing::ValuesIn;

namespace {

using SortFunc = std::function<void(std::vector<int>&)>;

const std::map<std::string, SortFunc> sortingFunctions = {
    {"InsertionSort", alg::insertionSort<std::vector<int>>},
    {"InsertionSortModern", alg::insertionSortModern<std::vector<int>>},
    {"SelectionSort", alg::selectionSort<std::vector<int>>},
    {"BubbleSort", alg::bubbleSort<std::vector<int>>},
    {"MergeSort", alg::mergeSort<std::vector<int>>},
    {"MergeSortCoarse", alg::mergeSortCoarse<std::vector<int>>},
    {"HeapSort", alg::heapSort<std::vector<int>>},
    {"QuickSort", alg::quickSort<std::vector<int>>},
    {"QuickSortRandom", alg::quickSortRandom<std::vector<int>>},
    {"QuickSortCoarse", alg::quickSortCoarse<std::vector<int>>},
};

const std::map<std::string, std::vector<int>> testVectors = {
    {"Sorted", {1, 2, 3, 4, 5}},
    {"Unsorted", {5, 3, 1, 4, 2}},
    {"Empty", {}},
    {"SingleElement", {5}},
    {"ReverseOrder", {9, 8, 7, 6, 5, 4, 3, 2, 1}},
    {"NegativeNumbers", {-5, -4, -3, -2, -1}},
    {"Duplicates", {4, 2, 5, 5, 3, 2, 1}},
    {"LargeValues", {2147483647, 2147483646, 2147483645, 2147483644, 2147483643, 2147483642, 2147483641}},
};

} // namespace

using SortingTestParamT = std::tuple<decltype(sortingFunctions)::value_type, decltype(testVectors)::value_type>;

class SortingTest : public TestWithParam<SortingTestParamT> {};

TEST_P(SortingTest, WorksWithAllInputs) {
    // Get the parameters for the current test case
    const auto& param = GetParam();
    const auto& sortFunc = std::get<0>(param).second;
    auto container = std::get<1>(param).second;
    auto expected = container;
    // Sort the containers
    sortFunc(container);
    std::sort(expected.begin(), expected.end());
    // Check if the containers are equal after sorting
    EXPECT_EQ(container, expected);
}

INSTANTIATE_TEST_SUITE_P(SortingTestsGenerator, SortingTest,
    Combine(ValuesIn(sortingFunctions), ValuesIn(testVectors)),
    [](const auto& info) {
        return std::format("{}_{}", std::get<0>(info.param).first, std::get<1>(info.param).first); 
    });
