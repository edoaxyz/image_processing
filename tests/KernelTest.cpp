//
// Created by edo on 07/07/22.
//
#include "gtest/gtest.h"
#include "../Kernel.cpp"

TEST(Kernel, TestValidation) {
    EXPECT_THROW({
                     Kernel k({1, 2, 3, 4});
                 }, WrongSizeKernelException);

    EXPECT_THROW({
                     Kernel k({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
                 }, WrongSizeKernelException);

    EXPECT_NO_THROW({
                        Kernel k({1, 2, 3, 4, 5, 6, 7, 8, 9});
                    });
}

TEST(Kernel, TestGet) {
    Kernel k{1, 2, 3, 4, 5, 6, 7, 8, 9};

    ASSERT_EQ(5, k.getValue(1, 1));
    ASSERT_EQ(5, k.getCenteredValue(0, 0));
}

TEST(Kernel, TestGaussBlur) {
    GaussBlurKernel k(0.84, 7);

    ASSERT_EQ(225, std::floor(k.getCenteredValue(0, 0) * 1000));
}