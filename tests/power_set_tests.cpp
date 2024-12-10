#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <format>
#include <functional>
#include "power_set.hpp"

using ::testing::TestWithParam;
using ::testing::Combine;
using ::testing::ValuesIn;

namespace {

using PowerSetFunc = std::function<std::vector<std::vector<int>>(std::vector<int>)>;

const std::map<std::string, PowerSetFunc> powerSetFunctions = {
    {"PowerSetIter", alg::powerSetIter<std::vector<int>>},
    {"PowerSetRec", alg::powerSetDFS<std::vector<int>>},
    {"PowerSetLib", alg::powerSetBacktrack<std::vector<int>>}
};

struct TestPowerSetInput {
    const std::vector<int> input;
    const std::vector<std::vector<int>> expected;
};

const std::map<std::string, TestPowerSetInput> testCases = {
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

} // namespace

using PowerSetParamT = std::tuple<decltype(powerSetFunctions)::value_type, decltype(testCases)::value_type>;

class PowerSetTest : public TestWithParam<PowerSetParamT> {};

TEST_P(PowerSetTest, TestPowerSet) {
    const auto& param = GetParam();
    const auto& powerSetFunc = std::get<0>(param).second;
    const auto& input = std::get<1>(param).second.input;
    auto expected = std::get<1>(param).second.expected;
    auto res = powerSetFunc(input);

    // Ignore the order of the elements in the comparison
    for (auto& subset : expected)
        std::sort(subset.begin(), subset.end());
    for (auto& subset : res)
        std::sort(subset.begin(), subset.end());
    EXPECT_THAT(res, ::testing::UnorderedElementsAreArray(expected));
}

INSTANTIATE_TEST_SUITE_P(PowerSetTestsGenerator, PowerSetTest,
    Combine(ValuesIn(powerSetFunctions), ValuesIn(testCases)),
    [](const auto& info) { 
        return std::format("{}_{}", std::get<0>(info.param).first, std::get<1>(info.param).first); 
    });
