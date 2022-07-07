//
// Created by edo on 05/07/22.
//
#include "gtest/gtest.h"
#include "../Image.h"

TEST(Image, TestCorrectInitialization) {
    Image<1> image(300, 400, 1);
    ASSERT_EQ(1, image.get(200, 200, 0));
}

TEST(Image, TestSet) {
    Image<1> image(300, 400, 1);
    image.set(200, 200, 0, 20);
    ASSERT_EQ(20, image.get(200, 200, 0));
}

TEST(Image, TestClone) {
    Image<1> image(300, 400, 1);
    image.set(200, 200, 0, 20);

    Image<1> image2(image);
    image.set(200, 200, 0, 30);
    ASSERT_EQ(20, image2.get(200, 200, 0));
}

TEST(Image, TestValidation) {
    Image<2> image(300, 400, 1);
    EXPECT_NO_THROW({
                        image.set(200, 200, 0, 20);
                    });

    EXPECT_THROW({
                     image.set(500, 200, 0, 20);
                 }, WrongArgumentsException);

    EXPECT_THROW({
                     image.set(200, 500, 0, 20);
                 }, WrongArgumentsException);

    EXPECT_THROW({
                     image.set(200, 500, 4, 20);
                 }, WrongArgumentsException);
}