//
// Created by edo on 09/07/22.
//


#include <fstream>
#include <iostream>
#include "PGMManager.h"
#include "PXXManager.h"

std::unique_ptr<GImage> PGMManager::readPGM(const std::string &path) {
    std::ifstream inputFile(path, std::ios::binary);
    if (!inputFile.is_open()) throw PGMReadException("Cannot open file " + path);

    PXXHeader header;
    header << inputFile;
    if (header.magicNumber != "P5")
        throw PGMReadException(
                "Cannot match magic number in " + path + ". Magic number found: " + header.magicNumber);
    if (header.maxVal != 255)
        throw PGMReadException(
                "Unsupported PGM MaxVal in " + path + ". MaxVal found: " + std::to_string(header.maxVal));

    auto image = std::make_unique<GImage>(header.width, header.height);
    for (int y = 0; y < image->getHeight(); y++) {
        for (int x = 0; x < image->getWidth(); x++) {
            unsigned char val;
            inputFile.read((char *) (&val), sizeof(unsigned char));
            image->set(x, y, 0, val);
        }
    }
    inputFile.close();
    return image;
}

std::unique_ptr<GAImage> PGMManager::readPGM(const std::string &path, const std::string &alphaPath) {
    auto baseImage = PGMManager::readPGM(path);
    auto alphaImage = PGMManager::readPGM(alphaPath);

    if (baseImage->getWidth() != alphaImage->getWidth() || baseImage->getHeight() != alphaImage->getHeight())
        throw PGMReadException("Grayscale and Alpha images have different dimensions");

    auto returnImage = std::make_unique<GAImage>(baseImage->getWidth(), baseImage->getHeight());
    for (int y = 0; y < returnImage->getHeight(); y++) {
        for (int x = 0; x < returnImage->getWidth(); x++) {
            returnImage->set(x, y, 0, baseImage->get(x, y, 0));
            returnImage->set(x, y, 1, alphaImage->get(x, y, 0));
        }
    }
    return returnImage;
}

void PGMManager::writePGM(const std::string &path, const GImage &image) {
    std::ofstream outputFile(path, std::ios::binary);
    if (!outputFile.is_open()) throw PGMWriteException("Cannot open file " + path);

    PXXHeader header("P5", image.getWidth(), image.getHeight(), 255);
    header >> outputFile;

    for (int y = 0; y < image.getHeight(); y++) {
        for (int x = 0; x < image.getWidth(); x++) {
            outputFile << image.get(x, y, 0);
        }
    }
    outputFile.close();
}

void PGMManager::writePGM(const std::string &path, const std::string &alphaPath, const GAImage &image) {
    std::unique_ptr<GImage> baseImage = image.extractChannel(0);
    PGMManager::writePGM(path, *baseImage);
    std::unique_ptr<GImage> alphaImage = image.extractChannel(1);
    PGMManager::writePGM(alphaPath, *alphaImage);
}

extern template
class Image<1>;

extern template
class Image<2>;
