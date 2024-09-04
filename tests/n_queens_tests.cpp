#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "n_queens.hpp"

static std::map<std::string, std::pair<int, std::vector<std::vector<int>>>> testCases = {
    {"OneQueen", {1, {{0}}}},
    {"TwoQueens", {2, {}}},
    {"ThreeQueens", {3,{}}},
    {"FourQueens", {4, {{1, 3, 0, 2}, {2, 0, 3, 1}}}},
    {"FiveQueens", {5, {
    {0, 2, 4, 1, 3}, {0, 3, 1, 4, 2}, {1, 3, 0, 2, 4}, {1, 4, 2, 0, 3}, 
    {2, 0, 3, 1, 4}, {2, 4, 1, 3, 0}, {3, 0, 2, 4, 1}, {3, 1, 4, 2, 0}, 
    {4, 1, 3, 0, 2}, {4, 2, 0, 3, 1}}}},
};

class NQueensTest : 
    public ::testing::TestWithParam<std::pair<const std::string, std::pair<int, std::vector<std::vector<int>>>>>
{};

TEST_P(NQueensTest, WorksWithAllInputs) {
    // Get the parameters for the current test case
    auto param = GetParam();
    auto n = std::get<1>(param).first;
    auto expected = std::get<1>(param).second;
    // Check if the board is valid
    EXPECT_THAT(alg::nQueens(n), ::testing::UnorderedElementsAreArray(expected));
}

INSTANTIATE_TEST_SUITE_P(NQueensTestsGenerator, NQueensTest,
    ::testing::ValuesIn(testCases),
    [](const testing::TestParamInfo<NQueensTest::ParamType>& info) {
        return std::get<0>(info.param); // Return the description as the test name
    });
