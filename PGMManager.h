//
// Created by edo on 09/07/22.
//

#ifndef IMAGE_PROCESSING_PGMMANAGER_H
#define IMAGE_PROCESSING_PGMMANAGER_H


#include <memory>
#include "Image.h"

class PGMException : public std::exception {
public:
    PGMException() : std::exception() {};

    virtual ~PGMException() {};
};

class PGMReadException : public PGMException, public std::runtime_error {
public:
    PGMReadException(std::string msg) : std::runtime_error(msg) {};

    virtual ~PGMReadException() {};
};

class PGMWriteException : public PGMException, public std::runtime_error {
public:
    PGMWriteException(std::string msg) : std::runtime_error(msg) {};

    virtual ~PGMWriteException() {};
};

class PGMManager {
public:
    static std::unique_ptr<Image<1>> readPGM(const std::string &path);

    static std::unique_ptr<Image<2>> readPGM(const std::string &path, const std::string &alphaPath);

    static void writePGM(const std::string &path, const Image<1> &image);

    static void writePGM(const std::string &path, const std::string &alphaPath, const Image<2> &image);
};


#endif //IMAGE_PROCESSING_PGMMANAGER_H
