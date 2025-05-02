#include "gmock/gmock.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <ranges>
#include "permutations.hpp"

namespace alg {
namespace {

using ::testing::TestWithParam;
using ::testing::UnorderedElementsAreArray;
using ::testing::ValuesIn;

/**
 * Generates all permutations of a vector.
 */
std::vector<std::vector<int>> genPermutations(std::vector<int> vec) {
    std::vector<std::vector<int>> permutations;
    std::ranges::sort(vec);
    do {
        permutations.push_back(vec);
    } while (std::next_permutation(vec.begin(), vec.end()));
    return permutations;
}

struct TestPermutationsInput {
    std::vector<int> input;
    std::vector<std::vector<int>> expected;
};

const std::map<std::string, TestPermutationsInput> testCases = {
    {"EmptyInput", {
        .input = {}, 
        .expected = genPermutations({})}},
    {"SingleElement", {
        .input = {1}, 
        .expected = genPermutations({1})}},
    {"TwoElements", {
        .input = {1, 2}, 
        .expected = genPermutations({1, 2})}},
    {"ThreeElements", {
        .input = {1, 2, 3}, 
        .expected = genPermutations({1, 2, 3})}},
    // next_permutation from std lib skips duplicate elements, so can't be used for this case
    {"DuplicateElements", {
        .input = {1, 1}, 
        .expected = {{1, 1}, {1, 1}}}},
    {"FourElements", {
        .input = {1, 2, 3, 4},
        .expected = genPermutations({1, 2, 3, 4})}},
};

using PermutationsTestParamT = decltype(testCases)::value_type;

class PermutationsTest : public TestWithParam<PermutationsTestParamT> {};

TEST_P(PermutationsTest, TestPermutations) {
    const auto& [vec, expected] = GetParam().second;
    // Ignore the order of the elements in the comparison
    EXPECT_THAT(permutations(vec), UnorderedElementsAreArray(expected));
}

INSTANTIATE_TEST_SUITE_P(PermutationsTestsGenerator, PermutationsTest,
    ValuesIn(testCases),
    [](const auto& info) { return info.param.first; });

}  // namespace
}  // namespace alg
