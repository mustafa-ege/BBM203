#include "ImageProcessor.h"
#include <iostream>

ImageProcessor::ImageProcessor() {

}

ImageProcessor::~ImageProcessor() {

}

std::string ImageProcessor::decodeHiddenMessage(const ImageMatrix &img) {
    ImageSharpening sharpener;
    ImageMatrix sharpenedImage = sharpener.sharpen(img, 2);
    EdgeDetector edgeDetector;
    std::vector<std::pair<int, int>> edgePairs = edgeDetector.detectEdges(sharpenedImage);
    DecodeMessage decoder;
    std::string decodedMessage = decoder.decodeFromImage(sharpenedImage, edgePairs);
    return decodedMessage;
}

ImageMatrix ImageProcessor::encodeHiddenMessage(const ImageMatrix &img, const std::string &message) {
    ImageSharpening sharpener;
    ImageMatrix sharpenedImage = sharpener.sharpen(img, 2);
    EdgeDetector edgeDetector;
    std::vector<std::pair<int, int>> edgePairs = edgeDetector.detectEdges(sharpenedImage);
    EncodeMessage encoder;
    ImageMatrix encodedMatrix = encoder.encodeMessageToImage(img, message, edgePairs);
    return encodedMatrix;
}
