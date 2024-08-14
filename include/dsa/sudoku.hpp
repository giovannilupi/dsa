#pragma once

#include <cmath>
#include <unordered_set>
#include <vector>
#include "alg_concepts.hpp"
#include "common.hpp"

namespace alg {

/**
 * Checks if a sudoku board is valid.
 * Empty cells are marked with the filler value.
 * Assumes that the board is a square matrix.
 * Assumes that the filler value is not a valid value for a cell.
 * Calling this violating the assumptions causes undefined behavior.
 */
template <Matrix T>
    requires std::equality_comparable<typename T::value_type::value_type>
bool isValidSudoku(T &board, typename T::value_type::value_type filler) {
    // Check if the board is a square
    auto sz = board.size();
    decltype(sz) box_sz = static_cast<decltype(sz)>(sqrt(sz));
    if (box_sz * box_sz != sz) throw std::invalid_argument("Board is not a square");
    // Define groups of rows, columns and boxes
    using VType = typename T::value_type::value_type;
    using TSet = std::unordered_set<VType>;
    std::vector<TSet> rows(sz);
    std::vector<TSet> cols(sz);
    std::vector<TSet> boxes(sz);
    // Assign each element to the right groups
    for (index row = 0; row != sz; ++row) {
        for (index col = 0; col != sz; ++col) {
            if (board[row][col] == filler) continue;
            index box = box_sz * (row / box_sz) + col / box_sz;
            VType curr = board[row][col];
            if (!rows[row].insert(curr).second || 
                !cols[col].insert(curr).second ||
                !boxes[box].insert(curr).second) return false;
        }
    }
    return true;
}

} // namespace alg