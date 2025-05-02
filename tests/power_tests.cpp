#include <gtest/gtest.h>
#include <vector>
#include <functional> 
#include <string>
#include "power.hpp"

namespace alg {
namespace {

using ::testing::TestWithParam;

using PowFunc = std::function<int64_t(int, uint8_t)>;

const std::map<std::string, PowFunc> powerFunctions = {
    {"PowerRecursive", powRec<int>},
    {"PowerTailRecursive", powTailRec<int>},
    {"PowerIterative", powIter<int>},
    {"PowerSquaring", powSquaring<int>},
    {"PowerSquaringRecursive", powSquaringRec<int>},
    {"PowerSquaringTailRec", powSquaringTailRec<int>},
};

using PowerTestParamT = decltype(powerFunctions)::value_type;

class PowerTest : public TestWithParam<PowerTestParamT> {};

TEST_P(PowerTest, WorksWithZeroBase) {
    const auto& powerFunction = GetParam().second;

    EXPECT_EQ(powerFunction(0, 0), 1);
    EXPECT_EQ(powerFunction(0, 1), 0);
    EXPECT_EQ(powerFunction(0, 3), 0);
    EXPECT_EQ(powerFunction(0, 100), 0);
}

TEST_P(PowerTest, WorksWithOneBase) {
    const auto& powerFunction = GetParam().second;

    EXPECT_EQ(powerFunction(1, 0), 1);
    EXPECT_EQ(powerFunction(1, 1), 1);
    EXPECT_EQ(powerFunction(1, 3), 1);
    EXPECT_EQ(powerFunction(1, 100), 1);
}

TEST_P(PowerTest, WorksWithNegativeBase) {
    const auto& powerFunction = GetParam().second;

    EXPECT_EQ(powerFunction(-1, 100), 1);
    EXPECT_EQ(powerFunction(-2, 0), 1);
    EXPECT_EQ(powerFunction(-2, 1), -2);
    EXPECT_EQ(powerFunction(-2, 2), 4);
    EXPECT_EQ(powerFunction(-2, 3), -8);
    EXPECT_EQ(powerFunction(-5, 5), -3125);
    EXPECT_EQ(powerFunction(-5, 6), 15625);
}

TEST_P(PowerTest, WorksWithRandomInputs) {
    const auto& powerFunction = GetParam().second;

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
    [](const auto& info) { return info.param.first; });

}  // namespace
}  // namespace alg
