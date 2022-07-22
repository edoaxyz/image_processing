//
// Created by edo on 08/07/22.
//

#include "gtest/gtest.h"
#include "../Image.h"
#include "../PPMManager.h"

/*TEST(Image, TestIdentityKernel) {
    RGBAImage image(200, 200);
    image.set(100, 100, 3, 255);
    IdentityKernel identityKernel;
    image.applyKernel(identityKernel);
    ASSERT_EQ(255, image.get(100, 100, 3));
}

TEST(Image, TestEdge0Kernel) {
    Edge0Kernel edge0Kernel;
    auto original = PXXManager::readPPM("assets/image_processing/original.ppm");
    original->applyKernel(edge0Kernel);

    auto edge0 = PXXManager::readPPM("assets/image_processing/edge0.ppm");
    for (int x = 0; x < edge0->getWidth(); x++)
        for (int y = 0; y < edge0->getHeight(); y++)
            for (int c = 0; c < 3; c++)
                ASSERT_EQ(original->get(x, y, c), edge0->get(x, y, c));
}

TEST(Image, TestEdge1Kernel) {
    Edge1Kernel edge1Kernel;
    auto original = PXXManager::readPPM("assets/image_processing/original.ppm");
    original->applyKernel(edge1Kernel);

    auto edge1 = PXXManager::readPPM("assets/image_processing/edge1.ppm");
    for (int x = 0; x < edge1->getWidth(); x++)
        for (int y = 0; y < edge1->getHeight(); y++)
            for (int c = 0; c < 3; c++)
                ASSERT_EQ(original->get(x, y, c), edge1->get(x, y, c));
}*/

TEST(Image, TestEdge2Kernel) {
    Edge2Kernel edge2Kernel;
    auto original = PXXManager::readPPM("assets/image_processing/original.ppm");
    original->applyKernel(edge2Kernel);

    auto edge2 = PXXManager::readPPM("assets/image_processing/edge2.ppm");
    for (int x = 0; x < edge2->getWidth(); x++)
        for (int y = 0; y < edge2->getHeight(); y++)
            for (int c = 0; c < 3; c++) {
                ASSERT_EQ(original->get(x, y, c), edge2->get(x, y, c));
            }
}

TEST(Image, TestSharpenKernel) {
    SharpenKernel sharpenKernel;
    auto original = PXXManager::readPPM("assets/image_processing/original.ppm");
    original->applyKernel(sharpenKernel);

    auto sharpen = PXXManager::readPPM("assets/image_processing/sharp.ppm");
    for (int x = 0; x < sharpen->getWidth(); x++)
        for (int y = 0; y < sharpen->getHeight(); y++)
            for (int c = 0; c < 3; c++)
                ASSERT_EQ(original->get(x, y, c), sharpen->get(x, y, c));
}

TEST(Image, TestGaussBlurKernel) {
    GaussBlurKernel gaussBlurKernel(5, 3);
    auto original = PXXManager::readPPM("assets/image_processing/original.ppm");
    original->applyKernel(gaussBlurKernel);

    auto blurred = PXXManager::readPPM("assets/image_processing/gauss-blur.ppm");
    for (int x = 0; x < blurred->getWidth(); x++)
        for (int y = 0; y < blurred->getHeight(); y++)
            for (int c = 0; c < 3; c++)
                ASSERT_EQ(original->get(x, y, c), blurred->get(x, y, c));
}

extern template
class Image<4>;