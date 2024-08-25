#include <gtest/gtest.h>
#include <string>
#include "common.hpp"
#include "levenshtein.hpp"

static std::map<std::string, std::pair<std::pair<std::string, std::string>, alg::index>> testCases = {
    {"EmptyStrings", {{"", ""}, 0}},
    {"OneEmptyString", {{"", "hello"}, 5}},
    {"IdenticalStrings", {{"test", "test"}, 0}},
    {"SingleCharDiff", {{"cat", "car"}, 1}},
    {"CompletelyDifferent", {{"abc", "xyz"}, 3}},
    {"Insertions", {{"kitten", "sitting"}, 3}},
    {"Deletions", {{"flaw", "lawn"}, 2}},
    {"MixedOperations", {{"intention", "execution"}, 5}},
    {"CaseSensitivity", {{"Case", "case"}, 1}},
    {"Prefix", {{"prefix", "prefixes"}, 2}},
    {"Suffix", {{"reading", "leading"}, 1}},
    {"Substring", {{"abc", "abcd"}, 1}},
    {"LongStrings", {{"abcdefghijklmnopqrstuvwxyz", "abcfghijklmnopqrstuvwxyz"}, 2}},
};

class LevenshteinTest : 
    public ::testing::TestWithParam<std::pair<const std::string, 
    std::pair<std::pair<std::string, std::string>, alg::index>>>
{};

TEST_P(LevenshteinTest, WorksWithAllInputs) {
    // Get the parameters for the current test case
    auto param = GetParam();
    auto string1 = std::get<1>(param).first.first;
    auto string2 = std::get<1>(param).first.second;
    auto expected = std::get<1>(param).second;

    EXPECT_EQ(alg::levenDist(string1, string2), expected);
}

INSTANTIATE_TEST_SUITE_P(LevenshteinTestsGenerator, LevenshteinTest,
    ::testing::ValuesIn(testCases),
    [](const testing::TestParamInfo<LevenshteinTest::ParamType>& info) {
        return std::get<0>(info.param); // Return the description as the test name
    });
