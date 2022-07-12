//
// Created by edo on 11/07/22.
//

#ifndef IMAGE_PROCESSING_PXXMANAGER_H
#define IMAGE_PROCESSING_PXXMANAGER_H

#include <istream>

class PXXReader {
public:
    static void getLine(std::istream &inputStream, std::string &line) {
        while (!(!line.empty() && line[0] == '#')) {
            getline(inputStream, line, '\n');
        }
    }
};

class PXXHeader {
public:
    PXXHeader() = default;

    PXXHeader(const std::string &magicNumber, unsigned int width, unsigned int height, unsigned short maxVal)
            : magicNumber(magicNumber), width(width), height(height), maxVal(maxVal) {};

    std::istream &operator<<(std::istream &inputStream);

    std::ostream &operator>>(std::ostream &outputStream) const;

    std::string magicNumber;
    unsigned int width;
    unsigned int height;
    unsigned short int maxVal;

};


#endif //IMAGE_PROCESSING_PXXMANAGER_H
