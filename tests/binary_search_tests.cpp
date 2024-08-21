#include <gtest/gtest.h>
#include <algorithm>
#include <tuple>
#include <vector>
#include <string>
#include <functional>
#include <iterator>
#include "binary_search.hpp"

// Type alias for the binary search functions with index return type
using BSearchIdxFunc = std::function<int(const std::vector<int>&, const int&)>;

// Type alias for the binary search functions with iterator return type
using BSearchIterFunc = std::function<std::vector<int>::const_iterator(const std::vector<int>&, const int&)>;

// Type alias for the binary search on a matrix
using BSearchMatrixFunc = std::function<std::pair<int, int>(const std::vector<std::vector<int>>& matrix, const int& target)>;

using ::testing::Combine;
using ::testing::ValuesIn;

static std::map<std::string, BSearchIdxFunc> idxBSearchFunctions = {
    {"BSearchIdxIterative", alg::bsearch<std::vector<int>>},
    {"BSearchIdxRecursive", alg::bSearchRec<std::vector<int>>}
};

static std::map<std::string, BSearchIterFunc> iterBSearchFunctions = {
    {"BSearchSTLContainer", alg::bSearchLib<std::vector<int>>},
    {"BSearchSTLIterators", [](const std::vector<int>& container, const int& val) {
        return alg::bSearchLibIt(container.cbegin(), container.cend(), val);
    }}
};

static std::map<std::string, std::tuple<std::vector<int>, int, int>> testVectors = {
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

static std::map<std::string, std::tuple<std::vector<std::vector<int>>, int, std::pair<int, int>>> testMatrices = {
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

class BSearchIndexTest : 
    public ::testing::TestWithParam<std::tuple<
    std::pair<const std::string, BSearchIdxFunc>,
    std::pair<const std::string, std::tuple<std::vector<int>, int, int>>>>
{};

class BSearchIteratorTest : 
    public ::testing::TestWithParam<std::tuple<
    std::pair<const std::string, BSearchIterFunc>,
    std::pair<const std::string, std::tuple<std::vector<int>, int, int>>>>
{};

class BSearchMatrixTest : 
    public ::testing::TestWithParam<std::pair<
    const std::string, std::tuple<std::vector<std::vector<int>>, int, std::pair<int, int>>>>
{};

TEST_P(BSearchIndexTest, WorksWithAllInputs) {
    // Get the parameters for the current test case
    auto param = GetParam();
    auto bSearchFunc = std::get<0>(param).second;
    auto testInput = std::get<1>(param).second;
    auto container = std::get<0>(testInput);
    auto val = std::get<1>(testInput);
    auto expected = std::get<2>(testInput);

    EXPECT_EQ(bSearchFunc(container, val), expected);
}

TEST_P(BSearchIteratorTest, SearchTests) {
    // Get the parameters for the current test case
    auto param = GetParam();
    auto bSearchFunc = std::get<0>(param).second;
    auto testInput = std::get<1>(param).second;
    auto container = std::get<0>(testInput);
    auto val = std::get<1>(testInput);
    auto expectedIdx = std::get<2>(testInput);
    auto expectedIter = (expectedIdx == -1)
                            ? container.cend() 
                            : std::next(container.cbegin(), expectedIdx);

    EXPECT_EQ(bSearchFunc(container, val), expectedIter);
}

TEST_P(BSearchMatrixTest, SearchTests) {
    // Get the parameters for the current test case
    auto param = GetParam().second;
    auto matrix = std::get<0>(param);
    auto val = std::get<1>(param);
    auto expected = std::get<2>(param);

    EXPECT_EQ(alg::bsearchMatrix(matrix, val), expected);
}

INSTANTIATE_TEST_SUITE_P(BSearchIdxTestsGenerator, BSearchIndexTest, 
    Combine(ValuesIn(idxBSearchFunctions), ValuesIn(testVectors)),
    [](const testing::TestParamInfo<BSearchIndexTest::ParamType>& info) {
        return std::get<0>(info.param).first + "_" + std::get<1>(info.param).first;
    });

INSTANTIATE_TEST_SUITE_P(BSearchIterTestsGenerator, BSearchIteratorTest, 
    Combine(ValuesIn(iterBSearchFunctions), ValuesIn(testVectors)),
    [](const testing::TestParamInfo<BSearchIteratorTest::ParamType>& info) {
        return std::get<0>(info.param).first + "_" + std::get<1>(info.param).first;
    });

INSTANTIATE_TEST_SUITE_P(BSearchMatrixTestsGenerator, BSearchMatrixTest,
    ::testing::ValuesIn(testMatrices),
    [](const testing::TestParamInfo<BSearchMatrixTest::ParamType>& info) {
        return std::get<0>(info.param);
    });
