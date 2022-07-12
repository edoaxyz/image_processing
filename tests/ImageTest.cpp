//
// Created by edo on 05/07/22.
//
#include "gtest/gtest.h"
#include "../Image.h"

TEST(Image, TestCorrectInitialization) {
    Image<1> image(300, 400, 1);
    for (int y = 0; y < image.getHeight(); y++) {
        for (int x = 0; x < image.getWidth(); x++) {
            ASSERT_EQ(1, image.get(x, y, 0));
        }
    }
}

TEST(Image, TestSet) {
    Image<1> image(300, 400, 1);
    image.set(200, 200, 0, 20);
    ASSERT_EQ(20, image.get(200, 200, 0));
    for (int y = 0; y < image.getHeight(); y++) {
        for (int x = 0; x < image.getWidth(); x++) {
            if (x != 200 && y != 200) ASSERT_EQ(1, image.get(x, y, 0));
        }
    }
}

TEST(Image, TestClone) {
    Image<1> image(300, 400, 1);
    image.set(200, 200, 0, 20);

    Image<1> image2(image);
    image.set(200, 200, 0, 30);
    ASSERT_EQ(20, image2.get(200, 200, 0));
    for (int y = 0; y < image2.getHeight(); y++) {
        for (int x = 0; x < image2.getWidth(); x++) {
            if (x != 200 && y != 200) ASSERT_EQ(1, image2.get(x, y, 0));
        }
    }
}

TEST(Image, TestValidation) {
    Image<2> image(300, 400, 1);
    EXPECT_NO_THROW({
                        image.set(200, 200, 0, 20);
                    });

    EXPECT_THROW({
                     image.set(500, 200, 0, 20);
                 }, WrongArgumentsImageException);

    EXPECT_THROW({
                     image.set(200, 500, 0, 20);
                 }, WrongArgumentsImageException);

    EXPECT_THROW({
                     image.set(200, 500, 4, 20);
                 }, WrongArgumentsImageException);
}

TEST(Image, TestGrayScale) {
    Image<3> image(200, 400, 0);
    image.set(100, 200, 1, 255);
    image.set(100, 200, 2, 255);

    auto grayscale = image.getGrayScale();
    ASSERT_EQ(170, grayscale->get(100, 200, 0));
    for (int y = 0; y < image.getHeight(); y++) {
        for (int x = 0; x < image.getWidth(); x++) {
            if (x != 100 && y != 200) ASSERT_EQ(0, grayscale->get(x, y, 0));
        }
    }
}

TEST(Image, TestChannelExtraction) {
    Image<2> image(200, 200, 0);
    image.set(100, 100, 0, 230);

    auto channel0 = image.extractChannel(0);
    auto channel1 = image.extractChannel(1);
    ASSERT_EQ(230, channel0->get(100, 100, 0));
    for (int y = 0; y < image.getHeight(); y++) {
        for (int x = 0; x < image.getWidth(); x++) {
            if (x != 100 && y != 100) ASSERT_EQ(0, channel0->get(x, y, 0));
            ASSERT_EQ(0, channel1->get(x, y, 0));
        }
    }
}