#include <gtest/gtest.h>
#include <functional>
#include <format>
#include <regex>
#include <vector>
#include "kadane.hpp"

namespace alg {
namespace {

using ::testing::TestWithParam;
using ::testing::Combine;
using ::testing::ValuesIn;

using KadaneFunc = std::function<int(std::vector<int>)>;
using KadaneIdxFunc = std::function<std::pair<int, int>(std::vector<int>)>;

const std::map<std::string, KadaneFunc> kadaneFunctions = {
    {"MinSubArray", getMinSubArray<std::vector<int>>},
    {"MaxSubArray", getMaxSubArray<std::vector<int>>}
};

const std::map <std::string, KadaneIdxFunc> kadaneIdxFunctions = {
    {"MinSubArrayIdx", getMinSubArrayIdx<std::vector<int>>},
    {"MaxSubArrayIdx", getMaxSubArrayIdx<std::vector<int>>},
};

struct TestKadaneInput {
    std::vector<int> container;
    std::pair<int, int> expected;
};

const std::map<std::string, TestKadaneInput> testVectors = {
    {"SinglePositiveElement", { 
        .container = {5}, 
        .expected = {5, 5} }},
    {"SingleNegativeElement", { 
        .container = {-3}, 
        .expected = {-3, -3} }},
    {"AllPositive", { 
        .container = {1, 2, 3, 4, 5}, 
        .expected = {1, 15} }},
    {"AllNegative", { 
        .container = {-1, -2, -3, -4, -5}, 
        .expected = {-15, -1} }},
    {"MixedPositiveNegative", { 
        .container = {1, -2, 3, -4, 5}, 
        .expected = {-4, 5} }},
    {"MixedPositiveNegativeLarge", { 
        .container = {2, -5, 3, 4, -1, 2, 1, -5, 4}, 
        .expected = {-5, 9} }},
    {"ConsecutivePositiveNegative", { 
        .container = {1, -1, 1, -1, 1, -1, 1}, 
        .expected = {-1, 1} }},
    {"ConsecutiveNegativePositive", { 
        .container = {-1, 1, -1, 1, -1, 1, -1}, 
        .expected = {-1, 1} }},
    {"AlternatingSigns", { 
        .container = {1, -2, 3, -4, 5}, 
        .expected = {-4, 5} }},
    {"ZerosInArray", { 
        .container = {0, 1, 0, -1, 0, 2, 0}, 
        .expected = {-1, 2} }},
    {"AllZeros", { 
        .container = {0, 0, 0, 0, 0}, 
        .expected = {0, 0} }},
    {"LargePositiveNumbers", { 
        .container = {100, 200, 300, 400, 500}, 
        .expected = {100, 1500} }},
    {"LargeNegativeNumbers", { 
        .container = {-100, -200, -300, -400, -500}, 
        .expected = {-1500, -100} }},
    {"MixedLargeNumbers", { 
        .container = {100, -200, 300, -400, 500}, 
        .expected = {-400, 500} }},
    {"LargeAlternatingSigns", { 
        .container = {1, -2, 3, -4, 5, -6, 7, -8, 9}, 
        .expected = {-8, 9} }},
    {"SingleElementLarge", { 
        .container = {1000}, 
        .expected = {1000, 1000} }},
    {"SingleElementSmall", { 
        .container = {-1000}, 
        .expected = {-1000, -1000} }},
    {"MultipleSamePositive", { 
        .container = {3, 3, 3, 3, 3}, 
        .expected = {3, 15} }},
    {"MultipleSameNegative", { 
        .container = {-3, -3, -3, -3, -3}, 
        .expected = {-15, -3} }},
};

struct TestKadaneIdxInput {
    std::vector<int> container;
    std::pair<int, int> expectedMin;
    std::pair<int, int> expectedMax;
};

const std::map<std::string, TestKadaneIdxInput> testIdxVectors = {
    {"SinglePositiveElement", { 
        .container = {5},
        .expectedMin = {0, 0}, 
        .expectedMax = {0, 0}} },
    {"SingleNegativeElement", {
        .container = {-3}, 
        .expectedMin = {0, 0}, 
        .expectedMax = {0, 0}} },
    {"AllPositive", { 
        .container = {1, 2, 3, 4, 5}, 
        .expectedMin = {0, 0}, 
        .expectedMax = {0, 4}} },
    {"AllNegative", { 
        .container = {-1, -2, -3, -4, -5}, 
        .expectedMin = {0, 4}, 
        .expectedMax = {0, 0}} },
    {"MixedPositiveNegative", { 
        .container = {1, -2, 3, -4, 5},
        .expectedMin = {3, 3}, 
        .expectedMax = {4, 4}} },
    {"MixedPositiveNegativeLarge", { 
        .container = {2, -5, 3, 4, -1, 2, 1, -5, 4}, 
        .expectedMin = {1, 1}, 
        .expectedMax = {2, 6}} },
    {"ConsecutivePositiveNegative", { 
        .container = {1, -1, 1, -1, 1, -1, 1}, 
        .expectedMin = {1, 1}, 
        .expectedMax = {0, 0}} },
    {"ConsecutiveNegativePositive", { 
        .container = {-1, 1, -1, 1, -1, 1, -1}, 
        .expectedMin = {0, 0}, 
        .expectedMax = {1, 1}} },
    {"AlternatingSigns", { 
        .container = {1, -2, 3, -4, 5}, 
        .expectedMin = {3, 3}, 
        .expectedMax = {4, 4}} },
    {"ZerosInArray", { 
        .container = {0, 1, 0, -1, 0, 2, 0}, 
        .expectedMin = {2, 3}, 
        .expectedMax = {0, 5}} },
    {"AllZeros", { 
        .container = {0, 0, 0, 0, 0},
        .expectedMin = {0, 0}, 
        .expectedMax = {0, 0}} },
    {"LargePositiveNumbers", { 
        .container = {100, 200, 300, 400, 500}, 
        .expectedMin = {0, 0}, 
        .expectedMax = {0, 4}} },
    {"LargeNegativeNumbers", { 
        .container = {-100, -200, -300, -400, -500}, 
        .expectedMin = {0, 4}, 
        .expectedMax = {0, 0}} },
    {"MixedLargeNumbers", { 
        .container = {100, -200, 300, -400, 500}, 
        .expectedMin = {3, 3}, 
        .expectedMax = {4, 4}} },
    {"LargeAlternatingSigns", { 
        .container = {1, -2, 3, -4, 5, -6, 7, -8, 9}, 
        .expectedMin = {7, 7},
        .expectedMax = {8, 8}} },
    {"SingleElementLarge", { 
        .container = {1000},
        .expectedMin = {0, 0}, 
        .expectedMax = {0, 0}} },
    {"SingleElementSmall", { 
        .container = {-1000},
        .expectedMin = {0, 0}, 
        .expectedMax = {0, 0}} },
    {"MultipleSamePositive", { 
        .container = {3, 3, 3, 3, 3}, 
        .expectedMin = {0, 0},
        .expectedMax = {0, 4}} },
    {"MultipleSameNegative", { 
        .container = {-3, -3, -3, -3, -3}, 
        .expectedMin = {0, 4}, 
        .expectedMax = {0, 0}} },
};

using KadaneTestParamT = std::tuple<decltype(kadaneFunctions)::value_type, decltype(testVectors)::value_type>;

class KadaneTest : public TestWithParam<KadaneTestParamT> {};

TEST_P(KadaneTest, WorksWithAllInputs) {
    // Get the parameters for the current test case
    const auto& param = GetParam();
    const auto& [funcName, kadaneFunc] = std::get<0>(param);
    const auto& testInput = std::get<1>(param).second;
    const auto& [container, expectedValues] = testInput;
    const auto& expected = std::regex_search(funcName, std::regex("Min"))
                            ? expectedValues.first 
                            : expectedValues.second;
    
    EXPECT_EQ(kadaneFunc(container), expected);
}

INSTANTIATE_TEST_SUITE_P(KadaneTestsGenerator, KadaneTest,
    Combine(ValuesIn(kadaneFunctions), ValuesIn(testVectors)),
    [](const auto& info) { 
        return std::format("{}_{}", std::get<0>(info.param).first, std::get<1>(info.param).first); 
    });

using KadaneIdxTestParamT = std::tuple<decltype(kadaneIdxFunctions)::value_type, decltype(testIdxVectors)::value_type>;

class KadaneIdxTest: public TestWithParam<KadaneIdxTestParamT> {};

TEST_P(KadaneIdxTest, WorksWithAllInputs) {
    // Get the parameters for the current test case
    const auto& param = GetParam();
    const auto& [funcName, kadaneFunc] = std::get<0>(param);
    const auto& testInput = std::get<1>(param).second;
    const auto& [container, expectedMin, expectedMax] = testInput;
    const auto& expected = std::regex_search(funcName, std::regex("Min"))
                            ? expectedMin
                            : expectedMax;

    EXPECT_EQ(kadaneFunc(container), expected);
}

INSTANTIATE_TEST_SUITE_P(KadaneIdxTestsGenerator, KadaneIdxTest,
    Combine(ValuesIn(kadaneIdxFunctions), ValuesIn(testIdxVectors)),
    [](const auto& info) { 
        return std::format("{}_{}", std::get<0>(info.param).first, std::get<1>(info.param).first); 
    });

}  // namespace
}  // namespace alg
