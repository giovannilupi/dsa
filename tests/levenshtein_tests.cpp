#include <gtest/gtest.h>
#include <string>
#include "common.hpp"
#include "levenshtein.hpp"

using ::testing::TestWithParam;
using ::testing::ValuesIn;

namespace {

struct LevenshteinTestCase {
    const std::string s1;
    const std::string s2;
    const alg::index expected;
};

const std::map<std::string, LevenshteinTestCase> testCases = {
    {"EmptyStrings", {"", "", 0}},
    {"OneEmptyString", {"", "hello", 5}},
    {"IdenticalStrings", {"test", "test", 0}},
    {"SingleCharDiff", {"cat", "car", 1}},
    {"CompletelyDifferent", {"abc", "xyz", 3}},
    {"Insertions", {"kitten", "sitting", 3}},
    {"Deletions", {"flaw", "lawn", 2}},
    {"MixedOperations", {"intention", "execution", 5}},
    {"CaseSensitivity", {"Case", "case", 1}},
    {"Prefix", {"prefix", "prefixes", 2}},
    {"Suffix", {"reading", "leading", 1}},
    {"Substring", {"abc", "abcd", 1}},
    {"LongStrings", {"abcdefghijklmnopqrstuvwxyz", "abcfghijklmnopqrstuvwxyz", 2}},
};

} // namespace

using LevenshteinTestParamT = decltype(testCases)::value_type;

class LevenshteinTest : public TestWithParam<LevenshteinTestParamT> {};

TEST_P(LevenshteinTest, WorksWithAllInputs) {
    // Get the parameters for the current test case
    auto param = GetParam();
    const auto& [s1, s2, expected] = GetParam().second;

    EXPECT_EQ(alg::levenDist(s1, s2), expected);
}

INSTANTIATE_TEST_SUITE_P(LevenshteinTestsGenerator, LevenshteinTest,
    ValuesIn(testCases),
    [](const auto& info) { return info.param.first; });
