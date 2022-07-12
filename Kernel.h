//
// Created by edo on 07/07/22.
//

#ifndef IMAGE_PROCESSING_KERNEL_H
#define IMAGE_PROCESSING_KERNEL_H

#include <vector>
#include <stdexcept>

class WrongSizeKernelException : public std::invalid_argument {
public:
    WrongSizeKernelException() : std::invalid_argument("Kernel must be a square matrix with an odd dimension.") {};

    virtual ~WrongSizeKernelException() {};
};

class OutOfRangeKernelException : public std::out_of_range {
public:
    OutOfRangeKernelException() : std::out_of_range("Wrong X,Y matrix coordinates") {};

    virtual ~OutOfRangeKernelException() {};
};

class Kernel {
public:
    explicit Kernel(std::initializer_list<float> list);

    explicit Kernel(int dimension, float initValue = 1);

    virtual ~Kernel() = default;

    int getMatrixSize() const;

    float getValue(int x, int y) const;

    float getCenteredValue(int x, int y) const;

protected:
    std::vector<float> values;
    int matrixSize;
};

class IdentityKernel : public Kernel {
public:
    IdentityKernel() : Kernel{0, 0, 0,
                              0, 1, 0,
                              0, 0, 0} {};

    virtual ~IdentityKernel() {};
};

class Edge0Kernel : public Kernel {
public:
    Edge0Kernel() : Kernel{1, 0, -1,
                           0, 0, 0,
                           -1, 0, 1} {};

    virtual ~Edge0Kernel() {};
};

class Edge1Kernel : public Kernel {
public:
    Edge1Kernel() : Kernel{0, 1, 0,
                           1, -4, 1,
                           0, 1, 0} {};

    virtual ~Edge1Kernel() {};
};

class Edge2Kernel : public Kernel {
public:
    Edge2Kernel() : Kernel{-1, -1, -1,
                           -1, 8, -1,
                           -1, 1, -1} {};

    virtual ~Edge2Kernel() {};
};

class SharpenKernel : public Kernel {
public:
    SharpenKernel() : Kernel{0, -1, 0,
                             -1, 5, -1,
                             0, -1, 0} {};

    virtual ~SharpenKernel() {};
};

class GaussBlurKernel : public Kernel {
public:
    explicit GaussBlurKernel(float stdDev, int dimension = 3);

    virtual ~GaussBlurKernel() {};
};

#endif //IMAGE_PROCESSING_KERNEL_H
