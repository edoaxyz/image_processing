//
// Created by edo on 12/07/22.
//
#include "gtest/gtest.h"
#include "../PGMManager.h"

TEST(PGMManager, TestReadMonocromeImage) {
    auto image = PXXManager::readPGM("assets/pgm/test.pgm");
    ASSERT_EQ(255, image->get(0, 0, 0));
    ASSERT_EQ(216, image->get(0, 1, 0));
    ASSERT_EQ(145, image->get(1, 0, 0));
    ASSERT_EQ(193, image->get(1, 1, 0));
    ASSERT_EQ(216, image->get(2, 0, 0));
    ASSERT_EQ(70, image->get(2, 1, 0));
}

TEST(PGMManager, TestReadImageWithAlpha) {
    auto image = PXXManager::readPGM("assets/pgm/test.pgm", "assets/pgm/test-alpha.pgm");
    ASSERT_EQ(255, image->get(0, 0, 0));
    ASSERT_EQ(216, image->get(0, 1, 0));
    ASSERT_EQ(145, image->get(1, 0, 0));
    ASSERT_EQ(193, image->get(1, 1, 0));
    ASSERT_EQ(216, image->get(2, 0, 0));
    ASSERT_EQ(70, image->get(2, 1, 0));
    ASSERT_EQ(0, image->get(0, 0, 1));
    ASSERT_EQ(85, image->get(0, 1, 1));
    ASSERT_EQ(234, image->get(1, 0, 1));
    ASSERT_EQ(127, image->get(1, 1, 1));
    ASSERT_EQ(86, image->get(2, 0, 1));
    ASSERT_EQ(244, image->get(2, 1, 1));
}

TEST(PGMManager, TestWriteMonocromeImage) {
    GImage image(2, 3, 0);
    image.set(1, 2, 0, 128);
    PXXManager::writePGM("test.pgm", image);
    auto output = PXXManager::readPGM("test.pgm");
    ASSERT_EQ(image.getWidth(), output->getWidth());
    ASSERT_EQ(image.getHeight(), output->getHeight());
    for (int y = 0; y < output->getHeight(); y++) {
        for (int x = 0; x < output->getWidth(); x++) {
            ASSERT_EQ(x != 1 || y != 2 ? 0 : 128, output->get(x, y, 0));
        }
    }
}

TEST(PGMManager, TestWriteImageWithAlpha) {
    GAImage image(2, 3, 0);
    image.set(1, 1, 0, 128);
    image.set(1, 2, 1, 128);
    PXXManager::writePGM("test.pgm", "test-alpha.pgm", image);
    auto output = PXXManager::readPGM("test.pgm", "test-alpha.pgm");
    ASSERT_EQ(image.getWidth(), output->getWidth());
    ASSERT_EQ(image.getHeight(), output->getHeight());
    for (int y = 0; y < output->getHeight(); y++) {
        for (int x = 0; x < output->getWidth(); x++) {
            ASSERT_EQ(x != 1 || y != 1 ? 0 : 128, output->get(x, y, 0));
            ASSERT_EQ(x != 1 || y != 2 ? 0 : 128, output->get(x, y, 1));
        }
    }
}

TEST(PGMManager, TestFake) {
    ASSERT_ANY_THROW({
                         PXXManager::readPGM("assets/pgm/fake.pgm");
                     });
}

TEST(PGMManager, TestUnsupportedMagic) {
    ASSERT_THROW({
                     try {
                         PXXManager::readPGM("assets/pgm/unsupported-magic.pgm");
                     }
                     catch (const PXXManager::PGMReadException &e) {
                         EXPECT_STREQ(
                                 "Cannot match magic number in assets/pgm/unsupported-magic.pgm. Magic number found: P2",
                                 e.what());
                         throw;
                     }
                 }, PXXManager::PGMReadException);
}

TEST(PGMManager, TestUnsupportedMaxVal) {
    ASSERT_THROW({
                     try {
                         PXXManager::readPGM("assets/pgm/unsupported-maxval.pgm");
                     }
                     catch (const PXXManager::PGMReadException &e) {
                         EXPECT_STREQ(
                                 "Unsupported PGM MaxVal in assets/pgm/unsupported-maxval.pgm. MaxVal found: 65535",
                                 e.what());
                         throw;
                     }
                 }, PXXManager::PGMReadException);
}

TEST(PGMManager, TestAlphaImageDifferentDimensions) {
    ASSERT_THROW({
                     try {
                         PXXManager::readPGM("assets/pgm/test.pgm", "assets/pgm/test-wrong-size-alpha.pgm");
                     }
                     catch (const PXXManager::PGMReadException &e) {
                         EXPECT_STREQ("Grayscale and Alpha images have different dimensions", e.what());
                         throw;
                     }

                 }, PXXManager::PGMReadException);
}
