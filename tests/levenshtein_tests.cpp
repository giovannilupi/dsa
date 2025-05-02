#include <gtest/gtest.h>
#include <string>
#include "common.hpp"
#include "levenshtein.hpp"

namespace alg {
namespace {

using ::testing::TestWithParam;
using ::testing::ValuesIn;

struct LevenshteinTestCase {
    std::string s1;
    std::string s2;
    index expected;
};

const std::map<std::string, LevenshteinTestCase> testCases = {
    {"EmptyStrings", {
        .s1 = "", 
        .s2 = "", 
        .expected = 0}},
    {"OneEmptyString", {
        .s1 = "", 
        .s2 ="hello", 
        .expected = 5}},
    {"IdenticalStrings", {
        .s1 ="test", 
        .s2 ="test", 
        .expected = 0}},
    {"SingleCharDiff", {
        .s1 = "cat",
        .s2 = "car", 
        .expected = 1}},
    {"CompletelyDifferent", {
        .s1 = "abc", 
        .s2 = "xyz", 
        .expected = 3}},
    {"Insertions", {
        .s1 = "kitten", 
        .s2 = "sitting", 
        .expected = 3}},
    {"Deletions", {
        .s1 = "flaw", 
        .s2 = "lawn",
        .expected = 2}},
    {"MixedOperations", {
        .s1 = "intention", 
        .s2 = "execution", 
        .expected = 5}},
    {"CaseSensitivity", {
        .s1 = "Case", 
        .s2 = "case", 
        .expected = 1}},
    {"Prefix", {
        .s1 = "prefix", 
        .s2 = "prefixes", 
        .expected = 2}},
    {"Suffix", {
        .s1 = "reading", 
        .s2 = "leading", 
        .expected = 1}},
    {"Substring", {
        .s1 = "abc", 
        .s2 = "abcd", 
        .expected = 1}},
    {"LongStrings", {
        .s1 = "abcdefghijklmnopqrstuvwxyz", 
        .s2 = "abcfghijklmnopqrstuvwxyz", 
        .expected = 2}},
};

using LevenshteinTestParamT = decltype(testCases)::value_type;

class LevenshteinTest : public TestWithParam<LevenshteinTestParamT> {};

TEST_P(LevenshteinTest, WorksWithAllInputs) {
    // Get the parameters for the current test case
    auto param = GetParam();
    const auto& [s1, s2, expected] = GetParam().second;

    EXPECT_EQ(levenDist(s1, s2), expected);
}

INSTANTIATE_TEST_SUITE_P(LevenshteinTestsGenerator, LevenshteinTest,
    ValuesIn(testCases),
    [](const auto& info) { return info.param.first; });

}  // namespace
}  // namespace alg
