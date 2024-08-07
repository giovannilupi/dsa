#include <functional>
#include <gtest/gtest.h>
#include <regex>
#include "kadane.hpp"

using KadaneFunc = std::function<int(std::vector<int>)>;
using KadaneIdxFunc = std::function<std::pair<int, int>(std::vector<int>)>;
using ::testing::Combine;
using ::testing::ValuesIn;

static std::map<std::string, KadaneFunc> kadaneFunctions = {
    {"MinSubArray", alg::getMinSubArray<std::vector<int>>},
    {"MaxSubArray", alg::getMaxSubArray<std::vector<int>>}
}; 

static std::map <std::string, KadaneIdxFunc> kadaneIdxFunctions = {
    {"MinSubArrayIdx", alg::getMinSubArrayIdx<std::vector<int>>},
    {"MaxSubArrayIdx", alg::getMaxSubArrayIdx<std::vector<int>>},
};

static std::map<std::string, std::pair<std::vector<int>, std::pair<int, int>>> testVectors = {
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

static std::map<std::string, std::pair<std::vector<int>, std::pair<std::pair<int, int>, std::pair<int, int>>>> testIdxVectors = {
    {"SinglePositiveElement", { {5}, {{0, 0}, {0, 0}} }},
    {"SingleNegativeElement", { {-3}, {{0, 0}, {0, 0}} }},
    {"AllPositive", { {1, 2, 3, 4, 5}, {{0, 0}, {0, 4}} }},
    {"AllNegative", { {-1, -2, -3, -4, -5}, {{0, 4}, {0, 0}} }},
    {"MixedPositiveNegative", { {1, -2, 3, -4, 5}, {{3, 3}, {4, 4}} }},
    {"MixedPositiveNegativeLarge", { {2, -5, 3, 4, -1, 2, 1, -5, 4}, {{1, 1}, {2, 6}} }},
    {"ConsecutivePositiveNegative", { {1, -1, 1, -1, 1, -1, 1}, {{1, 1}, {0, 0}} }},
    {"ConsecutiveNegativePositive", { {-1, 1, -1, 1, -1, 1, -1}, {{0, 0}, {1, 1}} }},
    {"AlternatingSigns", { {1, -2, 3, -4, 5}, {{3, 3}, {4, 4}} }},
    {"ZerosInArray", { {0, 1, 0, -1, 0, 2, 0}, {{2, 3}, {0, 5}} }},
    {"AllZeros", { {0, 0, 0, 0, 0}, {{0, 0}, {0, 0}} }},
    {"LargePositiveNumbers", { {100, 200, 300, 400, 500}, {{0, 0}, {0, 4}} }},
    {"LargeNegativeNumbers", { {-100, -200, -300, -400, -500}, {{0, 4}, {0, 0}} }},
    {"MixedLargeNumbers", { {100, -200, 300, -400, 500}, {{3, 3}, {4, 4}} }},
    {"LargeAlternatingSigns", { {1, -2, 3, -4, 5, -6, 7, -8, 9}, {{7, 7}, {8, 8}} }},
    {"SingleElementLarge", { {1000}, {{0, 0}, {0, 0}} }},
    {"SingleElementSmall", { {-1000}, {{0, 0}, {0, 0}} }},
    {"MultipleSamePositive", { {3, 3, 3, 3, 3}, {{0, 0}, {0, 4}} }},
    {"MultipleSameNegative", { {-3, -3, -3, -3, -3}, {{0, 4}, {0, 0}} }},
};

class KadaneTest : 
    public ::testing::TestWithParam<std::tuple<
    std::pair<const std::string, KadaneFunc>,
    std::pair<const std::string, std::pair<std::vector<int>, std::pair<int, int>>>>>
{};

class KadaneIdxTest:
    public ::testing::TestWithParam<std::tuple<
    std::pair<const std::string, KadaneIdxFunc>,
    std::pair<const std::string, std::pair<std::vector<int>, std::pair<std::pair<int, int>, std::pair<int, int>>>>>>
{};

TEST_P(KadaneTest, WorksWithAllInputs) {
    // Get the parameters for the current test case
    auto param = GetParam();
    auto funcName = std::get<0>(param).first;
    auto kadaneFunc = std::get<0>(param).second;
    auto testInput = std::get<1>(param).second;
    auto container = testInput.first;
    auto expected = std::regex_search(funcName, std::regex("Min"))
                            ? testInput.second.first 
                            : testInput.second.second;
    
    EXPECT_EQ(kadaneFunc(container), expected);
}

TEST_P(KadaneIdxTest, WorksWithAllInputs) {
    // Get the parameters for the current test case
    auto param = GetParam();
    auto funcName = std::get<0>(param).first;
    std::cout << funcName << std::endl;
    auto kadaneFunc = std::get<0>(param).second;
    auto testInput = std::get<1>(param).second;
    auto container = testInput.first;
    auto expected = std::regex_search(funcName, std::regex("Min"))
                            ? testInput.second.first 
                            : testInput.second.second;

    EXPECT_EQ(kadaneFunc(container), expected);
}

INSTANTIATE_TEST_SUITE_P(KadaneTestsGenerator, KadaneTest,
    Combine(ValuesIn(kadaneFunctions), ValuesIn(testVectors)),
    [](const testing::TestParamInfo<KadaneTest::ParamType>& info) {
        return std::get<0>(info.param).first + "_" + std::get<1>(info.param).first;
    });

INSTANTIATE_TEST_SUITE_P(KadaneIdxTestsGenerator, KadaneIdxTest,
    Combine(ValuesIn(kadaneIdxFunctions), ValuesIn(testIdxVectors)),
    [](const testing::TestParamInfo<KadaneIdxTest::ParamType>& info) {
        return std::get<0>(info.param).first + "_" + std::get<1>(info.param).first;
    });
