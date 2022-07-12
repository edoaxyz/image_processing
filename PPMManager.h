//
// Created by edo on 09/07/22.
//

#ifndef IMAGE_PROCESSING_PPMMANAGER_H
#define IMAGE_PROCESSING_PPMMANAGER_H

#include <memory>
#include "Image.h"

class PPMException : public std::exception {
public:
    PPMException() : std::exception() {};

    virtual ~PPMException() {};
};

class PPMReadException : public PPMException, public std::runtime_error {
public:
    PPMReadException(std::string msg) : std::runtime_error(msg) {};

    virtual ~PPMReadException() {};
};

class PPMWriteException : public PPMException, public std::runtime_error {
public:
    PPMWriteException(std::string msg) : std::runtime_error(msg) {};

    virtual ~PPMWriteException() {};
};

class PPMManager {
public:
    static std::unique_ptr<Image<3>> readPPM(const std::string &path);

    static std::unique_ptr<Image<4>> readPPM(const std::string &path, const std::string &alphaPath);

    static void writePGM(const std::string &path, const Image<3> &image);

    static void writePGM(const std::string &path, const std::string &alphaPath, const Image<4> &image);
};


#endif //IMAGE_PROCESSING_PPMMANAGER_H
