//
// Created by edo on 09/07/22.
//

#include <fstream>
#include <memory>
#include "PXXManager.h"
#include "Image.h"
#include "PGMManager.h"

std::unique_ptr<Image<1>> PGMManager::readPGM(const std::string &path) {
    std::ifstream inputFile(path, std::ios::binary);
    if (!inputFile.is_open()) throw PGMReadException("Cannot open file " + path);

    PXXHeader header;
    header << inputFile;
    if (header.magicNumber != "P5")
        throw PGMReadException(
                "Cannot match magic number in " + path + " . Magic number found: " + header.magicNumber);
    if (header.maxVal != 255)
        throw PGMReadException("Unsupported PGM MaxVal in " + path + " . MaxVal found" + std::to_string(header.maxVal));

    auto image = std::make_unique<Image<1>>(header.width, header.height);
    for (int y = 0; y < image->getHeight(); y++) {
        for (int x = 0; x < image->getWidth(); x++) {
            unsigned int val;
            inputFile >> val;
            image->set(x, y, 0, val);
        }
    }
    inputFile.close();
    return image;
}

std::unique_ptr<Image<2>> PGMManager::readPGM(const std::string &path, const std::string &alphaPath) {
    auto baseImage = PGMManager::readPGM(path);
    auto alphaImage = PGMManager::readPGM(alphaPath);

    if (baseImage->getWidth() != alphaImage->getWidth() || baseImage->getHeight() != alphaImage->getHeight())
        throw PGMReadException("Grayscale and Alpha images have different dimensions");

    auto returnImage = std::make_unique<Image<2>>(baseImage->getWidth(), baseImage->getHeight());
    for (int y = 0; y < returnImage->getHeight(); y++) {
        for (int x = 0; x < returnImage->getWidth(); x++) {
            returnImage->set(x, y, 0, baseImage->get(x, y, 0));
            returnImage->set(x, y, 1, alphaImage->get(x, y, 0));
        }
    }
    return returnImage;
}

void PGMManager::writePGM(const std::string &path, const Image<1> &image) {
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

void PGMManager::writePGM(const std::string &path, const std::string &alphaPath, const Image<2> &image) {
    std::unique_ptr<Image<1>> baseImage = image.extractChannel(0);
    PGMManager::writePGM(path, *baseImage);
    std::unique_ptr<Image<1>> alphaImage = image.extractChannel(1);
    PGMManager::writePGM(alphaPath, *alphaImage);
}

