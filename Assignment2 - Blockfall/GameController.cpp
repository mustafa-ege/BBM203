#include "GameController.h"
#include <fstream>
#include <sstream>

bool GameController::play(BlockFall& game, const string& commands_file){

    // TODO: Implement the gameplay here while reading the commands from the input file given as the 3rd command-line
    //       argument. The return value represents if the gameplay was successful or not: false if game over,
    //       true otherwise.

    grid_to_be_shown = game.grid;
    grid = &game.grid;
    grid_rows = (*grid).size();
    grid_cols = (*grid)[0].size();
    row = 0;
    col = 0;
    game.active_rotation = game.initial_block;
    if (game.leaderboard.head_leaderboard_entry)
        highScore = game.leaderboard.head_leaderboard_entry->score;
    else
        highScore = 0;

    score = &game.current_score;

    std::ifstream commandsFile(commands_file);
    if (!commandsFile.is_open()) {
        std::cerr << "error with command file";
        exit(-1);
    }
    clearFile( output_file);

    if (can_place_block(game.active_rotation, 0, 0)){
        place_block(game.active_rotation, 0, 0);
    }
    string command;

    while (getline(commandsFile, command)){
//        const char charsToRemove[] = {'\n', '\r','\t','\v','\f','\r'};
//        command = BlockFall::trim_string(command, charsToRemove);
        const char* whitespaces = " \t\n\r\f\v";
        command = trim(command, whitespaces);
        if (game_over_condition == "noblockleft" || game_over_condition == "cantfit"){
            break;
        }
        if (command == "PRINT_GRID") {
            print_score();
            print_grid(grid_to_be_shown);
        } else if (command == "DROP") {
            for (int i = 0; i < game.active_rotation->shape.size(); i++){
                for (int j = 0; j < game.active_rotation->shape[0].size(); j++)
                    if (game.active_rotation->shape[i][j] == 1)
                        filled_squares ++;
            }

            if (!game.gravity_mode_on){
                drop_block(game.active_rotation, game.power_up);
            } else
                drop_block_with_gravity(game.active_rotation, game.power_up);
            filled_squares = 0;
        } else if (command == "MOVE_RIGHT") {
            move_right(game.active_rotation);
        } else if (command == "MOVE_LEFT") {
            move_left(game.active_rotation);
        } else if (command == "ROTATE_RIGHT") {
            rotate_right(game.active_rotation);
        } else if (command == "ROTATE_LEFT") {
            rotate_left(game.active_rotation);
        } else if (command == "GRAVITY_SWITCH") {
            if (game.gravity_mode_on)
                game.gravity_mode_on = false;
            else {
                game.gravity_mode_on = true;
                apply_gravity_to_grid();
            }
        } else {
//            print_to_file("Unknown command: ", output_file);
//            print_to_file(command, output_file);
//            print_to_file("\n",output_file);
            cout << "Unknown command: " << command << endl;

        }
    }

    time_t currentTime;
    time(&currentTime);
    game.leaderboard.insert_new_entry(new LeaderboardEntry((*score), currentTime, game.player_name));

    game.leaderboard.write_to_file(game.leaderboard_file_name);

    if (game_over_condition == "noblockleft"){
//        print_to_file("YOU WIN!\nNo more blocks.\nFinal grid and score:\n\n",output_file);
        cout << "YOU WIN!\nNo more blocks.\nFinal grid and score:\n\n";
        print_score();
        print_grid(*grid);

        game.leaderboard.print_leaderboard();
        return true;
    } else if (game_over_condition == "cantfit") {
//        print_to_file("GAME OVER!\nNext block that couldn't fit:\n",output_file);
        cout << "GAME OVER!\nNext block that couldn't fit:\n";
        print_block(game.active_rotation->shape);
//        print_to_file("Final grid and score:\n\n",output_file);
        cout << "Final grid and score:\n\n";
        print_score();
        print_grid(grid_to_be_shown);

        game.leaderboard.print_leaderboard();
        return false;
    }
//    print_to_file("GAME FINISHED!\nNo more commands.\nFinal grid and score:\n\n",output_file);
    cout << "GAME FINISHED!\nNo more commands.\nFinal grid and score:\n\n";
    print_score();
    print_grid(*grid);
    game.leaderboard.print_leaderboard();

    commandsFile.close();
    return true;
}

