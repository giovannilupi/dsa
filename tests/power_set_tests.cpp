#include <functional>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "power_set.hpp"

using PowerSetFunc = std::function<std::vector<std::vector<int>>(std::vector<int>)>;
using ::testing::Combine;
using ::testing::ValuesIn;

static std::map<std::string, PowerSetFunc> powerSetFunctions = {
    {"PowerSetIter", alg::powerSetIter<std::vector<int>>},
    {"PowerSetRec", alg::powerSetDFS<std::vector<int>>},
    {"PowerSetLib", alg::powerSetBacktrack<std::vector<int>>}
};

static std::map<std::string, std::pair<std::vector<int>, std::vector<std::vector<int>>>> testCases = {
    {"EmptySet", 
        {{}, {{}}
    }},
    {"SingleElement", 
        {{1}, {{}, {1}}}
    },
    {"TwoElements", 
        {{1, 2}, {{}, {1}, {2}, {1, 2}}}
    },
    {"ThreeElements", 
        {{1, 2, 3}, 
         {{}, {1}, {2}, {3}, {1, 2}, {1, 3}, {2, 3}, {1, 2, 3}}}
    },
    // Testing with negative and positive numbers
    {"MixedElements", 
        {{-1, 2, -3}, 
         {{}, {-1}, {2}, {-3}, {-1, 2}, {-1, -3}, {2, -3}, {-1, 2, -3}}}
    },
    {"LargerSet", 
        {{1, 2, 3, 4}, 
         {{}, {1}, {2}, {3}, {4}, {1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, 
          {3, 4}, {1, 2, 3}, {1, 2, 4}, {1, 3, 4}, {2, 3, 4}, {1, 2, 3, 4}}}
    },
    {"RangeOfElements", 
        {{-2, 0, 2}, 
         {{}, {-2}, {0}, {2}, {-2, 0}, {-2, 2}, {0, 2}, {-2, 0, 2}}}
    }
};

class PowerSetTest :
    public ::testing::TestWithParam<std::tuple<
    std::pair<const std::string, PowerSetFunc>,
    std::pair<const std::string, std::pair<std::vector<int>, std::vector<std::vector<int>>>>>>
{};

TEST_P(PowerSetTest, TestPowerSet) {
    auto param = GetParam();
    auto powerSetFunc = std::get<0>(param).second;
    auto testCase = std::get<1>(param).second.first;
    auto expected = std::get<1>(param).second.second;
    auto res = powerSetFunc(testCase);

    // Ignore the order of the elements in the comparison
    for (auto& subset : expected)
        std::sort(subset.begin(), subset.end());
    for (auto& subset : res)
        std::sort(subset.begin(), subset.end());
    EXPECT_THAT(res, ::testing::UnorderedElementsAreArray(expected));
}

INSTANTIATE_TEST_SUITE_P(PowerSetTestsGenerator, PowerSetTest,
    Combine(ValuesIn(powerSetFunctions), ValuesIn(testCases)),
    [](const testing::TestParamInfo<PowerSetTest::ParamType>& info) {
        return std::get<0>(info.param).first + "_" + std::get<1>(info.param).first;
    });