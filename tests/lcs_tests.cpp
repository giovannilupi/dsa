#include <gtest/gtest.h>
#include <format>
#include <string>
#include "common.hpp"
#include "lcs.hpp"

using ::testing::TestWithParam;
using ::testing::Combine;
using ::testing::ValuesIn;

namespace {

using LCSFunc = std::function<alg::index(const std::string&, const std::string&)>;

struct LCSTestInput {
    const std::string s1;
    const std::string s2;
    const alg::index expected;
};

const std::map<std::string, LCSFunc> lcsFunctions = {
    {"LCSRecursive", alg::lcsRec<std::string>},
    {"LCSMemoization", alg::lcsMemoized<std::string>},
    {"LCSTabulated", alg::lcsDP<std::string>},
    {"LCSTabulatedOptimized", alg::lcsDPOptimized<std::string>},
};

const std::map<std::string, LCSTestInput> testStrings = {
    {"EmptyStrings", {"", "" , 0}},
    {"OneEmptyString", {"ABCDE", "", 0}},
    {"CompletelyDifferent", {"ABC", "DEF", 0}},
    {"SingleCharacterMatch", {"A", "A", 1}},
    {"SingleCharacterNoMatch", {"A", "B", 0}},
    {"Simple", {"abcba", "abcbcba", 5}},
    {"CaseSensitiveMatch", {"abc", "ABC", 0}},
    {"RepeatedCharacters", {"AAAA", "AA", 2}},
    {"NonContiguousMatch", {"axbycz", "abc", 3}},
    {"DifferentRepeatedCharacters", {"AABBAABB", "ABAB", 4}},
    {"IdenticalStrings", {"SAMESTRING", "SAMESTRING", 10}},
};

} // namespace

using LCSTestParamT = std::tuple<decltype(lcsFunctions)::value_type, decltype(testStrings)::value_type>;

class LCSTest : public ::testing::TestWithParam<LCSTestParamT> {};

TEST_P(LCSTest, WorksWithAllInputs) {
    // Get the parameters for the current test case
    const auto& param = GetParam();
    const auto& lcsFunc = std::get<0>(param).second;
    const auto& [s1, s2, expected] = std::get<1>(param).second;

    EXPECT_EQ(lcsFunc(s1, s2), expected);
}

INSTANTIATE_TEST_SUITE_P(LCSTestsGenerator, LCSTest,
    Combine(ValuesIn(lcsFunctions), ValuesIn(testStrings)),
    [](const auto& info) { 
        return std::format("{}_{}", std::get<0>(info.param).first, std::get<1>(info.param).first); 
    });
