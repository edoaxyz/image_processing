//
// Created by edo on 11/07/22.
//

#include "PXXManager.h"

std::istream &PXXManager::PXXHeader::operator<<(std::istream &inputStream) {
    PXXManager::getLine(inputStream, magicNumber);

    std::string dimensions;
    PXXManager::getLine(inputStream, dimensions);
    width = stoi(dimensions.substr(0, dimensions.find(' ')));
    height = stoi(dimensions.substr(dimensions.find(' ')));

    std::string tempMaxVal;
    PXXManager::getLine(inputStream, tempMaxVal);
    this->maxVal = stoi(tempMaxVal);
    return inputStream;
}

std::ostream &PXXManager::PXXHeader::operator>>(std::ostream &outputStream) const {
    outputStream << magicNumber << '\n';
    outputStream << width << ' ' << height << '\n';
    outputStream << maxVal << '\n';
    return outputStream;
}

