#pragma once

#include "alg_concepts.hpp"
#include "common.hpp"

namespace alg {

namespace detail {

/**
 * Helper function for divide and conquer implementation of matrix multiplication.
 */
template <Matrix MatT, std::integral TSize>
void matrixMultDivConqHelper(const MatT &a, const MatT &b, MatT &c, 
                             index rowA, index colA, 
                             index rowB, index colB, 
                             index rowC, index colC, 
                             TSize n) {
    if (n == 1) c[rowC][colC] += a[rowA][colA] * b[rowB][colB];
    else {
        index nsize = n / 2;
        // C11 += A11 * B11
        matrixMultDivConqHelper(a, b, c, rowA, colA, rowB, colB, rowC, colC, nsize);
        // C12 += A11 * B12
        matrixMultDivConqHelper(a, b, c, rowA, colA, rowB, colB + nsize, rowC, colC + nsize, nsize);
        // C21 += A21 * B11
        matrixMultDivConqHelper(a, b, c, rowA + nsize, colA, rowB, colB, rowC + nsize, colC, nsize);
        // C22 += A21 * B12
        matrixMultDivConqHelper(a, b, c, rowA + nsize, colA, rowB, colB + nsize, rowC + nsize, colC + nsize, nsize);
        // C11 += A12 * B21
        matrixMultDivConqHelper(a, b, c, rowA, colA + nsize, rowB + nsize, colB, rowC, colC, nsize);
        // C12 += A12 * B22
        matrixMultDivConqHelper(a, b, c, rowA, colA + nsize, rowB + nsize, colB + nsize, rowC, colC + nsize, nsize);
        // C21 += A22 * B21
        matrixMultDivConqHelper(a, b, c, rowA + nsize, colA + nsize, rowB + nsize, colB, rowC + nsize, colC, nsize);
        // C22 += A22 * B22
        matrixMultDivConqHelper(a, b, c, rowA + nsize, colA + nsize, rowB + nsize, colB + nsize, rowC + nsize, colC + nsize, nsize);    }
}

/**
 * Adds two matrices of same size and retrurns a new matrix containing the sum.
 */
template <Matrix MatT>
MatT addMatrices(const MatT &a, const MatT &b) {
    index n = a.size();
    MatT result(n, typename MatT::value_type(n));
    for (index i = 0; i != n; ++i) {
        for (index j = 0; j != n; ++j) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
    return result;
}

/**
 * Subtracts two matrices of same size and retrurns a new matrix containing the difference.
 */
template <Matrix MatT>
MatT subMatrices(const MatT &a, const MatT &b) {
    index n = a.size();
    MatT result(n, typename MatT::value_type(n));
    for (index i = 0; i != n; ++i) {
        for (index j = 0; j != n; ++j) {
            result[i][j] = a[i][j] - b[i][j];
        }
    }
    return result;
}

/**
 * Splits the content of a matrix into 4 smaller ones of size n/2.
 */
template <Matrix MatT>
void splitMatrix(const MatT &a, MatT &a11, MatT &a12, MatT &a21, MatT &a22) {
    index nsize = a.size() / 2;
    for (index i = 0; i != nsize; ++i) {
        for (index j = 0; j != nsize; ++j) {
            a11[i][j] = a[i][j];
            a12[i][j] = a[i][j + nsize];
            a21[i][j] = a[i + nsize][j];
            a22[i][j] = a[i + nsize][j + nsize];
        }
    }
}

/**
 * Joins the content of 4 smaller matrices into a single matrix of size n.
 */
template <Matrix MatT>
void joinMatrices(MatT &c, const MatT &c11, const MatT &c12, const MatT &c21, const MatT &c22) {
    index nsize = c.size() / 2;
    for (index i = 0; i != nsize; ++i) {
        for (index j = 0; j != nsize; ++j) {
            c[i][j] = c11[i][j];
            c[i][j + nsize] = c12[i][j];
            c[i + nsize][j] = c21[i][j];
            c[i + nsize][j + nsize] = c22[i][j];
        }
    }
}

} // namespace detail

/**
 * Schoolbook algorithm for multiplication of square matrices.
 * Complexity: O(n^3).
 */
template <Matrix MatT>
void matrixMultiplication(const MatT &a, const MatT &b, MatT &c) {
    index n = a.size();
    for (index i = 0; i != n; ++i) {
        for (index j = 0; j != n; ++j) {
            for (index k = 0; k != n; ++k) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

/**
 * Divide and conquer algorithm for multiplication of square matrices.
 * This only works on matrices of size that are powers of 2.
 * Recurrence relation: T(n) = 8T(n/2) + \Theta(1)
 * Complexity: O(n^3).
 */
template <Matrix MatT>
void matrixMultDivConq(const MatT &a, const MatT &b, MatT &c) {
    index n = a.size();
    index idx = 0;
    detail::matrixMultDivConqHelper(a, b, c, idx, idx, idx, idx, idx, idx, n);
}

/**
 * Strassen's algorithm for multiplication of square matrices.
 * This only works on matrices of size that are powers of 2.
 * Recurrence relation: T(n) = 7T(n/2) + \Theta(n^2)
 * Complexity: O(n^(2.8)).
 */
template <Matrix MatT>
void matrixMultStrassen(const MatT &a, const MatT &b, MatT &c) {
    std::integral auto n = a.size();

    // Base case
    if (n == 1) { c[0][0] += a[0][0] * b[0][0]; return; }
    
    // This is of type index because we need to loop until that value
    index nsize = n / 2;
    
    // Split each matrix into 4 smaller ones
    MatT a11(nsize, typename MatT::value_type(nsize));
    MatT a12(nsize, typename MatT::value_type(nsize));
    MatT a21(nsize, typename MatT::value_type(nsize));
    MatT a22(nsize, typename MatT::value_type(nsize));
    MatT b11(nsize, typename MatT::value_type(nsize));
    MatT b12(nsize, typename MatT::value_type(nsize));
    MatT b21(nsize, typename MatT::value_type(nsize));
    MatT b22(nsize, typename MatT::value_type(nsize));
    detail::splitMatrix(a, a11, a12, a21, a22);
    detail::splitMatrix(b, b11, b12, b21, b22);

    // Helper matrices for computing the product
    MatT m1(nsize, typename MatT::value_type(nsize));
    MatT m2(nsize, typename MatT::value_type(nsize));
    MatT m3(nsize, typename MatT::value_type(nsize));
    MatT m4(nsize, typename MatT::value_type(nsize));
    MatT m5(nsize, typename MatT::value_type(nsize));
    MatT m6(nsize, typename MatT::value_type(nsize));
    MatT m7(nsize, typename MatT::value_type(nsize));
    // m1 = (a11 + a22) * (b11 + b22)
    MatT temp1 = detail::addMatrices(a11, a22);
    MatT temp2 = detail::addMatrices(b11, b22);
    matrixMultStrassen(temp1, temp2, m1);
    // m2 = (a21 + a22) * b11
    temp1 = detail::addMatrices(a21, a22);
    matrixMultStrassen(temp1, b11, m2);
    // m3 = a11 * (b12 - b22)
    temp2 = detail::subMatrices(b12, b22);
    matrixMultStrassen(a11, temp2, m3);
    // m4 = a22 * (b21 - b11)
    temp2 = detail::subMatrices(b21, b11);
    matrixMultStrassen(a22, temp2, m4);
    // m5 = (a11 + a12) * b22
    temp1 = detail::addMatrices(a11, a12);
    matrixMultStrassen(temp1, b22, m5);
    // m6 = (a21 - a11) * (b11 + b12)
    temp1 = detail::subMatrices(a21, a11);
    temp2 = detail::addMatrices(b11, b12);
    matrixMultStrassen(temp1, temp2, m6);
    // m7 = (a12 - a22) * (b21 + b22)
    temp1 = detail::subMatrices(a12, a22);
    temp2 = detail::addMatrices(b21, b22);
    matrixMultStrassen(temp1, temp2, m7);

    // Compute the result C
    MatT c11 = detail::addMatrices(detail::subMatrices(detail::addMatrices(m1, m4), m5), m7);
    MatT c12 = detail::addMatrices(m3, m5);
    MatT c21 = detail::addMatrices(m2, m4);
    MatT c22 = detail::addMatrices(detail::addMatrices(detail::subMatrices(m1, m2), m3), m6);
    detail::joinMatrices(c, c11, c12, c21, c22);
}

} // namespace alg
