//
// Created by edo on 05/07/22.
//

#ifndef IMAGE_PROCESSING_IMAGE_H
#define IMAGE_PROCESSING_IMAGE_H


#include <memory>
#include <string>
#include <stdexcept>

class ImageException : public std::exception {
public:
    ImageException() : std::exception() {};

    virtual ~ImageException() override {};
};

class WrongArgumentsException : public ImageException, public std::out_of_range {
public:
    WrongArgumentsException(const std::string &msg) : std::out_of_range(msg) {};

    virtual ~WrongArgumentsException() override {};
};

template<int channels, typename T = unsigned char>
class Image {
public:
    Image(const unsigned int width, const unsigned int height, T initValue = 0) : width(width), height(height) {
        data = std::make_unique<T[]>(width * height * channels);
        for (int i = 0; i < width * height * channels; i++)
            data[i] = initValue;
    }

    Image<channels, T>(const Image<channels, T> &copy) : Image(copy.width, copy.height) {
        for (int i = 0; i < width * height * channels; i++)
            data[i] = copy.data[i];
    }

    void set(const unsigned int x, const unsigned int y, const unsigned int channel, const T value) {
        validateArguments(x, y, channel);
        data[y * width + x + getChannelOffset(channel)] = value;
    }

    const T get(const unsigned int x, const unsigned int y, const unsigned int channel) const {
        validateArguments(x, y, channel);
        return data[y * width + x + getChannelOffset(channel)];
    }

    unsigned int getWidth() const {
        return width;
    }

    unsigned int getHeight() const {
        return height;
    }

    std::unique_ptr<Image<1, T>> getGrayScale() const {
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

protected:
    unsigned int width;
    unsigned int height;
    std::unique_ptr<T[]> data;

private:
    void validateArguments(const unsigned int x = 0, const unsigned int y = 0, const unsigned int channel = 0) const {
        if (channel >= channels || channel < 0)
            throw WrongArgumentsException("Passed wrong channel parameter: passed " + std::to_string(channel) +
                                          ", expected value >= 0 and < " + std::to_string(channels));
        if (x >= width || x < 0)
            throw WrongArgumentsException(
                    "Passed wrong x parameter: passed " + std::to_string(x) + ", expected value >= 0 and < " +
                    std::to_string(width));
        if (y >= height || y < 0)
            throw WrongArgumentsException(
                    "Passed wrong y parameter: passed " + std::to_string(y) + ", expected value >= 0 and < " +
                    std::to_string(height));
    }

    int getChannelOffset(unsigned int channel) const {
        validateArguments(0, 0, channel);
        return (width * height * channel);
    }
};


#endif //IMAGE_PROCESSING_IMAGE_H
