#pragma once

#include <vector>

namespace alg {

namespace detail {

/**
 * Backtracking helper function for the n-queens problem.
 * In prunes all invalid moves and stops when the board is full.
 * We can only have one queen per row, so we recurse once a row is filled.
 * When picking the row position, we can only pick a column or diagonal that has not been used yet.
 */
inline void nQueensBacktrack(int row,
                    std::vector<int>& curr, 
                    std::vector<std::vector<int>>& res,             
                    std::vector<bool>& usedCols, 
                    std::vector<bool>& usedDiag, 
                    std::vector<bool>& usedAntiDiag) {
    if (row == curr.size()) {
        res.push_back(curr);
        return;
    }
    for (int col = 0; col < curr.size(); ++col) {
        // Prune invalid moves
        if (usedCols[col] || 
            // All items in the diagonal have the same row - col difference
            // We add n-1 to shift the result of the subtraction to be in the range [0, n-1]
            usedDiag[row - col + curr.size() - 1] ||
            // All items in the anti-diagonal have the same row + col sum
            usedAntiDiag[row + col]) continue;
        // Mark the current move
        curr[row] = col;
        usedCols[col] = usedDiag[row - col + curr.size() - 1] = usedAntiDiag[row + col] = true;
        nQueensBacktrack(row + 1, curr, res, usedCols, usedDiag, usedAntiDiag);
        // Unmark the current move (backtrack)
        usedCols[col] = usedDiag[row - col + curr.size() - 1] = usedAntiDiag[row + col] = false;
    }
}

} // namespace detail

/**
 * Returns all the possible solutions to the n-queens problem for a board of size n.
 * This uses a backtracking approach.
 * The board is represented as a vector of column positions of each queen.
 * The index represents the Queen's row, and the value represents its column.
 * Example: vec[0] = 1 represents a queen in row 0 and column 1.
 * Complexity: O(n!)
 */
inline std::vector<std::vector<int>> nQueens(int n) {
    if (n <= 0) throw std::invalid_argument("Number of queens must be greater than 0");
    std::vector<std::vector<int>> res;
    // Holds the current state of the board
    std::vector<int> curr(n, 0);
    // Hold whether a column or diagonal is already used
    std::vector<bool> usedCols(n, false);
    std::vector<bool> usedDiag(2 * n - 1, false);
    std::vector<bool> usedAntiDiag(2 * n - 1, false);
    // Backtracking helper function
    detail::nQueensBacktrack(0, curr, res, usedCols, usedDiag, usedAntiDiag);
    return res;
}

} // namespace alg
