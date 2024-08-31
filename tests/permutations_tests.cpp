#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "permutations.hpp"

static std::map<std::string, std::pair<std::vector<int>, std::vector<std::vector<int>>>> testCases = {
    {"EmptyInput", {{}, {{}}}},
    {"SingleElement", {{1}, {{1}}}},
    {"TwoElements", {{1, 2}, {{1, 2}, {2, 1}}}},
    {"ThreeElements", {{1, 2, 3}, {{1, 2, 3}, {1, 3, 2}, {2, 1, 3}, {2, 3, 1}, {3, 1, 2}, {3, 2, 1}}}},
    {"DuplicateElements", {{1, 1}, {{1, 1}, {1, 1}}}},
    {"FourElements", {{1, 2, 3, 4}, {
        {1, 2, 3, 4}, {1, 2, 4, 3}, {1, 3, 2, 4}, {1, 3, 4, 2}, {1, 4, 2, 3}, {1, 4, 3, 2},
        {2, 1, 3, 4}, {2, 1, 4, 3}, {2, 3, 1, 4}, {2, 3, 4, 1}, {2, 4, 1, 3}, {2, 4, 3, 1},
        {3, 1, 2, 4}, {3, 1, 4, 2}, {3, 2, 1, 4}, {3, 2, 4, 1}, {3, 4, 1, 2}, {3, 4, 2, 1},
        {4, 1, 2, 3}, {4, 1, 3, 2}, {4, 2, 1, 3}, {4, 2, 3, 1}, {4, 3, 1, 2}, {4, 3, 2, 1}
    }}},
};

class PermutationsTest : 
    public ::testing::TestWithParam<std::pair<
    const std::string, std::pair<std::vector<int>, std::vector<std::vector<int>>>>> 
{};


TEST_P(PermutationsTest, TestPermutations) {
    auto param = GetParam();
    auto vec = param.second.first;
    auto expected = param.second.second;
    // Ignore the order of the elements in the comparison
    auto res = alg::permutations(vec);
    EXPECT_THAT(res, ::testing::UnorderedElementsAreArray(expected));
}

INSTANTIATE_TEST_SUITE_P(PermutationsTestsGenerator, PermutationsTest,
    ::testing::ValuesIn(testCases),
    [](const testing::TestParamInfo<PermutationsTest::ParamType>& info) {
        return std::get<0>(info.param); // Return the description as the test name
    });
