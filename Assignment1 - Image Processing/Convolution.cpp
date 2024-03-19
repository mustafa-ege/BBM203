#include <iostream>

#include "Convolution.h"

// Default constructor 
Convolution::Convolution() {
}

// Parametrized constructor for custom kernel and other parameters
Convolution::Convolution(double** customKernel, int kh, int kw, int stride_val, bool pad){
    stride = stride_val;
    padding = pad;
    kernel = new ImageMatrix(customKernel,kh, kw);
}

// Destructor
Convolution::~Convolution() {
    delete kernel;
}

// Copy constructor
Convolution::Convolution(const Convolution &other){
    stride = other.stride;
    padding = other.padding;
    kernel = new ImageMatrix(*(other.kernel));
}

// Copy assignment operator
Convolution& Convolution::operator=(const Convolution &other) {
    if (this == &other)
        return *this;
    delete kernel;
    kernel = new ImageMatrix(*(other.kernel));
    padding = other.padding;
    stride = other.stride;
    return *this;
}

// Convolve Function: Responsible for convolving the input image with a kernel and return the convolved image.
ImageMatrix Convolution::convolve(const ImageMatrix& input_image) const {
    ImageMatrix paddedImage = matrixPadding(input_image);
    int kernelHeight = kernel->get_height();
    int kernelWidth = kernel->get_width();
    int paddedHeight = paddedImage.get_height();
    int paddedWidth = paddedImage.get_width();
    int convolvedHeight = (paddedImage.get_height() - kernelHeight) / stride + 1;
    int convolvedWidth = (paddedImage.get_width() - kernelWidth) / stride + 1;

    ImageMatrix convolvedImage(convolvedHeight, convolvedWidth);

    for (int i = 0; i < convolvedHeight; i++) {
        for (int j = 0; j < convolvedWidth; j++) {
            for (int k = 0; k < kernelHeight; k++) {
                for (int l = 0; l < kernelWidth; l++) {
                    int x = i * stride + k;
                    int y = j * stride + l;
                    if ( x < paddedHeight && y < paddedWidth)
                        convolvedImage.get_data()[i][j] += paddedImage.get_data(x, y) * kernel->get_data(k, l);
                }
            }
        }
    }
    return convolvedImage;
}

ImageMatrix Convolution::matrixPadding(const ImageMatrix& input_image) const {
    if (padding == 0)
        return input_image;
    else {
        int inputHeight = input_image.get_height();
        int inputWidth = input_image.get_width();
        int paddedHeight = inputHeight + 2 * padding;
        int paddedWidth = inputWidth + 2 * padding;
        ImageMatrix paddedMatrix(paddedHeight, paddedWidth);
        for (int i = 0; i < inputHeight; i++){
            for (int j = 0; j < inputWidth; j++)
                paddedMatrix.get_data()[i+1][j+1] = input_image.get_data()[i][j];
        }
        return paddedMatrix;
    }
}

ImageMatrix* Convolution::getKernel() const {
    return kernel;
}
