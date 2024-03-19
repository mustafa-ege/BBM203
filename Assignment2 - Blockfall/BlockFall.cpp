#include "BlockFall.h"
#include <fstream>
#include <sstream>
#include <iostream>

BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on,
                     const string &leaderboard_file_name, const string &player_name) : gravity_mode_on (
        gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}

void BlockFall::read_blocks(const string &input_file) {
    // TODO: Read the blocks from the input file and initialize "initial_block" and "active_block" member variables
    // TODO: For every block, generate its rotations and properly implement the multilevel linked list structure
    //       that represents the game blocks, as explained in the PA instructions.
    // TODO: Initialize the "power_up" member variable as the last block from the input file (do not add it to the linked list!)
    std::ifstream blocksFile(input_file);
    if (!blocksFile.is_open()) {
        std::cerr << "error with blocks file";
        exit(-1);
    }

    //// Input as a vector<string> instead of .dat file
    string line;
    vector<string> inputAsVector;
    while (std::getline(blocksFile, line)){
        if (line.find_first_of("[") != std::string::npos) {
            inputAsVector.push_back("[");
//            const char charsToRemove[] = {'['};
//            line = trim_string(line, charsToRemove);
            line = trim(line, "[");
        }
        if (line.find("]") != std::string::npos){
//            const char charsToRemove[] = {']','\n', '\r','\t','\v','\f','\r'};
//            line = trim_string(line, charsToRemove);
            line = trim(line, whitespaces);
            line = trim(line, "]");
            inputAsVector.push_back(line);
            inputAsVector.push_back("]");
        } else if (line.find_first_not_of(" \t\n\v\f\r") != std::string::npos) {
//            const char charsToRemove[] = {'\n', '\r','\t','\v','\f','\r'};
            line = trim(line,whitespaces);
//            line = trim_string(line, charsToRemove);
            inputAsVector.push_back(line);
        }
    }
    blocksFile.close();

    int numberOfBlocks = 0;
    for (string i : inputAsVector){
        if (i == "["){
            numberOfBlocks++;
        }
    }

    //// Linked list with blocks
    vector<vector<bool>> block;
    vector<bool> blockRow;
    int numberOfBlocksAdded = 0;
    for (const string& blockOrBracket : inputAsVector){
        blockRow.clear();
        //// power up
        if (numberOfBlocksAdded == (numberOfBlocks-1)){
            if (blockOrBracket == "[")
                continue;
            if (blockOrBracket == "]")
                break;
            vector<bool> powerUpRow;
            for (char c : blockOrBracket) {
//                cout<< c ;
                powerUpRow.push_back(c == '1');
            }
            power_up.push_back(powerUpRow);
        } else {
            if (blockOrBracket == "[") {
                block.clear();
                continue;
            } else if (blockOrBracket == "]") {
                if (!initial_block) {
                    initial_block = new Block(block);
                } else {
                    Block *currentBlock = initial_block;
                    while (currentBlock->next_block) {
                        currentBlock = currentBlock->next_block;
                    }
                    currentBlock->next_block = new Block(block);
                }
                numberOfBlocksAdded++;
                continue;
            } else {
                for (char c: blockOrBracket)
                    blockRow.push_back(c == '1');
                block.push_back(blockRow);
            }
        }
    }


    Block *currentBlock = initial_block;
    while (currentBlock) {
        rotate_block(currentBlock,currentBlock);
        currentBlock = currentBlock->next_block;
    }
}

vector<vector<bool> > BlockFall::rotate_matrix_right(const vector<vector<bool> >& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    std::vector<std::vector<bool> > rotatedMatrix(cols, std::vector<bool>(rows, false));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            rotatedMatrix[j][rows - 1 - i] = matrix[i][j];
        }
    }
    return rotatedMatrix;
}

void BlockFall::rotate_block(Block*& block_without_rotation, Block*& first_block){
    vector<vector<bool> > rotated_matrix = rotate_matrix_right(block_without_rotation->shape);
    if (block_without_rotation->shape == rotated_matrix){
        block_without_rotation->right_rotation = block_without_rotation;
        block_without_rotation->left_rotation = block_without_rotation;
    } else if (first_block->shape == rotated_matrix) {
        block_without_rotation->right_rotation = first_block;
        first_block->left_rotation = block_without_rotation;
        block_without_rotation->next_block = first_block->next_block;
    } else {
        Block *newBlock = new Block(rotated_matrix);
        newBlock->left_rotation = block_without_rotation;
        newBlock->right_rotation = block_without_rotation->right_rotation;
        block_without_rotation->right_rotation = newBlock;
        if (first_block->next_block == nullptr){
        } else {
            newBlock->next_block = first_block->next_block;
        }
        rotate_block(newBlock, first_block);
    }
}


void BlockFall::initialize_grid(const string &input_file) {
    // TODO: Initialize "rows" and "cols" member variables
    // TODO: Initialize "grid" member variable using the command-line argument 1 in main

    std::ifstream gridFile(input_file);
    if (!gridFile.is_open()) {
        std::cerr << "error with grid file";
        exit(-1);
    }

    string line;
    while (std::getline(gridFile, line)) {
        rows++;
        std::stringstream lineStream(line);
        int value;
        vector<int> row;
        while (lineStream >> value) {
            cols++;
            row.push_back(value);
        }
        grid.push_back(row);
    }
    cols /= rows;
    gridFile.close();
}
//

std::string BlockFall::trim_string(const std::string& input, const char* charsToRemove) {
    std::string result = input;
    for (int i = 0; charsToRemove[i] != '\0'; ++i) {
        char c = charsToRemove[i];
        size_t found = result.find(c);
        while (found != std::string::npos) {
            result.erase(found, 1);
            found = result.find(c);
        }
    }
    return result;
}

BlockFall::~BlockFall() {
    // TODO: Free dynamically allocated memory used for storing game blocks
    Block* verticalBlock = initial_block;
    while (verticalBlock) {
        while (verticalBlock->right_rotation && verticalBlock->right_rotation != verticalBlock) {
            Block* horizontalBlock = verticalBlock->right_rotation;
            horizontalBlock->right_rotation->left_rotation = verticalBlock;
            verticalBlock->right_rotation = horizontalBlock->right_rotation;
            delete horizontalBlock;
        }
        Block* nextBlock = verticalBlock->next_block;
        delete verticalBlock;
        verticalBlock = nextBlock;
    }
}
//// trimming white spaces from input taken from samples
//const char* whitespaces = " \t\n\r\f\v";
inline std::string& BlockFall::rtrim(std::string& s, const char* t){
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}
inline std::string& BlockFall::ltrim(std::string& s, const char* t){
    s.erase(0, s.find_first_not_of(t));
    return s;
}
inline std::string& BlockFall::trim(std::string& s, const char* t){
    return ltrim(rtrim(s, t), t);
}