bool GameController::can_place_block(const Block* block, int row, int col) {
    vector<vector<bool> > blockMatrix = block->shape;
    int rows = grid_to_be_shown.size();
    int columns = grid_to_be_shown[0].size();
    for (int i = 0; i < blockMatrix.size(); ++i) {
        for (int j = 0; j < blockMatrix[i].size(); ++j) {
            if (row + i >= rows || col + j >= columns || col < 0 || (blockMatrix[i][j] && (*grid)[row + i][col + j])) {
                return false;
            }
        }
    }
    return true;
}
void GameController::place_block(const Block* blockObject, int row, int col) {
    vector<vector<bool> > block = blockObject->shape;
    for (int i = 0; i < block.size(); ++i) {
        for (int j = 0; j < block[i].size(); ++j) {
            if (block[i][j]) {
                if (row + i >= 0 && row + i < grid_rows && col + j >= 0 && col + j < grid_cols) {
                    grid_to_be_shown[row + i][col + j] = 1;
                }
            }
        }
    }
}
void GameController::clean_block(const Block* blockObject, int row, int col) {
    vector<vector<bool> > block = blockObject->shape;
    for (int i = 0; i < block.size(); ++i) {
        for (int j = 0; j < block[i].size(); ++j) {
            if (block[i][j]) {
                if (row + i >= 0 && row + i < grid_rows && col + j >= 0 && col + j < grid_cols) {
                    grid_to_be_shown[row + i][col + j] = 0;
                }
            }
        }
    }
}
//// moving
void GameController::move_right(Block *&block) {
    if (can_place_block(block, row, col + 1)) {
        clean_block(block, row, col);
        col++;
        place_block(block, row, col);
    }
}
void GameController::move_left(Block *&block) {
    if (can_place_block(block, row, col - 1)) {
        clean_block(block, row, col);
        col --;
        place_block(block, row, col);
    }
}

//// falling
void GameController::move_down(Block *&block) {
    if (can_place_block(block, row + 1, col)) {
        clean_block(block, row, col);
        row ++;
        place_block(block, row, col);
    }
}
void GameController::drop_block(Block *&block,vector<vector<bool>> &power_up) {
    while (can_place_block(block, row +1, col)) {
        move_down(block);
        (*score) += filled_squares;
        if (*score > highScore)
            highScore = (*score);
    }
    extra_work_for_drop(block, power_up);
}

void GameController::drop_block_with_gravity(Block *&block,vector<vector<bool>>& power_up) {
    while (can_place_block(block, row +1, col)) {
        (*score) += filled_squares;
        if (*score > highScore)
            highScore = (*score);
        move_down(block);
    }
    for (int i = block->shape.size()-1; i >=0; --i) {
        for (int j = block->shape[i].size()-1; j >= 0 ; --j) {
            if (block->shape[i][j]) {
                int currentRow = row + i;
                int currentCol = col + j;
                while (currentRow < grid_rows - 1 && grid_to_be_shown[currentRow + 1][currentCol] == 0) {
                    grid_to_be_shown[currentRow][currentCol] = 0;
                    grid_to_be_shown[currentRow+1][currentCol] = 1;
                    ++currentRow;
                }
            }
        }
    }
    extra_work_for_drop(block, power_up);
}
void GameController::extra_work_for_drop(Block *&block,vector<vector<bool>>& power_up) {
    *grid = grid_to_be_shown;
    if (is_powerup(power_up)){
//        print_to_file("Before clearing:\n",output_file);
        cout << "Before clearing:\n";
        print_grid(grid_to_be_shown);
        (*score) += 1000;
        if (*score > highScore)
            highScore = (*score);
        clear_grid();
    } else {
        if (check_for_line()){
//            print_to_file("Before clearing:\n",output_file);
            cout << "Before clearing:\n";
            print_grid(grid_to_be_shown);
        }
    }
    bring_new_block(block);
    grid_to_be_shown = *grid;
    if (block == nullptr){
        game_over_condition = "noblockleft";
    } else {
        if (can_place_block(block, 0, 0)){
            place_block(block, 0, 0);
        } else {
            game_over_condition = "cantfit";
        }
    }
}

void GameController::apply_gravity_to_grid() {
    for (int i = grid_rows - 1; i >= 0; --i) {
        for (int j = 0; j < grid_cols; ++j) {
            if ((*grid)[i][j] == 1) {
                int currentRow = i;
                while (currentRow < grid_rows - 1 && (*grid)[currentRow + 1][j] == 0) {
                    (*grid)[currentRow][j] = 0;
                    (*grid)[currentRow+1][j] = 1;
                    ++currentRow;
                }
            }
        }
    }
    check_for_line();

    grid_to_be_shown = *grid;
}

void GameController::bring_new_block(Block *&block) {
    block = block->next_block;
    row = 0;
    col = 0;
}

