#include <gtest/gtest.h>
#include <functional>
#include <format>
#include <regex>
#include <vector>
#include "kadane.hpp"

using ::testing::TestWithParam;
using ::testing::Combine;
using ::testing::ValuesIn;

namespace {

using KadaneFunc = std::function<int(std::vector<int>)>;
using KadaneIdxFunc = std::function<std::pair<int, int>(std::vector<int>)>;

const std::map<std::string, KadaneFunc> kadaneFunctions = {
    {"MinSubArray", alg::getMinSubArray<std::vector<int>>},
    {"MaxSubArray", alg::getMaxSubArray<std::vector<int>>}
};

const std::map <std::string, KadaneIdxFunc> kadaneIdxFunctions = {
    {"MinSubArrayIdx", alg::getMinSubArrayIdx<std::vector<int>>},
    {"MaxSubArrayIdx", alg::getMaxSubArrayIdx<std::vector<int>>},
};

struct TestKadaneInput {
    const std::vector<int> container;
    const std::pair<int, int> expected;
};

const std::map<std::string, TestKadaneInput> testVectors = {
    {"SinglePositiveElement", { {5}, {5, 5} }},
    {"SingleNegativeElement", { {-3}, {-3, -3} }},
    {"AllPositive", { {1, 2, 3, 4, 5}, {1, 15} }},
    {"AllNegative", { {-1, -2, -3, -4, -5}, {-15, -1} }},
    {"MixedPositiveNegative", { {1, -2, 3, -4, 5}, {-4, 5} }},
    {"MixedPositiveNegativeLarge", { {2, -5, 3, 4, -1, 2, 1, -5, 4}, {-5, 9} }},
    {"ConsecutivePositiveNegative", { {1, -1, 1, -1, 1, -1, 1}, {-1, 1} }},
    {"ConsecutiveNegativePositive", { {-1, 1, -1, 1, -1, 1, -1}, {-1, 1} }},
    {"AlternatingSigns", { {1, -2, 3, -4, 5}, {-4, 5} }},
    {"ZerosInArray", { {0, 1, 0, -1, 0, 2, 0}, {-1, 2} }},
    {"AllZeros", { {0, 0, 0, 0, 0}, {0, 0} }},
    {"LargePositiveNumbers", { {100, 200, 300, 400, 500}, {100, 1500} }},
    {"LargeNegativeNumbers", { {-100, -200, -300, -400, -500}, {-1500, -100} }},
    {"MixedLargeNumbers", { {100, -200, 300, -400, 500}, {-400, 500} }},
    {"LargeAlternatingSigns", { {1, -2, 3, -4, 5, -6, 7, -8, 9}, {-8, 9} }},
    {"SingleElementLarge", { {1000}, {1000, 1000} }},
    {"SingleElementSmall", { {-1000}, {-1000, -1000} }},
    {"MultipleSamePositive", { {3, 3, 3, 3, 3}, {3, 15} }},
    {"MultipleSameNegative", { {-3, -3, -3, -3, -3}, {-15, -3} }},
};

struct TestKadaneIdxInput {
    const std::vector<int> container;
    const std::pair<int, int> expectedMin;
    const std::pair<int, int> expectedMax;
};

const std::map<std::string, TestKadaneIdxInput> testIdxVectors = {
    {"SinglePositiveElement", { {5}, {0, 0}, {0, 0}} },
    {"SingleNegativeElement", { {-3}, {0, 0}, {0, 0}} },
    {"AllPositive", { {1, 2, 3, 4, 5}, {0, 0}, {0, 4}} },
    {"AllNegative", { {-1, -2, -3, -4, -5}, {0, 4}, {0, 0}} },
    {"MixedPositiveNegative", { {1, -2, 3, -4, 5}, {3, 3}, {4, 4}} },
    {"MixedPositiveNegativeLarge", { {2, -5, 3, 4, -1, 2, 1, -5, 4}, {1, 1}, {2, 6}} },
    {"ConsecutivePositiveNegative", { {1, -1, 1, -1, 1, -1, 1}, {1, 1}, {0, 0}} },
    {"ConsecutiveNegativePositive", { {-1, 1, -1, 1, -1, 1, -1}, {0, 0}, {1, 1}} },
    {"AlternatingSigns", { {1, -2, 3, -4, 5}, {3, 3}, {4, 4}} },
    {"ZerosInArray", { {0, 1, 0, -1, 0, 2, 0}, {2, 3}, {0, 5}} },
    {"AllZeros", { {0, 0, 0, 0, 0}, {0, 0}, {0, 0}} },
    {"LargePositiveNumbers", { {100, 200, 300, 400, 500}, {0, 0}, {0, 4}} },
    {"LargeNegativeNumbers", { {-100, -200, -300, -400, -500}, {0, 4}, {0, 0}} },
    {"MixedLargeNumbers", { {100, -200, 300, -400, 500}, {3, 3}, {4, 4}} },
    {"LargeAlternatingSigns", { {1, -2, 3, -4, 5, -6, 7, -8, 9}, {7, 7}, {8, 8}} },
    {"SingleElementLarge", { {1000}, {0, 0}, {0, 0}} },
    {"SingleElementSmall", { {-1000}, {0, 0}, {0, 0}} },
    {"MultipleSamePositive", { {3, 3, 3, 3, 3}, {0, 0}, {0, 4}} },
    {"MultipleSameNegative", { {-3, -3, -3, -3, -3}, {0, 4}, {0, 0}} },
};

} // namespace

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
