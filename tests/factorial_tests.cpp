#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <functional> 
#include "factorial.hpp"

using FactFunc = std::function<uint64_t(uint64_t)>;

std::map<std::string, FactFunc> factorialFunctions = {
    {"FactorialRecursive", alg::factRec},
    {"FactorialTailRecursive", alg::factTailRec},
    {"FactorialIterative", alg::factIter},
};

class FactorialTest : 
    public ::testing::TestWithParam<std::pair<const std::string, FactFunc>>
{};

TEST_P(FactorialTest, WorksWithAllInputs) {
    auto factorialFunction = GetParam().second;
    
    EXPECT_EQ(factorialFunction(0), 1);
    EXPECT_EQ(factorialFunction(1), 1);
    EXPECT_EQ(factorialFunction(2), 2);
    EXPECT_EQ(factorialFunction(3), 6);
    EXPECT_EQ(factorialFunction(4), 24);
    EXPECT_EQ(factorialFunction(5), 120);
    EXPECT_EQ(factorialFunction(6), 720);
    EXPECT_EQ(factorialFunction(7), 5040);
    EXPECT_EQ(factorialFunction(8), 40320);
    EXPECT_EQ(factorialFunction(9), 362880);
    EXPECT_EQ(factorialFunction(10), 3628800);
    EXPECT_EQ(factorialFunction(11), 39916800);
    EXPECT_EQ(factorialFunction(12), 479001600);
}

INSTANTIATE_TEST_SUITE_P(FactorialTestsGenerator, FactorialTest, 
        ::testing::ValuesIn(factorialFunctions),
        [](const testing::TestParamInfo<FactorialTest::ParamType> &info){
            return info.param.first;
        }); 
