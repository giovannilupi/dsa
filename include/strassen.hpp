#pragma once

#include "alg_concepts.hpp"

namespace alg {

namespace detail {

/**
 * Helper function for divide and conquer implementation of matrix multiplication.
 */
template <Matrix T, std::integral Q>
void matrixMultDivConqHelper(const T &a, const T &b, T &c, Q rowA, Q colA, Q rowB, Q colB, Q rowC, Q colC, Q n) {
    if (n == 1) c[rowC][colC] += a[rowA][colA] * b[rowB][colB];
    else {
        Q nsize = n / 2;
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
template <Matrix T>
T addMatrices(const T &a, const T &b) {
    std::integral auto n = a.size();
    T result(n, typename T::value_type(n));
    for (decltype(n) i = 0; i != n; ++i) {
        for (decltype(n) j = 0; j != n; ++j) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
    return result;
}

/**
 * Subtracts two matrices of same size and retrurns a new matrix containing the difference.
 */
template <Matrix T>
T subMatrices(const T &a, const T &b) {
    std::integral auto n = a.size();
    T result(n, typename T::value_type(n));
    for (decltype(n) i = 0; i != n; ++i) {
        for (decltype(n) j = 0; j != n; ++j) {
            result[i][j] = a[i][j] - b[i][j];
        }
    }
    return result;
}

/**
 * Splits the content of a matrix into 4 smaller ones of size n/2.
 */
template <Matrix T>
void splitMatrix(const T &a, T &a11, T &a12, T &a21, T &a22) {
    std::integral auto nsize = a.size() / 2;
    for (decltype(nsize) i = 0; i != nsize; ++i) {
        for (decltype(nsize) j = 0; j != nsize; ++j) {
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
template <Matrix T>
void joinMatrices(T &c, const T &c11, const T &c12, const T &c21, const T &c22) {
    std::integral auto nsize = c.size() / 2;
    for (decltype(nsize) i = 0; i != nsize; ++i) {
        for (decltype(nsize) j = 0; j != nsize; ++j) {
            c[i][j] = c11[i][j];
            c[i][j + nsize] = c12[i][j];
            c[i + nsize][j] = c21[i][j];
            c[i + nsize][j + nsize] = c22[i][j];
        }
    }
}

/**
 * Helper function for strassen's algorithm.
 */
template <Matrix T>
void matrixMultStrassenHelper(const T &a, const T &b, T &c) {
    std::integral auto n = a.size();

    // Base case
    if (n == 1) { c[0][0] += a[0][0] * b[0][0]; return; }
    
    decltype(n) nsize = n / 2;
    
    // Split each matrix into 4 smaller ones
    T a11(nsize, typename T::value_type(nsize));
    T a12(nsize, typename T::value_type(nsize));
    T a21(nsize, typename T::value_type(nsize));
    T a22(nsize, typename T::value_type(nsize));
    T b11(nsize, typename T::value_type(nsize));
    T b12(nsize, typename T::value_type(nsize));
    T b21(nsize, typename T::value_type(nsize));
    T b22(nsize, typename T::value_type(nsize));
    splitMatrix(a, a11, a12, a21, a22);
    splitMatrix(b, b11, b12, b21, b22);

    // Helper matrices for computing the product
    T m1(nsize, typename T::value_type(nsize));
    T m2(nsize, typename T::value_type(nsize));
    T m3(nsize, typename T::value_type(nsize));
    T m4(nsize, typename T::value_type(nsize));
    T m5(nsize, typename T::value_type(nsize));
    T m6(nsize, typename T::value_type(nsize));
    T m7(nsize, typename T::value_type(nsize));
    // m1 = (a11 + a22) * (b11 + b22)
    T temp1 = addMatrices(a11, a22);
    T temp2 = addMatrices(b11, b22);
    matrixMultStrassenHelper(temp1, temp2, m1);
    // m2 = (a21 + a22) * b11
    temp1 = addMatrices(a21, a22);
    matrixMultStrassenHelper(temp1, b11, m2);
    // m3 = a11 * (b12 - b22)
    temp2 = subMatrices(b12, b22);
    matrixMultStrassenHelper(a11, temp2, m3);
    // m4 = a22 * (b21 - b11)
    temp2 = subMatrices(b21, b11);
    matrixMultStrassenHelper(a22, temp2, m4);
    // m5 = (a11 + a12) * b22
    temp1 = addMatrices(a11, a12);
    matrixMultStrassenHelper(temp1, b22, m5);
    // m6 = (a21 - a11) * (b11 + b12)
    temp1 = subMatrices(a21, a11);
    temp2 = addMatrices(b11, b12);
    matrixMultStrassenHelper(temp1, temp2, m6);
    // m7 = (a12 - a22) * (b21 + b22)
    temp1 = subMatrices(a12, a22);
    temp2 = addMatrices(b21, b22);
    matrixMultStrassenHelper(temp1, temp2, m7);

    // Compute the result C
    T c11 = addMatrices(subMatrices(addMatrices(m1, m4), m5), m7);
    T c12 = addMatrices(m3, m5);
    T c21 = addMatrices(m2, m4);
    T c22 = addMatrices(addMatrices(subMatrices(m1, m2), m3), m6);
    joinMatrices(c, c11, c12, c21, c22);
}

}
/**
 * Schoolbook algorithm for multiplication of square matrices.
 * Complexity: O(n^3).
 */
template <Matrix T>
void matrixMultiplication(const T &a, const T &b, T &c) {
    std::integral auto n = a.size();
    using SzType = decltype(n);
    for (SzType i = 0; i != n; ++i) {
        for (SzType j = 0; j != n; ++j) {
            for (SzType k = 0; k != n; ++k) {
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
template <Matrix T>
void matrixMultDivConq(const T &a, const T &b, T &c) {
    std::integral auto n = a.size();
    if (!n) return;
    decltype(n) idx = 0;
    detail::matrixMultDivConqHelper(a, b, c, idx, idx, idx, idx, idx, idx, n);
}

/**
 * Strassen's algorithm for multiplication of square matrices.
 * This only works on matrices of size that are powers of 2.
 * Recurrence relation: T(n) = 7T(n/2) + \Theta(n^2)
 * Complexity: O(n^(2.8)).
 */
template <Matrix T>
void matrixMultStrassen(const T &a, const T &b, T &c) {
    std::integral auto n = a.size();
    if (!n) return;
    decltype(n) idx = 0;
    detail::matrixMultStrassenHelper(a, b, c);
}

}
