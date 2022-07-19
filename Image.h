//
// Created by edo on 05/07/22.
//

#ifndef IMAGE_PROCESSING_IMAGE_H
#define IMAGE_PROCESSING_IMAGE_H


#include <memory>
#include <string>
#include <iostream>
#include <stdexcept>
#include <limits>
#include "Kernel.h"

class ImageException {
public:
    ImageException() = default;

    virtual ~ImageException() = default;
};

class WrongArgumentsImageException : public ImageException, public std::out_of_range {
public:
    explicit WrongArgumentsImageException(const std::string &msg) : std::out_of_range(msg) {};

    ~WrongArgumentsImageException() override {};
};

template<typename T = unsigned char>
class AbstractImage {
public:
    virtual void set(const unsigned int x, const unsigned int y, const unsigned int channel, const T value) = 0;

    virtual const T get(const unsigned int x, const unsigned int y, const unsigned int channel) const = 0;

    virtual unsigned int getWidth() const = 0;

    virtual unsigned int getHeight() const = 0;

    virtual void applyKernel(const Kernel &kernel) = 0;
};

template<int channels, typename T = unsigned char>
class Image : public AbstractImage<T> {
public:
    Image<channels, T>(const unsigned int width, const unsigned int height, T initValue = 0) : width(width),
                                                                                               height(height) {
        data = std::make_unique<T[]>(width * height * channels);
        for (int i = 0; i < width * height * channels; i++)
            data[i] = initValue;
    }

    Image<channels, T>(const Image<channels, T> &copy) : Image(copy.width, copy.height) {
        for (int i = 0; i < width * height * channels; i++)
            data[i] = copy.data[i];
    }

    virtual Image<channels, T> &operator=(Image<channels, T> &copy) {
        width = copy.width;
        height = copy.height;
        data = std::move(copy.data);
        return *this;
    }

    virtual void set(const unsigned int x, const unsigned int y, const unsigned int channel, const T value) {
        validateArguments(x, y, channel);
        data[y * width + x + getChannelOffset(channel)] = value;
    }

    virtual const T get(const unsigned int x, const unsigned int y, const unsigned int channel) const {
        validateArguments(x, y, channel);
        return data[y * width + x + getChannelOffset(channel)];
    }

    virtual unsigned int getWidth() const {
        return width;
    }

    virtual unsigned int getHeight() const {
        return height;
    }

    virtual std::unique_ptr<Image<1, T>> getGrayScale() const {
        auto ptr = std::make_unique<Image<1, T>>(width, height);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                double sum = 0;
                for (int c = 0; c < channels; c++) {
                    sum += data[y * width + x + getChannelOffset(c)];
                }
                ptr->set(x, y, 0, sum / channels);
            }
        }
        return ptr;
    }

    virtual std::unique_ptr<Image<1, T>> extractChannel(int channel) const {
        auto ptr = std::make_unique<Image<1, T>>(width, height);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                ptr->set(x, y, 0, get(x, y, channel));
            }
        }
        return ptr;
    }

    virtual void applyKernel(const Kernel &kernel) {
        Image<channels, T> copy(*this);
        for (int i = 0; i < channels; i++) {
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // convolute
                    double sum = 0;
                    for (int kernelX = -kernel.getMatrixSize() / 2; kernelX <= kernel.getMatrixSize() / 2; kernelX++) {
                        for (int kernelY = -kernel.getMatrixSize() / 2;
                             kernelY <= kernel.getMatrixSize() / 2; kernelY++) {
                            try {
                                sum += kernel.getCenteredValue(kernelX, kernelY) *
                                       copy.get(x - kernelX, y - kernelY, i);
                            } catch (const WrongArgumentsImageException &exception) {}
                        }
                    }

                    sum = std::min(std::max(sum, double(std::numeric_limits<T>::min())),
                                   double(std::numeric_limits<T>::max()));
                    set(x, y, i, sum);
                }
            }
        }
    }

protected:
    unsigned int width;
    unsigned int height;

    std::unique_ptr<T[]> data;
private:
    virtual void
    validateArguments(const unsigned int x = 0, const unsigned int y = 0, const unsigned int channel = 0) const {
        if (channel >= channels || channel < 0)
            throw WrongArgumentsImageException("Passed wrong channel parameter: passed " + std::to_string(channel) +
                                               ", expected value >= 0 and < " + std::to_string(channels));
        if (x >= width || x < 0)
            throw WrongArgumentsImageException(
                    "Passed wrong x parameter: passed " + std::to_string(x) + ", expected value >= 0 and < " +
                    std::to_string(width));
        if (y >= height || y < 0)
            throw WrongArgumentsImageException(
                    "Passed wrong y parameter: passed " + std::to_string(y) + ", expected value >= 0 and < " +
                    std::to_string(height));
    }

    virtual int getChannelOffset(unsigned int channel) const {
        validateArguments(0, 0, channel);
        return (width * height * channel);
    }
};

#endif //IMAGE_PROCESSING_IMAGE_H
