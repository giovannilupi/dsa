#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <functional> 
#include "knapsack.hpp"

using KnapFunc = std::function<int(int, std::vector<int>, std::vector<int>)>;
using ::testing::Combine;
using ::testing::ValuesIn;

static std::map<std::string, KnapFunc> knapFunctions = {
    {"KnapsackRecursive", alg::knapsackRec<std::vector<int>>},
    {"KnapsackMemoized", alg::knapsackMemoized<std::vector<int>>},
    {"KnapsackTabulation", alg::knapsackDP<std::vector<int>>}
};

static std::map<std::string, std::pair<std::tuple<int, std::vector<int>, std::vector<int>>, int>> testCases = {
    {"NoItems", {{0, {}, {}}, 0}},
    {"SingleItemFits", {{5, {5}, {10}}, 10}},
    {"SingleItemTooHeavy", {{5, {6}, {10}}, 0}},
    {"SmallValues", {{10, {2, 3, 5}, {1, 2, 3}}, 6}},
    {"ThreeItems", {{50, {10, 20, 30}, {30, 20, 15}}, 50}},
    {"FourItems", {{10, {5, 4, 6, 3}, {10, 40, 30, 50}}, 90}},
};

class KnapsackTest :
    public ::testing::TestWithParam<std::tuple<
    std::pair<const std::string, KnapFunc>,
    std::pair<const std::string, std::pair<std::tuple<int, std::vector<int>, std::vector<int>>, int>>>> 
{};

TEST_P(KnapsackTest, KnapsackTest) {
    // Get the parameters for the current test case
    auto param = GetParam();
    auto knapFunc = std::get<0>(param).second;
    auto knapParams = std::get<1>(param).second;
    auto capacity = std::get<0>(knapParams.first);
    auto weights = std::get<1>(knapParams.first);
    auto values = std::get<2>(knapParams.first);
    auto expected = knapParams.second;
    
    EXPECT_EQ(knapFunc(capacity, weights, values), expected);
}

INSTANTIATE_TEST_SUITE_P(KanpsackTestsGenerator, KnapsackTest,
    Combine(ValuesIn(knapFunctions), ValuesIn(testCases)),
    [](const testing::TestParamInfo<KnapsackTest::ParamType>& info) {
        return std::get<0>(info.param).first + "_" + std::get<1>(info.param).first;
    });
