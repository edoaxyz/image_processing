//
// Created by edo on 11/07/22.
//

#include "PXXManager.h"

std::istream &PXXHeader::operator<<(std::istream &inputStream) {
    PXXReader::getLine(inputStream, magicNumber);

    std::string dimensions;
    PXXReader::getLine(inputStream, dimensions);
    width = stoi(dimensions.substr(0, dimensions.find(' ')));
    height = stoi(dimensions.substr(0, dimensions.find(' ')));

    std::string tempMaxVal;
    PXXReader::getLine(inputStream, tempMaxVal);
    this->maxVal = stoi(tempMaxVal);
    return inputStream;
}

std::ostream &PXXHeader::operator>>(std::ostream &outputStream) const {
    outputStream << magicNumber << '\n';
    outputStream << width << ' ' << height << '\n';
    outputStream << maxVal << '\n';
    return outputStream;
}

