#include <gtest/gtest.h>
#include <vector>
#include <list>
#include <regex>
#include "selection.hpp"

using ExtremeValFunc = std::function<int(std::vector<int>&)>;
using MinMaxFunc = std::function<std::pair<int, int>(std::vector<int>&)>;
using SelectionFunc = std::function<int(std::vector<int>&, int)>;
using ::testing::Combine;
using ::testing::ValuesIn;

static std::map<std::string, ExtremeValFunc> extremeValFunctions = {
    {"GetMin", alg::getMin<std::vector<int>>},
    {"GetMax", alg::getMax<std::vector<int>>},
};

static std::map<std::string, MinMaxFunc> minMaxFunctions = {
    {"GetMinMaxNaive", alg::getMinMax<std::vector<int>>},
    {"GetMinMaxOptimal", alg::getMinMaxOpt<std::vector<int>>},
};

static std::map<std::string, SelectionFunc> selectionFunctions = {
    {"SelectMinNaive", alg::selectNthMin<std::vector<int>>},
    {"SelectMaxNaice", alg::selectNthMax<std::vector<int>>},
    {"QuickSelectMin", alg::quickSelectMin<std::vector<int>>},
    {"QuickSelectMax", alg::quickSelectMax<std::vector<int>>},
    {"HeapSelectMin", alg::heapSelectMin<std::vector<int>>},
    {"HeapSelectMax", alg::heapSelectMax<std::vector<int>>},
    {"SelectMinSTL", alg::selectMinModern<std::vector<int>>},
    {"SelectMaxSTL", alg::selectMaxModern<std::vector<int>>},
};

static std::map<std::string, std::tuple<std::vector<int>, int, int>> testInputMinMax = {
    {"SinglePositive", {{1}, 1, 1}},
    {"SingleNegative", {{-1}, -1, -1}},
    {"AllPositive", {{1, 2, 3, 4, 5}, 1, 5}},
    {"AllNegative", {{-1, -2, -3, -4, -5}, -5, -1}},
    {"AlternateSigns", {{-1, 2, -3, 4, -5}, -5, 4}},
    {"Mixed", {{-1, 2, 3, -1, -2, -3, -4, -5}, -5, 3}},
    {"PositiveNegative", {{10, 20, 30, 40, 50, -10, -20, -30}, -30, 50}},
    {"NegativePositive", {{-10, -20, -30, -40, -50, 10, 20, 30}, -50, 30}},                                        
    {"LongVector", {{1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49}, 1, 49}},  
};

static std::map<std::string, std::tuple<std::pair<std::vector<int>, std::size_t>, int, int>> testInputSelection = {
    {"SinglePositive", { { {1}, 0 }, 1, 1 }},
    {"SingleNegative", { { {-1}, 0 }, -1, -1 }},
    {"AllPositive", { { {1, 2, 3, 4, 5}, 2 }, 3, 3 }},
    {"AllNegative", { { {-5, -4, -3, -2, -1}, 1 }, -4, -2 }},
    {"MixedValues", { { {1, -1, 2, -2, 3, -3}, 2 }, -1, 1 }},
    {"LargeValues", { { {100, 200, 300, -100, -200}, 1 }, -100, 200 }},
    {"AllZeros", { { {0, 0, 0, 0}, 0 }, 0, 0 }},
    {"IncreasingSequence", { { {1, 3, 5, 7, 9}, 0 }, 1, 9 }},
    {"DecreasingSequence", { { {9, 7, 5, 3, 1}, 0 }, 1, 9 }},
    {"AlternatingSigns", { { {-10, 10, -20, 20, -30, 30}, 3 }, 10, -10 }},
};

static std::map<std::string, std::pair<std::vector<int>, int>> testInputMedian = {
    {"SingleElement", { {1}, 1 }},
    {"TwoElementsEven", { {1, 2}, 1 }},
    {"ThreeElementsOdd", { {1, 2, 3}, 2 }},
    {"FourElementsEven", { {1, 2, 3, 4}, 2 }},
    {"FiveElementsOdd", { {5, 1, 3, 2, 4}, 3 }},
    {"AllPositive", { {10, 20, 30, 40, 50}, 30 }},
    {"AllNegative", { {-10, -20, -30, -40, -50}, -30 }},
    {"MixedValues", { {3, -1, 2, -2, 1}, 1 }},
    {"LargeValues", { {100, 200, 300, 400, 500}, 300 }},
    {"DecreasingSequence", { {9, 7, 5, 3, 1}, 5 }},
    {"IncreasingSequence", { {1, 3, 5, 7, 9}, 5 }},
    {"AlternatingSigns", { {10, -10, 20, -20, 30}, 10 }},
    {"MultipleSameValues", { {1, 1, 1, 1, 1}, 1 }},
};

