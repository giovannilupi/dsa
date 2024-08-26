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

static std::map<std::string, std::pair<std::tuple<double, std::vector<int>, std::vector<int>>, double>> fracTestCases = {
    {"emptyInput", {{50.0, {}, {}}, 0.0}},
    {"ZeroCapacity", {{0.0, {10, 20, 30}, {60, 100, 120}}, 0.0}},
    {"SingleItem", {{50.0, {10}, {60}}, 60.0}},
    {"SingleItemTooLarge", {{30.0, {60}, {50}}, 25.0}},
    {"AllItemsFit", {{60.0, {10, 20, 30}, {60, 100, 120}}, 280.0}},
    {"SimpleCase", {{50.0, {10, 20, 30}, {60, 100, 120}}, 240.0}},
    {"MultipleItemsFractional", {{25.0, {10, 15, 20, 25}, {40, 60, 100, 80}}, 120.0}},
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

INSTANTIATE_TEST_SUITE_P(KnapsackTestsGenerator, KnapsackTest,
    Combine(ValuesIn(knapFunctions), ValuesIn(testCases)),
    [](const testing::TestParamInfo<KnapsackTest::ParamType>& info) {
        return std::get<0>(info.param).first + "_" + std::get<1>(info.param).first;
    });

class KnapsackFactionalTest :
    public ::testing::TestWithParam<std::pair<const std::string, 
    std::pair<std::tuple<double, std::vector<int>, std::vector<int>>, double>>>
{};

TEST_P(KnapsackFactionalTest, WorksWithAllInputs) {
    // Get the parameters for the current test case
    auto param = GetParam();
    auto knapParams = std::get<1>(param);
    auto capacity = std::get<0>(knapParams.first);
    auto weights = std::get<1>(knapParams.first);
    auto values = std::get<2>(knapParams.first);
    auto expected = knapParams.second;

    EXPECT_EQ(alg::knapsackFractional(capacity, weights, values), expected);
}

INSTANTIATE_TEST_SUITE_P(KanpsackFractionalTestsGenerator, KnapsackFactionalTest,
    ::testing::ValuesIn(fracTestCases),
    [](const testing::TestParamInfo<KnapsackFactionalTest::ParamType>& info) {
        return std::get<0>(info.param);
    });