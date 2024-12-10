#include <gtest/gtest.h>
#include <format>
#include <algorithm>
#include <tuple>
#include <vector>
#include <string>
#include <functional>
#include <iterator>
#include "binary_search.hpp"

using ::testing::TestWithParam;
using ::testing::Combine;
using ::testing::ValuesIn;

namespace {

using BSearchIdxFunc = std::function<int(const std::vector<int>&, const int&)>;
using BSearchIterFunc = std::function<std::vector<int>::const_iterator(const std::vector<int>&, const int&)>;

const std::map<std::string, BSearchIdxFunc> idxBSearchFunctions = {
    {"BSearchIdxIterative", alg::bsearch<std::vector<int>>},
    {"BSearchIdxRecursive", alg::bSearchRec<std::vector<int>>}
};

const std::map<std::string, BSearchIterFunc> iterBSearchFunctions = {
    {"BSearchSTLContainer", alg::bSearchLib<std::vector<int>>},
    {"BSearchSTLIterators", [](const auto& container, int val) {
        return alg::bSearchLibIt(container.cbegin(), container.cend(), val);
    }}
};

struct TestVectInput {
    const std::vector<int> container;
    const int val;
    const int expected;
};

const std::map<std::string, TestVectInput> testVectors = {
    {"EmptyContainer", {
        {}, 
        0, 
        -1}},
    {"SingleElement", {
        {1},
        1, 
        0}},
    {"FirstElement", {
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
        1,
        0}},
    {"MiddleElement", {
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 
        5, 
        4}},
    {"LastElement", {
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 
        10, 
        9}},
    {"OutOfRangeLow", {
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
        0,
        -1
    }},
    {"OutOfRangeHigh", {
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
        11,
        -1
    }}
};

struct TestMatrixInput {
    const std::vector<std::vector<int>> matrix;
    const int target;
    const std::pair<int, int> expected;
};

const std::map<std::string, TestMatrixInput> testMatrices = {
    {"EmptyMatrix", {{{}}, 1, {-1, -1}}},
    {"SingleElementMatrix", {{{1}}, 1, {0, 0}}},
    {"SingleElementNotFound", {{{1}}, 2, {-1, -1}}},
    {"TargetInMiddle", {{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 5, {1, 1}}},
    {"TargetAtBeginning", {{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 1, {0, 0}}},
    {"TargetAtEnd", {{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 9, {2, 2}}},
    {"TargetNotFound", {{{1, 3, 5}, {7, 9, 11}, {13, 15, 17}}, 6, {-1, -1}}},
    {"SingleRowFound", {{{1, 3, 5, 7, 9}}, 7, {0, 3}}},
    {"SingleColumnFound", {{{1}, {3}, {5}, {7}, {9}}, 5, {2, 0}}},
};

} // namespace

using BSearchIdxTestParamT = std::tuple<decltype(idxBSearchFunctions)::value_type, decltype(testVectors)::value_type>;

class BSearchIndexTest : public TestWithParam<BSearchIdxTestParamT> {};

TEST_P(BSearchIndexTest, WorksWithAllInputs) {
    // Get the parameters for the current test case
    const auto& param = GetParam();
    const auto& bSearchFunc = std::get<0>(param).second;
    const auto& testInput = std::get<1>(param).second;
    const auto& [container, val, expected] = std::get<1>(param).second;

    EXPECT_EQ(bSearchFunc(container, val), expected);
}

INSTANTIATE_TEST_SUITE_P(BSearchIdxTestsGenerator, BSearchIndexTest, 
    Combine(ValuesIn(idxBSearchFunctions), ValuesIn(testVectors)),
    [](const auto& info) { 
        return std::format("{}_{}", std::get<0>(info.param).first, std::get<1>(info.param).first); 
    });

using BSearchIterTestParamT = std::tuple<decltype(iterBSearchFunctions)::value_type, decltype(testVectors)::value_type>;

class BSearchIteratorTest : public TestWithParam<BSearchIterTestParamT> {};

TEST_P(BSearchIteratorTest, SearchTests) {
    // Get the parameters for the current test case
    const auto& param = GetParam();
    const auto& bSearchFunc = std::get<0>(param).second;
    const auto& [container, val, expectedIdx] = std::get<1>(param).second;
    const auto expectedIter = (expectedIdx == -1)
                            ? container.cend() 
                            : std::next(container.cbegin(), expectedIdx);

    EXPECT_EQ(bSearchFunc(container, val), expectedIter);
}

INSTANTIATE_TEST_SUITE_P(BSearchIterTestsGenerator, BSearchIteratorTest, 
    Combine(ValuesIn(iterBSearchFunctions), ValuesIn(testVectors)),
    [](const auto& info) { 
        return std::format("{}_{}", std::get<0>(info.param).first, std::get<1>(info.param).first); 
    });

using BSearchMatrixTestParamT = decltype(testMatrices)::value_type;

class BSearchMatrixTest : public TestWithParam<BSearchMatrixTestParamT> {};

TEST_P(BSearchMatrixTest, SearchTests) {
    // Get the parameters for the current test case
    const auto& [matrix, val, expected] = GetParam().second;

    EXPECT_EQ(alg::bsearchMatrix(matrix, val), expected);
}

INSTANTIATE_TEST_SUITE_P(BSearchMatrixTestsGenerator, BSearchMatrixTest,
    ValuesIn(testMatrices),
    [](const auto& info) { return info.param.first; });
