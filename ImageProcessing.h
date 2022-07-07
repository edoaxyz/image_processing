//
// Created by edo on 07/07/22.
//

#ifndef IMAGE_PROCESSING_IMAGEPROCESSING_H
#define IMAGE_PROCESSING_IMAGEPROCESSING_H

#include <vector>
#include <string>
#include <stdexcept>
#include "Image.h"

class ImageProcessingException : public std::exception {
public:
    ImageProcessingException() : std::exception() {};

    virtual ~ImageProcessingException() override {};
};


template<int channels, typename T>
class ImageProcessing {
public:


private:

    static void convolute(Image<channels, T> &image, const std::vector<std::vector<float>> &kernel) {
        for (int i = 0; i < channels; i++) {
            for (int y = kernel.size() / 2; y < image.getHeight() - (kernel.size() / 2); y++) {
                //for (int x = kernel.size() / 2;)
            }
            for (auto &row: kernel) {
                for (auto &col: row) {
                    //image.get()
                }
            }
        }
    }


}

#endif //IMAGE_PROCESSING_IMAGEPROCESSING_H
