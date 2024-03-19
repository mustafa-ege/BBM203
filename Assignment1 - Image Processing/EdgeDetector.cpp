// EdgeDetector.cpp

#include "EdgeDetector.h"
#include <cmath>

#include <cmath>

// Default constructor
EdgeDetector::EdgeDetector() {
    double sobel_gx[3][3] = {
            {-1.0, 0.0, 1.0},
            {-2.0, 0.0, 2.0},
            {-1.0, 0.0, 1.0}
    };
    double sobel_gy[3][3] = {
            {-1.0, -2.0, -1.0},
            {0.0, 0.0, 0.0},
            {1.0, 2.0, 1.0}
    };

    gx = new double*[3];
    gy = new double*[3];
    for (int i = 0; i < 3; i++) {
        gx[i] = new double[3];
        gy[i] = new double[3];
        for (int j = 0; j < 3; j++) {
            gx[i][j] = sobel_gx[i][j];
            gy[i][j] = sobel_gy[i][j];
        }
    }
}

// Destructor
EdgeDetector::~EdgeDetector() {
    for (int i = 0; i < 3; i++) {
        delete[] gx[i];
        delete[] gy[i];
    }
    delete[] gx;
    delete[] gy;
}

// Detect Edges using the given algorithm
std::vector<std::pair<int, int>> EdgeDetector::detectEdges(const ImageMatrix& input_image) {
    std::vector<std::pair<int, int>> edgePixels;

    Convolution convolveX(gx, 3, 3, 1, 1);
    Convolution convolveY(gy, 3, 3, 1, 1);

    ImageMatrix Ix = convolveX.convolve(input_image);
    ImageMatrix Iy = convolveY.convolve(input_image);

    int height = input_image.get_height(), width = input_image.get_width();
    ImageMatrix magnitudeMatrix(height, width);
    double sum = 0;
    int totalPixels = height * width;
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            double x = Ix.get_data(i,j);
            double y = Iy.get_data(i,j);
            double magnitude = sqrt((x * x) + (y * y));
            magnitudeMatrix.get_data()[i][j] = magnitude;
            sum += magnitude;
        }
    }
    double average = sum / totalPixels;
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if (magnitudeMatrix.get_data(i,j) > average)
                edgePixels.push_back(std::make_pair(i,j));
        }
    }
    return edgePixels;
}

