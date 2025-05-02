#include <gtest/gtest.h>
#include <format>
#include <string>
#include <vector>
#include <functional> 
#include "knapsack.hpp"

namespace alg {
namespace {

using ::testing::TestWithParam;
using ::testing::Combine;
using ::testing::ValuesIn;

using KnapFunc = std::function<int(int, std::vector<int>, std::vector<int>)>;

const std::map<std::string, KnapFunc> knapFunctions = {
    {"KnapsackRecursive", knapsackRec<std::vector<int>>},
    {"KnapsackMemoized", knapsackMemoized<std::vector<int>>},
    {"KnapsackTabulated", knapsackDP<std::vector<int>>},
    {"KnapsackTabulatedOptimized", knapsackDPOptimized<std::vector<int>>}
};

struct TestKnapsackInput {
    int capacity;
    std::vector<int> weights;
    std::vector<int> values;
    int expected;
};

const std::map<std::string, TestKnapsackInput> testCases = {
    {"NoItems", {0, {}, {}, 0}},
    {"SingleItemFits", {5, {5}, {10}, 10}},
    {"SingleItemTooHeavy", {5, {6}, {10}, 0}},
    {"SmallValues", {10, {2, 3, 5}, {1, 2, 3}, 6}},
    {"ThreeItems", {50, {10, 20, 30}, {30, 20, 15}, 50}},
    {"FourItems", {10, {5, 4, 6, 3}, {10, 40, 30, 50}, 90}},
};

struct TestKnapsackFractInput {
    double capacity;
    std::vector<int> weights;
    std::vector<int> values;
    double expected;
};

const std::map<std::string, TestKnapsackFractInput> fracTestCases = {
    {"EmptyInput", {50.0, {}, {}, 0.0}},
    {"ZeroCapacity", {0.0, {10, 20, 30}, {60, 100, 120}, 0.0}},
    {"SingleItem", {50.0, {10}, {60}, 60.0}},
    {"SingleItemTooLarge", {30.0, {60}, {50}, 25.0}},
    {"AllItemsFit", {60.0, {10, 20, 30}, {60, 100, 120}, 280.0}},
    {"SimpleCase", {50.0, {10, 20, 30}, {60, 100, 120}, 240.0}},
    {"MultipleItemsFractional", {25.0, {10, 15, 20, 25}, {40, 60, 100, 80}, 120.0}},
};

const std::map<std::string, TestKnapsackInput> unboundedTestCases = {
    {"NoItems", {0, {}, {}, 0}},
    {"SingleItemFits", {5, {5}, {10}, 10}},
    {"SingleItemTooHeavy", {5, {6}, {10}, 0}},
    {"NoValueItems", {10, {2, 3, 4}, {0, 0, 0}, 0}},
    {"MaxValueWithSmallWeight", {100, {1, 50}, {1, 30}, 100}},
    {"SimpleCase", {8, {1, 3, 4, 5}, {10, 40, 50, 70}, 110}},
    {"SimpleCase2", {10, {3, 4, 6, 8}, {15, 25, 20, 10}, 55}},
};

using KnapTestParamT = std::tuple<decltype(knapFunctions)::value_type, decltype(testCases)::value_type>;

class KnapsackTest : public TestWithParam<KnapTestParamT> {};

TEST_P(KnapsackTest, KnapsackTest) {
    // Get the parameters for the current test case
    const auto& param = GetParam();
    auto knapFunc = std::get<0>(param).second;
    const auto& [capacity, weights, values, expected] = std::get<1>(param).second;
    
    EXPECT_EQ(knapFunc(capacity, weights, values), expected);
}

INSTANTIATE_TEST_SUITE_P(KnapsackTestsGenerator, KnapsackTest,
    Combine(ValuesIn(knapFunctions), ValuesIn(testCases)),
    [](const auto& info) { 
        return std::format("{}_{}", std::get<0>(info.param).first, std::get<1>(info.param).first); 
    });

using KnapFractTestParamT = decltype(fracTestCases)::value_type;

class KnapsackFractionalTest : public TestWithParam<KnapFractTestParamT> {};

TEST_P(KnapsackFractionalTest, WorksWithAllInputs) {
    // Get the parameters for the current test case
    const auto& [capacity, weights, values, expected] = GetParam().second;

    EXPECT_EQ(knapsackFractional(capacity, weights, values), expected);
}

INSTANTIATE_TEST_SUITE_P(KanpsackFractionalTestsGenerator, KnapsackFractionalTest,
    ::testing::ValuesIn(fracTestCases),
    [](const auto& info) { return info.param.first; });

using KnapUnboundTestParamT = decltype(unboundedTestCases)::value_type;

class KnapsackUnboundedTest : public TestWithParam<KnapUnboundTestParamT> {};

TEST_P(KnapsackUnboundedTest, WorksWithAllInputs) {
    // Get the parameters for the current test case
    const auto& [capacity, weights, values, expected] = GetParam().second;

    EXPECT_EQ(knapsackUnbounded(capacity, weights, values), expected);
}

INSTANTIATE_TEST_SUITE_P(KanpsackUnboundedTestsGenerator, KnapsackUnboundedTest,
    ValuesIn(unboundedTestCases),
    [](const auto& info) { return info.param.first; });

}  // namespace
}  // namespace alg
