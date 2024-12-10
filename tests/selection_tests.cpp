#include <gtest/gtest.h>
#include <vector>
#include <format>
#include <list>
#include <regex>
#include "selection.hpp"

using ::testing::TestWithParam;
using ::testing::Combine;
using ::testing::ValuesIn;

namespace {

using ExtremeValFunc = std::function<int(const std::vector<int>&)>;
using MinMaxFunc = std::function<std::pair<int, int>(const std::vector<int>&)>;
using SelectionFunc = std::function<int(std::vector<int>&, int)>;

const std::map<std::string, ExtremeValFunc> extremeValFunctions = {
    {"GetMin", alg::getMin<std::vector<int>>},
    {"GetMax", alg::getMax<std::vector<int>>},
};

const std::map<std::string, MinMaxFunc> minMaxFunctions = {
    {"GetMinMaxNaive", alg::getMinMax<std::vector<int>>},
    {"GetMinMaxOptimal", alg::getMinMaxOpt<std::vector<int>>},
};

const std::map<std::string, SelectionFunc> selectionFunctions = {
    {"SelectMinNaive", alg::selectNthMin<std::vector<int>>},
    {"SelectMaxNaice", alg::selectNthMax<std::vector<int>>},
    {"QuickSelectMin", alg::quickSelectMin<std::vector<int>>},
    {"QuickSelectMax", alg::quickSelectMax<std::vector<int>>},
    {"HeapSelectMin", alg::heapSelectMin<std::vector<int>>},
    {"HeapSelectMax", alg::heapSelectMax<std::vector<int>>},
    {"SelectMinSTL", alg::selectMinModern<std::vector<int>>},
    {"SelectMaxSTL", alg::selectMaxModern<std::vector<int>>},
};

struct TestMinMaxInput {
    const std::vector<int> input;
    const int expectedMin;
    const int expectedMax;
};

const std::map<std::string, TestMinMaxInput> testInputMinMax = {
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

struct TestSelectionInput {
    const std::vector<int> input;
    const std::size_t k;
    const int kth_min;
    const int kth_max;
};

const std::map<std::string, TestSelectionInput> testInputSelection = {
    {"SinglePositive", { {1}, 0, 1, 1 }},
    {"SingleNegative", { {-1}, 0, -1, -1 }},
    {"AllPositive", { {1, 2, 3, 4, 5}, 2, 3, 3 }},
    {"AllNegative", { {-5, -4, -3, -2, -1}, 1, -4, -2 }},
    {"MixedValues", { {1, -1, 2, -2, 3, -3}, 2, -1, 1 }},
    {"LargeValues", { {100, 200, 300, -100, -200}, 1, -100, 200 }},
    {"AllZeros", { {0, 0, 0, 0}, 0, 0, 0 }},
    {"IncreasingSequence", { {1, 3, 5, 7, 9}, 0, 1, 9 }},
    {"DecreasingSequence", { {9, 7, 5, 3, 1}, 0, 1, 9 }},
    {"AlternatingSigns", { {-10, 10, -20, 20, -30, 30}, 3, 10, -10 }},
};

struct TestMedianInput {
    const std::vector<int> input;
    const double expected;
};

const std::map<std::string, TestMedianInput> testInputMedian = {
    {"SingleElement", { {1}, 1.0 }},
    {"TwoElementsEven", { {1, 2}, 1.5 }},
    {"ThreeElementsOdd", { {1, 2, 3}, 2.0 }},
    {"FourElementsEven", { {1, 2, 3, 4}, 2.5 }},
    {"FiveElementsOdd", { {5, 1, 3, 2, 4}, 3.0 }},
    {"AllPositive", { {10, 20, 30, 40, 50}, 30.0 }},
    {"AllNegative", { {-10, -20, -30, -40, -50}, -30.0 }},
    {"MixedValues", { {3, -1, 2, -2, 1}, 1.0 }},
    {"LargeValues", { {100, 200, 300, 400, 500}, 300.0 }},
    {"DecreasingSequence", { {9, 7, 5, 3, 1}, 5.0 }},
    {"IncreasingSequence", { {1, 3, 5, 7, 9}, 5.0 }},
    {"AlternatingSigns", { {10, -10, 20, -20, 30}, 10.0 }},
    {"MultipleSameValues", { {1, 1, 1, 1, 1}, 1.0 }},
};

} // namespace

using ExtremeValTestParamT = std::tuple<decltype(extremeValFunctions)::value_type, decltype(testInputMinMax)::value_type>;

class ExtremeValueTest : public TestWithParam<ExtremeValTestParamT> {};

TEST_P(ExtremeValueTest, WorksWithAllInputs) {
    const auto& param = GetParam();
    const auto& [funcName, func] = std::get<0>(param);
    const auto& [input, expectedMin, expectedMax] = std::get<1>(param).second;
    const auto& expected = std::regex_search(funcName, std::regex("Min"))
                            ? expectedMin
                            : expectedMax;

    EXPECT_EQ(func(input), expected);
}

INSTANTIATE_TEST_SUITE_P(ExtremeValueTestsGenerator, ExtremeValueTest,
    Combine(ValuesIn(extremeValFunctions), ValuesIn(testInputMinMax)),
    [](const auto& info) { 
        return std::format("{}_{}", std::get<0>(info.param).first, std::get<1>(info.param).first); 
    });

using MinMaxTestParamT = std::tuple<decltype(minMaxFunctions)::value_type, decltype(testInputMinMax)::value_type>;

class MinMaxTest : public TestWithParam<MinMaxTestParamT> {};

TEST_P(MinMaxTest, WorksWithAllInputs) {
    const auto& param = GetParam();
    const auto& func = std::get<0>(param).second;
    const auto& [input, expectedMin, expectedMax] = std::get<1>(param).second;
    const decltype(func(input)) expected = {expectedMin, expectedMax};

    EXPECT_EQ(func(input), expected);
}

INSTANTIATE_TEST_SUITE_P(MinMaxTestsGenerator, MinMaxTest,
    Combine(ValuesIn(minMaxFunctions), ValuesIn(testInputMinMax)),
    [](const auto& info) { 
        return std::format("{}_{}", std::get<0>(info.param).first, std::get<1>(info.param).first); 
    });

using SelectionTestParamT = std::tuple<decltype(selectionFunctions)::value_type, decltype(testInputSelection)::value_type>;

class SelectionTest : public TestWithParam<SelectionTestParamT> {};

TEST_P(SelectionTest, WorksWithAllInputs) {
    const auto& param = GetParam();
    const auto& [funcName, func] = std::get<0>(param);
    const auto& testInput = std::get<1>(param).second;
    auto input = testInput.input;
    const auto& k = testInput.k;
    const auto& expected = std::regex_search(funcName, std::regex("Min"))
                            ? testInput.kth_min
                            : testInput.kth_max;

    EXPECT_EQ(func(input, k), expected);
}

INSTANTIATE_TEST_SUITE_P(SelectionTestsGenerator, SelectionTest,
    Combine(ValuesIn(selectionFunctions), ValuesIn(testInputSelection)),
    [](const auto& info) { 
        return std::format("{}_{}", std::get<0>(info.param).first, std::get<1>(info.param).first); 
    });

using MedianTestParamT = decltype(testInputMedian)::value_type;

class MedianTest : public TestWithParam<MedianTestParamT> {};

TEST_P(MedianTest, WorksWithAllInputs) {
    auto testInput = GetParam().second;
    auto vec = testInput.input;
    const auto& expected = testInput.expected;

    EXPECT_EQ(alg::getMedian(vec), expected);
}

INSTANTIATE_TEST_SUITE_P(MedianTestsGenerator, MedianTest,
    ::testing::ValuesIn(testInputMedian),
    [](const auto& info) { return info.param.first; });
