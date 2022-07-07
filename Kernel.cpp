//
// Created by edo on 07/07/22.
//

#include <cstdarg>
#include <cmath>
#include <list>
#include "Kernel.h"

Kernel::Kernel(std::initializer_list<float> list) : values(list) {
    auto dim = sqrt(values.size());
    if (std::floor(dim) != dim || int(dim) % 2 != 1) throw WrongSizeKernelException();
};

Kernel::Kernel(int dimension, float initValue) : values(pow(dimension, 2), initValue) {
    if (dimension % 2 != 1) throw WrongSizeKernelException();
}

int Kernel::getMatrixSize() const {
    return int(sqrt(values.size()));
}

float Kernel::getValue(int x, int y) const {
    return values[getMatrixSize() * y + x];
}

float Kernel::getCenteredValue(int x, int y) const {
    int size = getMatrixSize();
    return getValue(x + (size / 2), y + (size / 2));
}

GaussBlurKernel::GaussBlurKernel(const float stdDev, int dimension) : Kernel(dimension) {
    int linearDimension = pow(dimension, 2);

    for (int y = 0; y < dimension; y++) {
        for (int x = 0; x < dimension; x++) {
            int centeredX = x - (dimension / 2);
            int centeredY = y - (dimension / 2);
            values[y * dimension + x] =
                    (1 / (2 * M_PI * pow(stdDev, 2))) *
                    exp(-(pow(centeredX, 2) + pow(centeredY, 2)) / (2 * pow(stdDev, 2)));
        }
    }
}
