#include <gtest/gtest.h>
#include <vector>
#include <format>
#include "palindrome.hpp"

namespace alg {
namespace {

using ::testing::TestWithParam;
using ::testing::Combine;
using ::testing::ValuesIn;

using PalindromFunc = std::function<bool(const std::vector<int>&)>;

const std::map<std::string, PalindromFunc> palindromeFunctions = {
    {"IsPalindromeRecursive", isPalindromeRec<std::vector<int>>},
    {"IsPalindromeIterative", isPalindromeIter<std::vector<int>>}
};

struct TestPalindromeInput {
    std::vector<int> input;
    bool expected;
};

const std::map<std::string, TestPalindromeInput> testVectors = {
    {"Empty", { 
        .input = {}, 
        .expected = true} },
    {"OneElement", { 
        .input = {1}, 
        .expected = true} },
    {"TwoElementsSame", { 
        .input = {1, 1}, 
        .expected = true} },
    {"TwoElementsDifferent", { 
        .input = {1, 2}, 
        .expected = false} },
    {"OddPalindrome", { 
        .input = {1, 2, 1}, 
        .expected = true} },
    {"EvenPalindrome", { 
        .input = {1, 2, 2, 1}, 
        .expected = true} },
    {"OddNonPalindrome", { 
        .input = {1, 2, 3},
        .expected = false} },
    {"EvenNonPalindrome", {
        .input = {1, 2, 3, 4}, 
        .expected = false} },
    {"AllSame", {
        .input = {1, 1, 1, 1, 1}, 
        .expected = true} },
    {"AllDifferent", {
        .input = {1, 2, 3, 4, 5}, 
        .expected = false} },
    {"LargePalindrome", { 
        .input = {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 
        11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
        21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 
        31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 
        51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 
        61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 
        71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 
        81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 
        91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 
        100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 
        90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 
        80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 
        70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 
        60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 
        50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 
        40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 
        30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 
        20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 
        10, 9, 8, 7, 6, 5, 4, 3, 2, 1 }, 
        .expected = true} }
};

using PalindromeTestParamT = std::tuple<decltype(palindromeFunctions)::value_type, decltype(testVectors)::value_type>;

class PalindromeTest : public TestWithParam<PalindromeTestParamT> {};

TEST_P(PalindromeTest, WorksWithAllInputs) {
    // Get the parameters for the current test case
    const auto& param = GetParam();
    const auto& palindromeFunction = std::get<0>(param).second;
    const auto& [container, expected] = std::get<1>(param).second;
    // Check if the container is a palindrome
    EXPECT_EQ(palindromeFunction(container), expected);
}

INSTANTIATE_TEST_SUITE_P(PalindromeTestsGenerator, PalindromeTest,
    Combine(ValuesIn(palindromeFunctions), ValuesIn(testVectors)),
    [](const auto& info) { 
        return std::format("{}_{}", std::get<0>(info.param).first, std::get<1>(info.param).first); 
    });

}  // namespace
}  // namespace alg
