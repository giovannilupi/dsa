#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include "permutations.hpp"

using ::testing::TestWithParam;

namespace {

auto genPermutations = [](std::vector<int> vec) {
    std::vector<std::vector<int>> permutations;
    std::sort(vec.begin(), vec.end());
    do {
        permutations.push_back(vec);
    } while (std::next_permutation(vec.begin(), vec.end()));
    return permutations;
};

struct TestPermutationsInput {
    const std::vector<int> input;
    const std::vector<std::vector<int>> expected;
};

const std::map<std::string, TestPermutationsInput> testCases = {
    {"EmptyInput", {{}, genPermutations({})}},
    {"SingleElement", {{1}, genPermutations({1})}},
    {"TwoElements", {{1, 2}, genPermutations({1, 2})}},
    {"ThreeElements", {{1, 2, 3}, genPermutations({1, 2, 3})}},
    // next_permutation from std lib skips duplicate elements, so can't be used for this case
    {"DuplicateElements", {{1, 1}, {{1, 1}, {1, 1}}}},
    {"FourElements", {{1, 2, 3, 4}, genPermutations({1, 2, 3, 4})}},
};

} // namespace

using PermutationsTestParamT = decltype(testCases)::value_type;

class PermutationsTest : public TestWithParam<PermutationsTestParamT> {};

TEST_P(PermutationsTest, TestPermutations) {
    const auto& [vec, expected] = GetParam().second;
    // Ignore the order of the elements in the comparison
    EXPECT_THAT(alg::permutations(vec), ::testing::UnorderedElementsAreArray(expected));
}

INSTANTIATE_TEST_SUITE_P(PermutationsTestsGenerator, PermutationsTest,
    ::testing::ValuesIn(testCases),
    [](const auto& info) { return info.param.first; });
