//
// Created by edo on 07/07/22.
//

#include <cstdarg>
#include <cmath>
#include <list>
#include "Kernel.h"

Kernel::Kernel(std::initializer_list<float> list) : values(list) {
    auto dim = sqrt(values.size());
    matrixSize = dim;
    if (std::floor(sqrt(values.size())) != dim || int(dim) % 2 != 1) throw WrongSizeKernelException();
};

Kernel::Kernel(int dimension, float initValue) : matrixSize(dimension), values(pow(dimension, 2), initValue) {
    if (dimension % 2 != 1) throw WrongSizeKernelException();
}

int Kernel::getMatrixSize() const {
    return matrixSize;
}

float Kernel::getValue(int x, int y) const {
    if (x < 0 || x >= matrixSize || y < 0 || y >= matrixSize) throw OutOfRangeKernelException();
    return values[matrixSize * y + x];
}

float Kernel::getCenteredValue(int x, int y) const {
    return getValue(x + (matrixSize / 2), y + (matrixSize / 2));
}

GaussBlurKernel::GaussBlurKernel(float stdDev, int dimension) : Kernel(dimension) {
    double sum = 0;
    for (int y = 0; y < dimension; y++) {
        for (int x = 0; x < dimension; x++) {
            int centeredX = x - (dimension / 2);
            int centeredY = y - (dimension / 2);
            values[y * dimension + x] =
                    (1 / (2 * M_PI * pow(stdDev, 2))) *
                    exp(-(pow(centeredX, 2) + pow(centeredY, 2)) / (2 * pow(stdDev, 2)));
            sum += values[y * dimension + x];
        }
    }

    for (int y = 0; y < dimension; y++) {
        for (int x = 0; x < dimension; x++) {
            values[y * dimension + x] /= sum;
        }
    }

}
