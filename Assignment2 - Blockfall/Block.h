#ifndef PA2_BLOCK_H
#define PA2_BLOCK_H

#include <vector>
#include <iostream>

using namespace std;

class Block {
public:

    vector<vector<bool>> shape; // Two-dimensional vector corresponding to the block's shape
    Block * right_rotation = nullptr; // Pointer to the block's clockwise neighbor block (its right rotation)
    Block * left_rotation = nullptr; // Pointer to the block's counter-clockwise neighbor block (its left rotation)
    Block * next_block = nullptr; // Pointer to the next block to appear in the game

    Block(vector<vector<bool>> shape) : shape(shape) {}

    bool operator==(const Block& other) const {
        // TODO: Overload the == operator to compare two blocks based on their shapes
        if (this->shape.size() != other.shape.size() || this->shape[0].size() != other.shape[0].size())
            return false;
        for (int i = 0; i < shape.size(); i++) {
            for (int j = 0; j < shape[i].size(); j++) {
                if (shape[i][j] != other.shape[i][j])
                    return false;
            }
        }
        return true;

    }

    bool operator!=(const Block& other) const {
        // TODO: Overload the != operator to compare two blocks based on their shapes
        return !(*this == other);
    }


    void print(){
        for (int i = 0; i < shape.size(); i++){
            for (int j = 0; j < shape[0].size(); j++)
                std::cout << shape[i][j];
            cout << endl;
        }
    }

};


#endif //PA2_BLOCK_H
