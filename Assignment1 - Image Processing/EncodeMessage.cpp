#include "EncodeMessage.h"
#include <cmath>

#include <iostream>
#include <bitset>

// Default Constructor
EncodeMessage::EncodeMessage() {
}

// Destructor
EncodeMessage::~EncodeMessage() {
}

bool isPrime(int n){
    if (n <= 1)
        return false;
    for (int i = 2; i <= std::sqrt(n); i++)
        if (n % i == 0)
            return false;
    return true;
}

std::vector<int> fibonacci(int n) {
    std::vector<int> fibonacciSequence;
    int a = 0, b = 1;
    for (int i = 0; i < n; ++i) {
        fibonacciSequence.push_back(a);
        int next = a + b;
        a = b;
        b = next;
    }
    return fibonacciSequence;
}
std::string asciiToBinary(std::string inputMsg){
    std::string binaryString = "";
    for (char c : inputMsg) {
        std::bitset<7> binaryChar(c);
        binaryString += binaryChar.to_string();
    }
    return binaryString;
}

// Function to encode a message into an image matrix
ImageMatrix EncodeMessage::encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& positions) {
    ImageMatrix encodedMatrix = img;
    std::vector<int> fibSequence = fibonacci(message.length());
    std::string modifiedMessage = message;
    for (int i = 0; i < message.length(); i++){
        if (isPrime(i)){
            int newValue = static_cast<int>(message[i]) + fibSequence[i];
            if (newValue <= 32)
                newValue += 33;
            else if (newValue >= 127)
                newValue = 126;
            modifiedMessage[i] = static_cast<char>(newValue);
        }
    }
    int shiftingValue = message.length() / 2;
    std::string shiftedMessage = modifiedMessage;

    for (int i = 0; i < message.length(); i++) {
        int newPosition = (i + shiftingValue) % message.length();
        shiftedMessage[newPosition] = modifiedMessage[i];
    }

    std::string binaryMessage = asciiToBinary(shiftedMessage);
    for (int i = 0; i < positions.size(); i++) {
        std::pair<int, int> position = positions[i];
        char bitValue = binaryMessage[i];
        if (bitValue == '0')
            encodedMatrix.set_least_significant_bit(position.first, position.second, 0);
        else if (bitValue == '1')
            encodedMatrix.set_least_significant_bit(position.first, position.second, 1);
    }
    return encodedMatrix;
}
