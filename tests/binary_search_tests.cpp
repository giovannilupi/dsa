#include <gtest/gtest.h>
#include <format>
#include <algorithm>
#include <tuple>
#include <vector>
#include <string>
#include <functional>
#include <iterator>
#include "binary_search.hpp"

namespace alg {
namespace {

using ::testing::TestWithParam;
using ::testing::Combine;
using ::testing::ValuesIn;

using BSearchIdxFunc = std::function<int(const std::vector<int>&, const int&)>;
using BSearchIterFunc = std::function<std::vector<int>::const_iterator(const std::vector<int>&, const int&)>;

const std::map<std::string, BSearchIdxFunc> idxBSearchFunctions = {
    {"BSearchIdxIterative", bsearch<std::vector<int>>},
    {"BSearchIdxRecursive", bSearchRec<std::vector<int>>}
};

const std::map<std::string, BSearchIterFunc> iterBSearchFunctions = {
    {"BSearchSTLContainer", bSearchLib<std::vector<int>>},
    {"BSearchSTLIterators", [](const auto& container, int val) {
        return bSearchLibIt(container.cbegin(), container.cend(), val);
    }}
};

struct TestVectInput {
    std::vector<int> container;
    int val;
    int expected;
};

const std::map<std::string, TestVectInput> testVectors = {
    {"EmptyContainer", {
        .container = {}, 
        .val = 0, 
        .expected = -1}},
    {"SingleElement", {
        .container = {1},
        .val = 1, 
        .expected = 0}},
    {"FirstElement", {
        .container = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
        .val = 1,
        .expected = 0}},
    {"MiddleElement", {
        .container = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 
        .val = 5, 
        .expected = 4}},
    {"LastElement", {
        .container = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 
        .val = 10, 
        .expected = 9}},
    {"OutOfRangeLow", {
        .container = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
        .val = 0,
        .expected = -1
    }},
    {"OutOfRangeHigh", {
        .container = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
        .val = 11,
        .expected = -1
    }}
};

struct TestMatrixInput {
    std::vector<std::vector<int>> matrix;
    int target;
    std::pair<int, int> expected;
};

const std::map<std::string, TestMatrixInput> testMatrices = {
    {"EmptyMatrix", {
        .matrix = {{}},
        .target = 1,
        .expected = {-1, -1}}},
    {"SingleElementMatrix", {
        .matrix = {{1}}, 
        .target = 1, 
        .expected = {0, 0}}},
    {"SingleElementNotFound", {
        .matrix = {{1}}, 
        .target = 2, 
        .expected = {-1, -1}}},
    {"TargetInMiddle", {
        .matrix = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 
        .target = 5, 
        .expected = {1, 1}}},
    {"TargetAtBeginning", {
        .matrix = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 
        .target = 1, 
        .expected = {0, 0}}},
    {"TargetAtEnd", {
        .matrix = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 
        .target = 9, 
        .expected = {2, 2}}},
    {"TargetNotFound", {
        .matrix = {{1, 3, 5}, {7, 9, 11}, {13, 15, 17}}, 
        .target = 6, 
        .expected = {-1, -1}}},
    {"SingleRowFound", {
        .matrix = {{1, 3, 5, 7, 9}}, 
        .target = 7, 
        .expected = {0, 3}}},
    {"SingleColumnFound", {
        .matrix = {{1}, {3}, {5}, {7}, {9}}, 
        .target = 5, 
        .expected = {2, 0}}},
};

using BSearchIdxTestParamT = std::tuple<decltype(idxBSearchFunctions)::value_type, decltype(testVectors)::value_type>;

class BSearchIndexTest : public TestWithParam<BSearchIdxTestParamT> {};

TEST_P(BSearchIndexTest, WorksWithAllInputs) {
    // Get the parameters for the current test case
    const auto& param = GetParam();
    const auto& bSearchFunc = std::get<0>(param).second;
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

    EXPECT_EQ(bsearchMatrix(matrix, val), expected);
}

INSTANTIATE_TEST_SUITE_P(BSearchMatrixTestsGenerator, BSearchMatrixTest,
    ValuesIn(testMatrices),
    [](const auto& info) { return info.param.first; });

}  // namespace
}  // namespace alg
