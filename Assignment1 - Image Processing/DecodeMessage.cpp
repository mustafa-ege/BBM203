// DecodeMessage.cpp

#include "DecodeMessage.h"

// Default constructor
DecodeMessage::DecodeMessage() {
    // Nothing specific to initialize here
}

// Destructor
DecodeMessage::~DecodeMessage() {
    // Nothing specific to clean up
}

char DecodeMessage::binaryToASCII(const std::string& binaryString){
    char asciiChar = 0;
    for (int i = 0; i < binaryString.length(); i++)
        asciiChar = (asciiChar << 1) | (binaryString[i] - '0');

    if (asciiChar <= 32)
        asciiChar += 33;
    else if (asciiChar >= 127)
        asciiChar = 126;
    return asciiChar;
}

std::string padToSevenBits(std::string binaryString){
    std::string paddedString = binaryString;
    while (paddedString.length() % 7 != 0)
        paddedString = "0" + paddedString;
    return paddedString;
}

std::string DecodeMessage::decodeFromImage(const ImageMatrix& image, const std::vector<std::pair<int, int>>& edgePixels) {
    std::string binaryMessage;
    for (const std::pair<int,int>& pixel : edgePixels){
        int lsb = image.get_least_significant_bit(pixel.first, pixel.second);
        binaryMessage += lsb;
    }
    std::string decodedMessage;
    binaryMessage = padToSevenBits(binaryMessage);

    for (int i = 0; i < binaryMessage.length(); i += 7) {
        std::string binaryPart = binaryMessage.substr(i, 7);
        char asciiChar = binaryToASCII(binaryPart);
        decodedMessage += asciiChar;
    }

    return decodedMessage;
}