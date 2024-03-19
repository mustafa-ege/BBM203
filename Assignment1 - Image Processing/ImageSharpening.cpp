#include "ImageSharpening.h"

// Default constructor
ImageSharpening::ImageSharpening() {
    kernel_height = kernel_width = 3;
    blurringKernel = new double*[kernel_height];
    for (int i = 0; i < kernel_height; i++){
        blurringKernel[i] = new double[kernel_width];
        for (int j = 0; j < kernel_width; j++)
            blurringKernel[i][j] = (1.0/9.0);
    }
}

ImageSharpening::~ImageSharpening(){
    for (int i = 0; i < kernel_height; i++) {
        delete[] blurringKernel[i];
    }
    delete[] blurringKernel;
}

ImageMatrix ImageSharpening::sharpen(const ImageMatrix& input_image, double k) {
    Convolution blurConvolve(blurringKernel, kernel_height, kernel_width, 1, 1);
    ImageMatrix blurredImage = blurConvolve.convolve(input_image);
    ImageMatrix sharpenedImage(input_image + ((input_image - blurredImage) * k));
    for (int i = 0; i < sharpenedImage.get_height(); i++){
        for (int j = 0; j < sharpenedImage.get_width(); j++){
            double* valueToCheck = &sharpenedImage.get_data()[i][j];
            if (*valueToCheck < 0)
                *valueToCheck = 0;
            else if(*valueToCheck > 255)
                *valueToCheck = 255;
        }
    }
    return sharpenedImage;
}
