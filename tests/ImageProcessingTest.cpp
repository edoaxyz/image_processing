//
// Created by edo on 08/07/22.
//

#include "gtest/gtest.h"
#include "../Image.h"

TEST(Image, TestIdentityKernel) {
    RGBAImage image(200, 200);
    image.set(100, 100, 3, 255);
    IdentityKernel identityKernel;
    image.applyKernel(identityKernel);
    ASSERT_EQ(255, image.get(100, 100, 3));
}

// TODO: add other test

extern template
class Image<4>;