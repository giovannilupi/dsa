#include <gtest/gtest.h>
#include "sudoku.hpp"

namespace alg {
namespace {

using ::testing::TestWithParam;
using ::testing::ValuesIn;

using SudokuBoard = std::vector<std::vector<char>>;

const std::map<std::string, std::pair<SudokuBoard, bool>> testBoards = {
    {"Valid9x9", {{
        {'5', '3', '.', '.', '7', '.', '.', '.', '.'},
        {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
        {'.', '9', '8', '.', '.', '.', '.', '6', '.'},
        {'8', '.', '.', '.', '6', '.', '.', '.', '3'},
        {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
        {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
        {'.', '6', '.', '.', '.', '.', '2', '8', '.'},
        {'.', '.', '.', '4', '1', '9', '.', '.', '5'},
        {'.', '.', '.', '.', '8', '.', '.', '7', '9'}},
        true
    }},
    // Last row contains a duplicate 8
    {"InvalidRow9x9", {{
        {'5', '3', '.', '.', '7', '.', '.', '.', '.'},
        {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
        {'.', '9', '8', '.', '.', '.', '.', '6', '.'},
        {'8', '.', '.', '.', '6', '.', '.', '.', '3'},
        {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
        {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
        {'.', '6', '.', '.', '.', '.', '2', '8', '.'},
        {'.', '.', '.', '4', '1', '9', '.', '.', '5'},
        {'.', '.', '.', '.', '8', '.', '.', '7', '8'}},
        false
    }},
    // Third column contains a duplicate 8
    {"InvalidColumn9x9", {{
        {'5', '3', '.', '.', '7', '.', '.', '.', '.'},
        {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
        {'.', '9', '8', '.', '.', '.', '.', '6', '.'},
        {'8', '.', '.', '.', '6', '.', '.', '.', '3'},
        {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
        {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
        {'.', '6', '.', '.', '.', '.', '2', '8', '.'},
        {'.', '.', '8', '4', '1', '9', '.', '.', '5'},
        {'.', '.', '.', '.', '8', '.', '.', '7', '9'}},
        false
    }},
    // Bottom left box contains a duplicate 6
    {"InvalidBox9x9", {{
        {'5', '3', '.', '.', '7', '.', '.', '.', '.'},
        {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
        {'.', '9', '8', '.', '.', '.', '.', '6', '.'},
        {'8', '.', '.', '.', '6', '.', '.', '.', '3'},
        {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
        {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
        {'.', '6', '.', '.', '.', '.', '2', '8', '.'},
        {'.', '.', '.', '4', '1', '9', '.', '.', '5'},
        {'.', '.', '6', '.', '8', '.', '.', '7', '9'}},
        false
    }},
    {"EmptyBoard", {{
        {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.'}},
        true
    }},
    // Multiple violations
    {"FullyFilledInvalid", {{
        {'5', '3', '5', '2', '7', '8', '9', '1', '4'},
        {'6', '8', '2', '1', '9', '5', '3', '7', '6'},
        {'1', '9', '8', '3', '4', '6', '5', '2', '7'},
        {'8', '5', '9', '7', '6', '4', '2', '3', '1'},
        {'4', '2', '6', '8', '5', '3', '7', '9', '2'},
        {'7', '1', '3', '9', '2', '5', '6', '8', '4'},
        {'9', '6', '1', '5', '3', '7', '8', '4', '2'},
        {'2', '8', '7', '4', '1', '9', '5', '6', '3'},
        {'3', '4', '5', '6', '8', '2', '1', '7', '9'}},
        false
    }}
};

using SudokuTestParamT = decltype(testBoards)::value_type;

class SudokuTest : public TestWithParam<SudokuTestParamT> {};

TEST_P(SudokuTest, WorksWithAllInputs) {
    // Get the parameters for the current test case
    const auto& [board, expected] = GetParam().second;
    // Check if the board is valid
    EXPECT_EQ(isValidSudoku(board, '.'), expected);
}

INSTANTIATE_TEST_SUITE_P(SudokuTestsGenerator, SudokuTest,
    ValuesIn(testBoards),
    [](const auto& info) { return info.param.first; });

}  // namespace alg
}  // namespace tests
