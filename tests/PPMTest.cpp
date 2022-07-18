//
// Created by edo on 15/07/22.
//

//
// Created by edo on 12/07/22.
//
#include "gtest/gtest.h"
#include "../PPMManager.h"

TEST(PPMManager, TestReadImage) {
    auto image = PPMManager::readPPM("assets/ppm/test.ppm");
    ASSERT_EQ(255, image->get(0, 0, 0));
    ASSERT_EQ(255, image->get(0, 0, 1));
    ASSERT_EQ(255, image->get(0, 0, 2));
    ASSERT_EQ(213, image->get(0, 1, 0));
    ASSERT_EQ(213, image->get(0, 1, 1));
    ASSERT_EQ(255, image->get(0, 1, 2));
    ASSERT_EQ(253, image->get(1, 0, 0));
    ASSERT_EQ(81, image->get(1, 0, 1));
    ASSERT_EQ(89, image->get(1, 0, 2));
    ASSERT_EQ(188, image->get(1, 1, 0));
    ASSERT_EQ(188, image->get(1, 1, 1));
    ASSERT_EQ(255, image->get(1, 1, 2));
    ASSERT_EQ(213, image->get(2, 0, 0));
    ASSERT_EQ(213, image->get(2, 0, 1));
    ASSERT_EQ(255, image->get(2, 0, 2));
    ASSERT_EQ(67, image->get(2, 1, 0));
    ASSERT_EQ(67, image->get(2, 1, 1));
    ASSERT_EQ(103, image->get(2, 1, 2));
}

TEST(PPMManager, TestReadImageWithAlpha) {
    auto image = PPMManager::readPPM("assets/ppm/test.ppm", "assets/ppm/test-alpha.pgm");
    ASSERT_EQ(255, image->get(0, 0, 0));
    ASSERT_EQ(255, image->get(0, 0, 1));
    ASSERT_EQ(255, image->get(0, 0, 2));
    ASSERT_EQ(0, image->get(0, 0, 3));
    ASSERT_EQ(213, image->get(0, 1, 0));
    ASSERT_EQ(213, image->get(0, 1, 1));
    ASSERT_EQ(255, image->get(0, 1, 2));
    ASSERT_EQ(85, image->get(0, 1, 3));
    ASSERT_EQ(253, image->get(1, 0, 0));
    ASSERT_EQ(81, image->get(1, 0, 1));
    ASSERT_EQ(89, image->get(1, 0, 2));
    ASSERT_EQ(234, image->get(1, 0, 3));
    ASSERT_EQ(188, image->get(1, 1, 0));
    ASSERT_EQ(188, image->get(1, 1, 1));
    ASSERT_EQ(255, image->get(1, 1, 2));
    ASSERT_EQ(127, image->get(1, 1, 3));
    ASSERT_EQ(213, image->get(2, 0, 0));
    ASSERT_EQ(213, image->get(2, 0, 1));
    ASSERT_EQ(255, image->get(2, 0, 2));
    ASSERT_EQ(86, image->get(2, 0, 3));
    ASSERT_EQ(67, image->get(2, 1, 0));
    ASSERT_EQ(67, image->get(2, 1, 1));
    ASSERT_EQ(103, image->get(2, 1, 2));
    ASSERT_EQ(244, image->get(2, 1, 3));
}

TEST(PPMManager, TestWriteImage) {
    Image<3> image(2, 3, 0);
    image.set(1, 2, 2, 128);
    PPMManager::writePPM("test.ppm", image);
    auto output = PPMManager::readPPM("test.ppm");
    ASSERT_EQ(image.getWidth(), output->getWidth());
    ASSERT_EQ(image.getHeight(), output->getHeight());
    for (int y = 0; y < output->getHeight(); y++) {
        for (int x = 0; x < output->getWidth(); x++) {
            for (int c = 0; c < 3; c++) ASSERT_EQ(x != 1 || y != 2 || c != 2 ? 0 : 128, output->get(x, y, c));
        }
    }
}

TEST(PPMManager, TestWriteImageWithAlpha) {
    Image<4> image(2, 3, 0);
    image.set(1, 2, 2, 128);
    image.set(0, 2, 3, 128);
    PPMManager::writePPM("test.ppm", "test-alpha.pgm", image);
    auto output = PPMManager::readPPM("test.ppm", "test-alpha.pgm");
    ASSERT_EQ(image.getWidth(), output->getWidth());
    ASSERT_EQ(image.getHeight(), output->getHeight());
    for (int y = 0; y < output->getHeight(); y++) {
        for (int x = 0; x < output->getWidth(); x++) {
            for (int c = 0; c < 3; c++) ASSERT_EQ(x != 1 || y != 2 || c != 2 ? 0 : 128, output->get(x, y, c));
            ASSERT_EQ(x != 0 || y != 2 ? 0 : 128, output->get(x, y, 3));
        }
    }
}


TEST(PPMManager, TestFake) {
    ASSERT_ANY_THROW({
                         PPMManager::readPPM("assets/ppm/fake.ppm");
                     });
}

TEST(PPMManager, TestUnsupportedMagic) {
    ASSERT_THROW({
                     try {
                         PPMManager::readPPM("assets/ppm/unsupported-magic.ppm");
                     }
                     catch (const PPMReadException &e) {
                         EXPECT_STREQ(
                                 "Cannot match magic number in assets/ppm/unsupported-magic.ppm. Magic number found: P3",
                                 e.what());
                         throw;
                     }
                 }, PPMReadException);
}

TEST(PPMManager, TestUnsupportedMaxVal) {
    ASSERT_THROW({
                     try {
                         PPMManager::readPPM("../../tests/assets/ppm/unsupported-maxval.ppm");
                     }
                     catch (const PPMReadException &e) {
                         EXPECT_STREQ(
                                 "Unsupported PPM MaxVal in ../../tests/assets/ppm/unsupported-maxval.ppm. MaxVal found: 65535",
                                 e.what());
                         throw;
                     }
                 }, PPMReadException);
}

TEST(PPMManager, TestAlphaImageDifferentDimensions) {
    ASSERT_THROW({
                     try {
                         PPMManager::readPPM("../../tests/assets/ppm/test.ppm",
                                             "../../tests/assets/ppm/test-wrong-size-alpha.pgm");
                     }
                     catch (const PPMReadException &e) {
                         EXPECT_STREQ("Grayscale and Alpha images have different dimensions", e.what());
                         throw;
                     }

                 }, PPMReadException);
}
