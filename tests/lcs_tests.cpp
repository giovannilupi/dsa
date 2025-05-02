#include <gtest/gtest.h>
#include <format>
#include <string>
#include "common.hpp"
#include "lcs.hpp"

namespace alg {
namespace {

using ::testing::TestWithParam;
using ::testing::Combine;
using ::testing::ValuesIn;

using LCSFunc = std::function<index(const std::string&, const std::string&)>;

struct LCSTestInput {
    std::string s1;
    std::string s2;
    index expected;
};

const std::map<std::string, LCSFunc> lcsFunctions = {
    {"LCSRecursive", lcsRec<std::string>},
    {"LCSMemoization", lcsMemoized<std::string>},
    {"LCSTabulated", lcsDP<std::string>},
    {"LCSTabulatedOptimized", lcsDPOptimized<std::string>},
};

const std::map<std::string, LCSTestInput> testStrings = {
    {"EmptyStrings", {
        .s1 = "", 
        .s2 = "" , 
        .expected = 0}},
    {"OneEmptyString", {
        .s1 = "ABCDE", 
        .s2 = "", 
        .expected = 0}},
    {"CompletelyDifferent", {
        .s1 = "ABC", 
        .s2 = "DEF", 
        .expected = 0}},
    {"SingleCharacterMatch", {
        .s1 = "A", 
        .s2 = "A", 
        .expected = 1}},
    {"SingleCharacterNoMatch", {
        .s1 = "A", 
        .s2 = "B", 
        .expected = 0}},
    {"Simple", {
        .s1 = "abcba", 
        .s2 = "abcbcba", 
        .expected = 5}},
    {"CaseSensitiveMatch", {
        .s1 = "abc", 
        .s2 = "ABC", 
        .expected = 0}},
    {"RepeatedCharacters", {
        .s1 = "AAAA", 
        .s2 = "AA", 
        .expected = 2}},
    {"NonContiguousMatch", {
        .s1 = "axbycz", 
        .s2 = "abc", 
        .expected = 3}},
    {"DifferentRepeatedCharacters", {
        .s1 = "AABBAABB", 
        .s2 = "ABAB", 
        .expected = 4}},
    {"IdenticalStrings", {
        .s1 = "SAMESTRING", 
        .s2 = "SAMESTRING", 
        .expected = 10}},
};

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

}  // namespace
}  // namespace alg