class ExtremeValueTest :
    public ::testing::TestWithParam<std::tuple<
    std::pair<const std::string, ExtremeValFunc>,
    std::pair<const std::string, std::tuple<std::vector<int>, int, int>>>>
{};

class MinMaxTest :
    public ::testing::TestWithParam<std::tuple<
    std::pair<const std::string, MinMaxFunc>,
    std::pair<const std::string, std::tuple<std::vector<int>, int, int>>>>
{};

class SelectionTest :
    public ::testing::TestWithParam<std::tuple<
    std::pair<const std::string, SelectionFunc>,
    std::pair<const std::string, std::tuple<std::pair<std::vector<int>, std::size_t>, int, int>>>>
{};

class MedianTest :
    public ::testing::TestWithParam<std::pair<
    const std::string, std::pair<std::vector<int>, int>>> 
{};

TEST_P(ExtremeValueTest, WorksWithAllInputs) {
    auto param = GetParam();
    auto funcName = std::get<0>(param).first;
    auto func = std::get<0>(param).second;
    auto testInput = std::get<1>(param).second;
    auto vec = std::get<0>(testInput);
    auto expected = std::regex_search(funcName, std::regex("Min"))
                            ? std::get<1>(testInput)
                            : std::get<2>(testInput);

    EXPECT_EQ(func(vec), expected);
}

TEST_P(MinMaxTest, WorksWithAllInputs) {
    auto param = GetParam();
    auto func = std::get<0>(param).second;
    auto testInput = std::get<1>(param).second;
    auto vec = std::get<0>(testInput);
    decltype(func(vec)) expected = {std::get<1>(testInput), std::get<2>(testInput)};

    EXPECT_EQ(func(vec), expected);
}

TEST_P(SelectionTest, WorksWithAllInputs) {
    auto param = GetParam();
    auto funcName = std::get<0>(param).first;
    auto func = std::get<0>(param).second;
    auto testInput = std::get<1>(param).second;
    auto vec = std::get<0>(testInput).first;
    auto index = std::get<0>(testInput).second;
    auto expected = std::regex_search(funcName, std::regex("Min"))
                            ? std::get<1>(testInput)
                            : std::get<2>(testInput);

    EXPECT_EQ(func(vec, index), expected);
}

TEST_P(MedianTest, WorksWithAllInputs) {
    auto testInput = GetParam().second;
    auto vec = testInput.first;
    auto expected = testInput.second;

    EXPECT_EQ(alg::getMedian(vec), expected);
}

INSTANTIATE_TEST_SUITE_P(ExtremeValueTestsGenerator, ExtremeValueTest,
    Combine(ValuesIn(extremeValFunctions), ValuesIn(testInputMinMax)),
    [](const testing::TestParamInfo<ExtremeValueTest::ParamType>& info) {
        return std::get<0>(info.param).first + "_" + std::get<1>(info.param).first;
    });

INSTANTIATE_TEST_SUITE_P(MinMaxTestsGenerator, MinMaxTest,
    Combine(ValuesIn(minMaxFunctions), ValuesIn(testInputMinMax)),
    [](const testing::TestParamInfo<MinMaxTest::ParamType>& info) {
        return std::get<0>(info.param).first + "_" + std::get<1>(info.param).first;
    });

INSTANTIATE_TEST_SUITE_P(SelectionTestsGenerator, SelectionTest,
    Combine(ValuesIn(selectionFunctions), ValuesIn(testInputSelection)),
    [](const testing::TestParamInfo<SelectionTest::ParamType>& info) {
        return std::get<0>(info.param).first + "_" + std::get<1>(info.param).first;
    });

INSTANTIATE_TEST_SUITE_P(MedianTestsGenerator, MedianTest,
    ::testing::ValuesIn(testInputMedian),
    [](const testing::TestParamInfo<MedianTest::ParamType>& info) {
        return info.param.first;
    });