//// power up
bool GameController::is_powerup(vector<vector<bool>> &power_up) {
    for (int i = 0; i <= grid_rows - power_up.size(); i++) {
        for (int j = 0; j <= grid_cols - power_up[0].size(); j++) {
            bool match = true;
            for (int pi = 0; pi < power_up.size(); pi++) {
                for (int pj = 0; pj < power_up[0].size(); pj++) {
                    if ((power_up[pi][pj] != (*grid)[i + pi][j + pj])) {
                        match = false;
                        break;
                    }
                }
                if (!match) break;
            }
            if (match) return true;
        }
    }
    return false;
}
void GameController::clear_grid() {
    for (int i = 0; i < (*grid).size(); i++){
        for (int j = 0; j < (*grid)[0].size(); j++)
            if ((*grid)[i][j] == 1) {
                (*score)++;
                if (*score > highScore)
                    highScore = (*score);
                (*grid)[i][j] = 0;
            }
    }
}

//// clear row
bool GameController::check_for_line() {
    bool any_line_cleared =false;
    for (int i = 0; i < grid_rows; i++) {
        bool row_full = true;
        for (int j = 0; j < grid_cols; j++) {
            if ((*grid)[i][j] == 0)
                row_full = false;
        }
        if (row_full){
            clear_full_row(i);
            any_line_cleared = true;
            (*score) += grid_cols;
            if (*score > highScore)
                highScore = (*score);
        }
    }
    return any_line_cleared;
}
void GameController::clear_full_row(int rowNumber) {
    for (int i = rowNumber; i>0; i--) {
        for (int j = 0; j < grid_cols; j++) {
            (*grid)[i][j] = (*grid)[i-1][j ];
        }
    }
}

//// rotate
void GameController::rotate_right(Block *&block) {
    if (can_place_block(block->right_rotation, row , col)) {
        clean_block(block, row, col);
        block = block->right_rotation;
        place_block(block, row, col);
    }
}
void GameController::rotate_left(Block *&block) {
    if (can_place_block(block->left_rotation, col , row)) {
        clean_block(block, row, col);
        block = block->left_rotation;
        place_block(block, row, col);
    }
}


void GameController::print_score(){
//    print_to_file("Score: ",output_file);
//    print_to_file((*score),output_file);
//    print_to_file("\nHigh Score: 5660\n",output_file);
    cout << "Score: " << (*score) << "\nHigh Score: "<< highScore << endl;
}

void GameController::print_grid(vector<vector<int> > grid){
    for (int i = 0; i < grid.size(); i++){
        for (int j = 0; j < grid[0].size(); j++){
            bool cell = grid[i][j];
            if (cell == 1)
                cout << occupiedCellChar;
//                print_to_file(occupiedCellChar,output_file);
            else if (cell == 0)
                cout << unoccupiedCellChar;
//                print_to_file(unoccupiedCellChar,output_file);

        }
//        print_to_file("\n", output_file);
        cout << endl;
    }
//    print_to_file("\n\n",output_file);
    cout << endl << endl;
};
void GameController::print_block(vector<vector<bool> > grid){
    for (int i = 0; i < grid.size(); i++){
        for (int j = 0; j < grid[0].size(); j++){
            bool cell = grid[i][j];
            if (cell == 1)
//                print_to_file(occupiedCellChar,output_file);
                cout << occupiedCellChar;
            else if (cell == 0)
//                print_to_file(unoccupiedCellChar,output_file);
                cout << unoccupiedCellChar;
        }
        cout << endl;
//        print_to_file("\n", output_file);
    }
    cout << endl;
//    print_to_file("\n",output_file);
};


void GameController::print_to_file(const string& text, const string& filename){
    std::ofstream outputFile(filename,std::ios::app);

    if (!outputFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(-1);

    }
    outputFile << text;
//    cout<< text;

};
//void GameController::print_to_file(const int& text, const string& filename){
//    std::ofstream outputFile(filename,std::ios::app);
//    if (!outputFile.is_open()) {
//        std::cerr << "Error opening file: " << filename << std::endl;
//        exit(-1);
//    }
//    outputFile << text;
////    cout<< text;
//};

void GameController::clearFile(const std::string& filename) {
    std::ofstream outputFile(filename, std::ios::trunc);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
}

inline std::string& GameController::rtrim(std::string& s, const char* t){
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}
inline std::string& GameController::ltrim(std::string& s, const char* t){
    s.erase(0, s.find_first_not_of(t));
    return s;
}
inline std::string& GameController::trim(std::string& s, const char* t){
    return ltrim(rtrim(s, t), t);
}

