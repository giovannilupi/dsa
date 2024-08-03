#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <functional> 
#include "fibonacci.hpp"

using FibFunc = std::function<uint64_t(uint64_t)>;

std::map<std::string, FibFunc> fibonacciFunctions = {
    {"FibonacciRecursive", alg::fibRec},
    {"FibonacciTailRecursive", alg::fibTailRec},
    {"FibonacciIterative", alg::fibIter},
};

class FibonacciTest : 
    public ::testing::TestWithParam<std::pair<const std::string, FibFunc>> 
{};

TEST_P(FibonacciTest, WorksWithAllInputs) {
    auto fibFunc = GetParam().second;

    EXPECT_EQ(fibFunc(0), 0);
    EXPECT_EQ(fibFunc(1), 1);
    EXPECT_EQ(fibFunc(2), 1);
    EXPECT_EQ(fibFunc(3), 2);
    EXPECT_EQ(fibFunc(4), 3);
    EXPECT_EQ(fibFunc(5), 5);
    EXPECT_EQ(fibFunc(6), 8);
    EXPECT_EQ(fibFunc(7), 13);
    EXPECT_EQ(fibFunc(8), 21);
    EXPECT_EQ(fibFunc(9), 34);
    EXPECT_EQ(fibFunc(10), 55);
    EXPECT_EQ(fibFunc(11), 89);
    EXPECT_EQ(fibFunc(12), 144);
    EXPECT_EQ(fibFunc(13), 233);
    EXPECT_EQ(fibFunc(14), 377);
    EXPECT_EQ(fibFunc(15), 610);
    EXPECT_EQ(fibFunc(16), 987);
    EXPECT_EQ(fibFunc(17), 1597);
    EXPECT_EQ(fibFunc(18), 2584);
    EXPECT_EQ(fibFunc(19), 4181);
    EXPECT_EQ(fibFunc(20), 6765);
}

INSTANTIATE_TEST_SUITE_P(FibonacciTestsGenerator, FibonacciTest, 
    ::testing::ValuesIn(fibonacciFunctions),
    [](const testing::TestParamInfo<FibonacciTest::ParamType> &info){
        return info.param.first;
    });
