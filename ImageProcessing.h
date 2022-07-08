//
// Created by edo on 07/07/22.
//

#ifndef IMAGE_PROCESSING_IMAGEPROCESSING_H
#define IMAGE_PROCESSING_IMAGEPROCESSING_H

#include <limits>
#include "Image.h"
#include "Kernel.h"

template<int channels, typename T = unsigned char>
class ImageProcessing {
public:
    static void applyKernel(Image<channels, T> &image, Kernel &kernel) {
        for (int i = 0; i < channels; i++) {
            for (int y = 0; y < image.getHeight(); y++) {
                for (int x = 0; x < image.getWidth(); x++) {
                    // convolute
                    double sum = 0;
                    for (int kernelX = -kernel.getMatrixSize() / 2; kernelX < kernel.getMatrixSize() / 2; kernelX++) {
                        for (int kernelY = -kernel.getMatrixSize() / 2;
                             kernelY < kernel.getMatrixSize() / 2; kernelY++) {
                            try {
                                sum += kernel.getCenteredValue(kernelX, kernelY) *
                                       image.get(x - kernelX, y - kernelY, i);
                            } catch (WrongArgumentsImageException &exception) {}
                        }
                    }
                    sum = std::min(std::max(sum, double(std::numeric_limits<T>::min())),
                                   double(std::numeric_limits<T>::max()));
                    image.set(x, y, i, sum);
                }
            }
        }
    }
};

class GImgProcessing : public ImageProcessing<1> {
public:
    static void applyKernel(GImage &image, Kernel &kernel) {
        ImageProcessing<1>::applyKernel(image, kernel);
    }
};

class GAImgProcessing : public ImageProcessing<2> {
public:
    static void applyKernel(GAImage &image, Kernel &kernel) {
        ImageProcessing<2>::applyKernel(image, kernel);
    }
};

class RGBImgProcessing : public ImageProcessing<3> {
public:
    static void applyKernel(RGBImage &image, Kernel &kernel) {
        ImageProcessing<3>::applyKernel(image, kernel);
    }
};

class RGBAImgProcessing : public ImageProcessing<4> {
public:
    static void applyKernel(RGBAImage &image, Kernel &kernel) {
        ImageProcessing<4>::applyKernel(image, kernel);
    }
};

#endif //IMAGE_PROCESSING_IMAGEPROCESSING_H
