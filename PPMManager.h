//
// Created by edo on 09/07/22.
//

#ifndef IMAGE_PROCESSING_PPMMANAGER_H
#define IMAGE_PROCESSING_PPMMANAGER_H

#include <memory>
#include "Image.h"

namespace PXXManager {
    class PPMException {
    public:
        PPMException() = default;

        virtual ~PPMException() = default;
    };

    class PPMReadException : public PPMException, public std::runtime_error {
    public:
        PPMReadException(std::string msg) : std::runtime_error(msg) {};

        ~PPMReadException() override = default;
    };

    class PPMWriteException : public PPMException, public std::runtime_error {
    public:
        PPMWriteException(std::string msg) : std::runtime_error(msg) {};

        ~PPMWriteException() override = default;
    };

    std::unique_ptr<RGBImage> readPPM(const std::string &path);

    std::unique_ptr<RGBAImage> readPPM(const std::string &path, const std::string &alphaPath);

    void writePPM(const std::string &path, const RGBImage &image);

    void writePPM(const std::string &path, const std::string &alphaPath, const RGBAImage &image);
}


#endif //IMAGE_PROCESSING_PPMMANAGER_H
