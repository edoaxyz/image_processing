//
// Created by edo on 09/07/22.
//

#include <fstream>
#include "PPMManager.h"
#include "PGMManager.h"
#include "PXXManager.h"

std::unique_ptr<RGBImage> PXXManager::readPPM(const std::string &path) {
    std::ifstream inputFile(path, std::ios::binary);
    if (!inputFile.is_open()) throw PPMReadException("Cannot open file " + path);

    PXXHeader header;
    header << inputFile;
    if (header.magicNumber != "P6")
        throw PPMReadException("Cannot match magic number in " + path + ". Magic number found: " + header.magicNumber);
    if (header.maxVal != 255)
        throw PPMReadException(
                "Unsupported PPM MaxVal in " + path + ". MaxVal found: " + std::to_string(header.maxVal));

    auto image = std::make_unique<RGBImage>(header.width, header.height);
    for (int y = 0; y < image->getHeight(); y++) {
        for (int x = 0; x < image->getWidth(); x++) {
            for (int c = 0; c < 3; c++) {
                unsigned char val;
                inputFile.read((char *) (&val), sizeof(unsigned char));
                image->set(x, y, c, val);
            }
        }
    }
    inputFile.close();
    return image;
}

std::unique_ptr<RGBAImage> PXXManager::readPPM(const std::string &path, const std::string &alphaPath) {
    auto base = PXXManager::readPPM(path);
    auto alpha = PXXManager::readPGM(alphaPath);
    if (base->getWidth() != alpha->getWidth() || base->getHeight() != alpha->getHeight())
        throw PPMReadException("Grayscale and Alpha images have different dimensions");

    auto final = std::make_unique<RGBAImage>(base->getWidth(), base->getHeight());
    for (int y = 0; y < final->getHeight(); y++) {
        for (int x = 0; x < final->getWidth(); x++) {
            final->set(x, y, 0, base->get(x, y, 0));
            final->set(x, y, 1, base->get(x, y, 1));
            final->set(x, y, 2, base->get(x, y, 2));
            final->set(x, y, 3, alpha->get(x, y, 0));
        }
    }
    return final;
}

void PXXManager::writePPM(const std::string &path, const RGBImage &image) {
    std::ofstream outputFile(path);
    if (!outputFile.is_open()) throw PGMWriteException("Cannot open file " + path);

    PXXHeader header("P6", image.getWidth(), image.getHeight(), 255);
    header >> outputFile;

    for (int y = 0; y < image.getHeight(); y++) {
        for (int x = 0; x < image.getWidth(); x++) {
            for (int c = 0; c < 3; c++) {
                outputFile << image.get(x, y, c);
            }
        }
    }
    outputFile.close();
}

void PXXManager::writePPM(const std::string &path, const std::string &alphaPath, const RGBAImage &image) {
    auto base = std::make_unique<RGBImage>(image.getWidth(), image.getHeight());
    auto alpha = std::make_unique<GImage>(image.getWidth(), image.getHeight());
    for (int y = 0; y < image.getHeight(); y++) {
        for (int x = 0; x < image.getWidth(); x++) {
            base->set(x, y, 0, image.get(x, y, 0));
            base->set(x, y, 1, image.get(x, y, 1));
            base->set(x, y, 2, image.get(x, y, 2));
            alpha->set(x, y, 0, image.get(x, y, 3));
        }
    }
    PXXManager::writePPM(path, *base);
    PXXManager::writePGM(alphaPath, *alpha);
}

extern template
class Image<1>;

extern template
class Image<3>;

extern template
class Image<4>;