#include <gtest/gtest.h>
#include <string>
#include "common.hpp"
#include "lcs.hpp"

using LCSFunc = std::function<alg::index(const std::string&, const std::string&)>;
using ::testing::Combine;
using ::testing::ValuesIn;

static std::map<std::string, LCSFunc> lcsFunctions = {
    {"LCSRecursive", alg::lcsRec<std::string>},
    {"LCSMemoization", alg::lcsMemoized<std::string>},
    {"LCSTabulated", alg::lcsDP<std::string>},
    {"LCSTabulatedOptimized", alg::lcsDPOptimized<std::string>},
};

static std::map<std::string, std::pair<std::pair<std::string, std::string>, alg::index>> testStrings = {
    {"EmptyStrings", {{ "", "" }, 0}},
    {"OneEmptyString", {{ "ABCDE", "" }, 0}},
    {"CompletelyDifferent", {{ "ABC", "DEF" }, 0}},
    {"SingleCharacterMatch", {{ "A", "A" }, 1}},
    {"SingleCharacterNoMatch", {{ "A", "B" }, 0}},
    {"Simple", {{ "abcba", "abcbcba" }, 5}},
    {"CaseSensitiveMatch", {{ "abc", "ABC" }, 0}},
    {"RepeatedCharacters", {{ "AAAA", "AA" }, 2}},
    {"NonContiguousMatch", {{ "axbycz", "abc" }, 3}},
    {"DifferentRepeatedCharacters", {{ "AABBAABB", "ABAB" }, 4}},
    {"IdenticalStrings", {{ "SAMESTRING", "SAMESTRING" }, 10}},
};

class LCSTest :
    public ::testing::TestWithParam<std::tuple<
    std::pair<const std::string, LCSFunc>,
    std::pair<const std::string, std::pair<std::pair<std::string, std::string>, alg::index>>>>
{};

TEST_P(LCSTest, WorksWithAllInputs) {
    // Get the parameters for the current test case
    auto param = GetParam();
    auto lcsFunc = std::get<0>(param).second;
    auto strings = std::get<1>(param).second;
    auto s1 = strings.first.first;
    auto s2 = strings.first.second;
    auto expected = strings.second;
    
    EXPECT_EQ(lcsFunc(s1, s2), expected);
}


INSTANTIATE_TEST_SUITE_P(LCSTestsGenerator, LCSTest,
    Combine(ValuesIn(lcsFunctions), ValuesIn(testStrings)),
    [](const testing::TestParamInfo<LCSTest::ParamType>& info) {
        return std::get<0>(info.param).first + "_" + std::get<1>(info.param).first;
    });
