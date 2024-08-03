#include <gtest/gtest.h>
#include <vector>
#include <functional> 
#include <string>
#include "power.hpp"

using PowFunc = std::function<int64_t(int, uint8_t)>;

std::map<std::string, PowFunc> powerFunctions = {
    {"PowerRecursive", alg::powRec<int>},
    {"PowerTailRecursive", alg::powTailRec<int>},
    {"PowerIterative", alg::powIter<int>},
    {"PowerSquaring", alg::powSquaring<int>},
    {"PowerSquaringRecursive", alg::powSquaringRec<int>},
    {"PowerSquaringTailRec", alg::powSquaringTailRec<int>},
};

class PowerTest : 
    public ::testing::TestWithParam<std::pair<const std::string, PowFunc>> 
{};

TEST_P(PowerTest, WorksWithZeroBase) {
    auto powerFunction = GetParam().second;

    EXPECT_EQ(powerFunction(0, 0), 1);
    EXPECT_EQ(powerFunction(0, 1), 0);
    EXPECT_EQ(powerFunction(0, 3), 0);
    EXPECT_EQ(powerFunction(0, 100), 0);
}

TEST_P(PowerTest, WorksWithOneBase) {
    auto powerFunction = GetParam().second;

    EXPECT_EQ(powerFunction(1, 0), 1);
    EXPECT_EQ(powerFunction(1, 1), 1);
    EXPECT_EQ(powerFunction(1, 3), 1);
    EXPECT_EQ(powerFunction(1, 100), 1);
}

TEST_P(PowerTest, WorksWithNegativeBase) {
    auto powerFunction = GetParam().second;

    EXPECT_EQ(powerFunction(-1, 100), 1);
    EXPECT_EQ(powerFunction(-2, 0), 1);
    EXPECT_EQ(powerFunction(-2, 1), -2);
    EXPECT_EQ(powerFunction(-2, 2), 4);
    EXPECT_EQ(powerFunction(-2, 3), -8);
    EXPECT_EQ(powerFunction(-5, 5), -3125);
    EXPECT_EQ(powerFunction(-5, 6), 15625);
}

TEST_P(PowerTest, WorksWithRandomInputs) {
    auto powerFunction = GetParam().second;

    EXPECT_EQ(powerFunction(333, 2), 110889);
    EXPECT_EQ(powerFunction(2, 10), 1024);
    EXPECT_EQ(powerFunction(5, 5), 3125);
    EXPECT_EQ(powerFunction(7, 3), 343);
    EXPECT_EQ(powerFunction(1, 100), 1);
    EXPECT_EQ(powerFunction(1, 0), 1);
    EXPECT_EQ(powerFunction(0, 5), 0);
    EXPECT_EQ(powerFunction(0, 10), 0);
    EXPECT_EQ(powerFunction(2, 20), 1048576);
    EXPECT_EQ(powerFunction(10, 6), 1000000);
}

INSTANTIATE_TEST_SUITE_P(PowerTestsGenerator, PowerTest,
    ::testing::ValuesIn(powerFunctions),
    [](const testing::TestParamInfo<PowerTest::ParamType>& info) {
        return info.param.first;
    });
