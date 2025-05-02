#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <format>
#include <functional>
#include <ranges>
#include "power_set.hpp"

namespace alg {
namespace {

using ::testing::TestWithParam;
using ::testing::Combine;
using ::testing::ValuesIn;
using ::testing::UnorderedElementsAreArray;

using PowerSetFunc = std::function<std::vector<std::vector<int>>(std::vector<int>)>;

const std::map<std::string, PowerSetFunc> powerSetFunctions = {
    {"PowerSetIter", powerSetIter<std::vector<int>>},
    {"PowerSetRec", powerSetDFS<std::vector<int>>},
    {"PowerSetLib", powerSetBacktrack<std::vector<int>>}
};

struct TestPowerSetInput {
    std::vector<int> input;
    std::vector<std::vector<int>> expected;
};

const std::map<std::string, TestPowerSetInput> testCases = {
    {"EmptySet", {
        .input = {}, 
        .expected = {{}}
    }},
    {"SingleElement", {
        .input = {1}, 
        .expected = {{}, {1}}}
    },
    {"TwoElements", {
        .input = {1, 2}, 
        .expected = {{}, {1}, {2}, {1, 2}}}
    },
    {"ThreeElements", {
        .input = {1, 2, 3}, 
        .expected = {{}, {1}, {2}, {3}, {1, 2}, {1, 3}, {2, 3}, {1, 2, 3}}}
    },
    // Testing with negative and positive numbers
    {"MixedElements", {
        .input = {-1, 2, -3}, 
        .expected = {{}, {-1}, {2}, {-3}, {-1, 2}, {-1, -3}, {2, -3}, {-1, 2, -3}}}
    },
    {"LargerSet", {
        .input = {1, 2, 3, 4}, 
        .expected = {{}, {1}, {2}, {3}, {4}, {1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, 
          {3, 4}, {1, 2, 3}, {1, 2, 4}, {1, 3, 4}, {2, 3, 4}, {1, 2, 3, 4}}}
    },
    {"RangeOfElements", {
        .input = {-2, 0, 2}, 
        .expected = {{}, {-2}, {0}, {2}, {-2, 0}, {-2, 2}, {0, 2}, {-2, 0, 2}}}
    }
};

using PowerSetParamT = std::tuple<decltype(powerSetFunctions)::value_type, decltype(testCases)::value_type>;

class PowerSetTest : public TestWithParam<PowerSetParamT> {};

TEST_P(PowerSetTest, TestPowerSet) {
    const auto& param = GetParam();
    const auto& powerSetFunc = std::get<0>(param).second;
    const auto& input = std::get<1>(param).second.input;
    auto expected = std::get<1>(param).second.expected;
    auto res = powerSetFunc(input);

    // Ignore the order of the elements in the comparison
    for (auto& subset : expected) std::ranges::sort(subset);
    for (auto& subset : res) std::ranges::sort(subset);
    EXPECT_THAT(res, UnorderedElementsAreArray(expected));
}

INSTANTIATE_TEST_SUITE_P(PowerSetTestsGenerator, PowerSetTest,
    Combine(ValuesIn(powerSetFunctions), ValuesIn(testCases)),
    [](const auto& info) { 
        return std::format("{}_{}", std::get<0>(info.param).first, std::get<1>(info.param).first); 
    });

}  // namespace
}  // namespace alg
