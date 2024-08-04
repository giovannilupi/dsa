#include <gtest/gtest.h>
#include "modulo.hpp"

TEST(ModuloTests, WorksWithPosMod) {
    EXPECT_EQ(alg::mod(3, 5), 3);
    EXPECT_EQ(alg::mod(4, 5), 4);
    EXPECT_EQ(alg::mod(5, 5), 0);
    EXPECT_EQ(alg::mod(999, 995), 4);
    EXPECT_EQ(alg::mod(1663, 11), 2);
    EXPECT_EQ(alg::mod(12489013, 292), 173);
}

TEST(ModuloTests, WorksWithNegDividend) {
    EXPECT_EQ(alg::mod(-3, 5), 2);
    EXPECT_EQ(alg::mod(-4, 5), 1);
    EXPECT_EQ(alg::mod(-10, 7), 4);
    EXPECT_EQ(alg::mod(-15, 4), 1);
    EXPECT_EQ(alg::mod(-20, 13), 6);
    EXPECT_EQ(alg::mod(-1, 3), 2);
    EXPECT_EQ(alg::mod(-25, 6), 5);
    EXPECT_EQ(alg::mod(-100, 21), 5);
    EXPECT_EQ(alg::mod(-123, 15), 12);
    EXPECT_EQ(alg::mod(-987, 23), 2);
}

TEST(ModuloTests, WorksWithZeroDividend) {
    EXPECT_EQ(alg::mod(0, 5), 0);
    EXPECT_EQ(alg::mod(0, 1), 0);
    EXPECT_EQ(alg::mod(0, 100), 0);
    EXPECT_EQ(alg::mod(0, 13), 0);
    EXPECT_EQ(alg::mod(0, 7), 0);
    EXPECT_EQ(alg::mod(0, 25), 0);
    EXPECT_EQ(alg::mod(0, 37), 0);
    EXPECT_EQ(alg::mod(0, 123), 0);
    EXPECT_EQ(alg::mod(0, 789), 0);
    EXPECT_EQ(alg::mod(0, 4567), 0);
}

TEST(ModuloTests, WorksWithOneDivisor) {
    EXPECT_EQ(alg::mod(100, 1), 0);
    EXPECT_EQ(alg::mod(-50, 1), 0);
    EXPECT_EQ(alg::mod(0, 1), 0);
    EXPECT_EQ(alg::mod(12345, 1), 0);
    EXPECT_EQ(alg::mod(-6789, 1), 0);
    EXPECT_EQ(alg::mod(99999, 1), 0);
    EXPECT_EQ(alg::mod(-100000, 1), 0);
    EXPECT_EQ(alg::mod(1, 1), 0);
    EXPECT_EQ(alg::mod(-1, 1), 0);
    EXPECT_EQ(alg::mod(987654321, 1), 0);
}

TEST(ModuloTests, ThrowsWithInvalidDivisor) {
    EXPECT_THROW(alg::mod(10, 0), std::invalid_argument);
    EXPECT_THROW(alg::mod(10, -5), std::invalid_argument);
}

