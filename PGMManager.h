//
// Created by edo on 09/07/22.
//

#ifndef IMAGE_PROCESSING_PGMMANAGER_H
#define IMAGE_PROCESSING_PGMMANAGER_H

#include <memory>
#include "Image.h"

namespace PXXManager {
    class PGMException {
    public:
        PGMException() = default;

        virtual ~PGMException() = default;
    };

    class PGMReadException : public PGMException, public std::runtime_error {
    public:
        explicit PGMReadException(std::string msg) : std::runtime_error(msg) {};

        ~PGMReadException() override = default;
    };

    class PGMWriteException : public PGMException, public std::runtime_error {
    public:
        explicit PGMWriteException(std::string msg) : std::runtime_error(msg) {};

        ~PGMWriteException() override = default;
    };

    std::unique_ptr<GImage> readPGM(const std::string &path);

    std::unique_ptr<GAImage> readPGM(const std::string &path, const std::string &alphaPath);

    void writePGM(const std::string &path, const GImage &image);

    void writePGM(const std::string &path, const std::string &alphaPath, const GAImage &image);
}

#endif //IMAGE_PROCESSING_PGMMANAGER_H
