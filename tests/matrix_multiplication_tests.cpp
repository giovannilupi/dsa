#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <functional> 
#include "matrix_multiplication.hpp"

using Matrix = std::vector<std::vector<int>>;
using MatMultFunc = std::function<void(Matrix&, Matrix&, Matrix&)>;
using ::testing::Combine;
using ::testing::ValuesIn;

std::map<std::string, MatMultFunc> matMulFunctions = {
    {"MatrixMultSchoolbook", alg::matrixMultiplication<Matrix>},
    {"MatrixMultDivAndConq", alg::matrixMultDivConq<Matrix>},
    {"MatrixMultStrassen", alg::matrixMultStrassen<Matrix>},
};

std::map<std::string, std::tuple<Matrix, Matrix, Matrix>> testMatrices = {
    {"Matrix1x1", { 
        {{1}}, 
        {{2}}, 
        {{2}} } 
    },
    {"Matrix2x2", { 
        {{1, 2}, {3, 4}}, 
        {{5, 6}, {7, 8}}, 
        {{19, 22}, {43, 50}} } 
    },
    {"Matrix4x4", { 
        {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}},
        {{16, 15, 14, 13}, {12, 11, 10, 9}, {8, 7, 6, 5}, {4, 3, 2, 1}},
        {{80, 70, 60, 50}, {240, 214, 188, 162}, {400, 358, 316, 274}, {560, 502, 444, 386}} } 
    },
    {"IdentityMatrix", { 
        {{1, 0}, {0, 1}}, 
        {{5, 6}, {7, 8}}, 
        {{5, 6}, {7, 8}} } 
    },
};

class MatrixMultTest : public ::testing::TestWithParam<std::tuple<
    std::pair<const std::string, MatMultFunc>,
    std::pair<const std::string, std::tuple<Matrix, Matrix, Matrix>>>> 
{};

TEST_P(MatrixMultTest, WorksWithAllInputs) {
    // Get the parameters for the current test case
    auto param = GetParam();
    auto matMultFunc = std::get<0>(param).second;
    auto matrices = std::get<1>(param).second;
    auto matA = std::get<0>(matrices);
    auto matB = std::get<1>(matrices);
    auto expected = std::get<2>(matrices);
    // Initialize the result matrix with zeros
    Matrix matC(matA.size(), std::vector<int>(matB[0].size(), 0));
    // Call the matrix multiplication function
    matMultFunc(matA, matB, matC);
    // Check if the result matrix is equal to the expected matrix
    EXPECT_EQ(matC, expected);
}

INSTANTIATE_TEST_SUITE_P(MatrixMultTestsGenerator, MatrixMultTest,
    Combine(ValuesIn(matMulFunctions), ValuesIn(testMatrices)),
    [](const testing::TestParamInfo<MatrixMultTest::ParamType>& info) {
        return std::get<0>(info.param).first + "_" + std::get<1>(info.param).first;
    });
