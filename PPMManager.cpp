//
// Created by edo on 09/07/22.
//

#include <fstream>
#include "PPMManager.h"
#include "PXXManager.h"
#include "PGMManager.h"

std::unique_ptr<Image<3>> PPMManager::readPPM(const std::string &path) {
    std::ifstream inputFile(path);
    if (!inputFile.is_open()) throw PPMReadException("Cannot open file " + path);

    PXXHeader header;
    header << inputFile;
    if (header.magicNumber != "P6")
        throw PPMReadException("Magic number not matched in " + path + ". Magic number found: " + header.magicNumber);
    if (header.maxVal != 255)
        throw PPMReadException("MaxVal not supported in " + path + ". MaxVal found" + std::to_string(header.maxVal));

    auto image = std::make_unique<Image<3>>(header.width, header.height);
    for (int y = 0; y < image->getHeight(); y++) {
        for (int x = 0; x < image->getWidth(); x++) {
            for (int c = 0; c < 3; c++) {
                unsigned int val;
                inputFile >> val;
                image->set(x, y, c, val);
            }
        }
    }
    inputFile.close();
    return image;
}

std::unique_ptr<Image<4>> PPMManager::readPPM(const std::string &path, const std::string &alphaPath) {
    auto base = PPMManager::readPPM(path);
    auto alpha = PGMManager::readPGM(alphaPath);
    if (base->getWidth() != alpha->getWidth() || base->getHeight() != alpha->getHeight())
        throw PPMReadException("Grayscale and Alpha images have different dimensions");

    auto final = std::make_unique<Image<4>>(base->getWidth(), base->getWidth());
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

void PPMManager::writePGM(const std::string &path, const Image<3> &image) {
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

void PPMManager::writePGM(const std::string &path, const std::string &alphaPath, const Image<4> &image) {
    auto base = std::make_unique<Image<3>>(image.getWidth(), image.getHeight());
    auto alpha = std::make_unique<Image<1>>(image.getWidth(), image.getHeight());
    for (int y = 0; y < image.getHeight(); y++) {
        for (int x = 0; x < image.getWidth(); x++) {
            base->set(x, y, 0, image.get(x, y, 0));
            base->set(x, y, 1, image.get(x, y, 1));
            base->set(x, y, 2, image.get(x, y, 2));
            alpha->set(x, y, 0, image.get(x, y, 3));
        }
    }
    PPMManager::writePGM(path, *base);
    PGMManager::writePGM(alphaPath, *alpha);
}


