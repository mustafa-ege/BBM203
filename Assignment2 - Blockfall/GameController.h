#ifndef PA2_GAMECONTROLLER_H
#define PA2_GAMECONTROLLER_H

#include "BlockFall.h"

using namespace std;

class GameController {
public:
    bool play(BlockFall &game, const string &commands_file); // Function that implements the gameplay

    string game_over_condition;
    vector<vector<int> > grid_to_be_shown;
    vector<vector<int> > *grid;

    int filled_squares = 0;
    unsigned long * score;
    int highScore;
//    vector<vector<bool> > power_up;

    string output_file = "output.txt";
    int row = 0;
    int col = 0;

    int grid_rows = 0;
    int grid_cols = 0;

    bool can_place_block(const Block* block, int row, int col);
    void place_block(const Block* blockObject, int row, int col);
    void clean_block(const Block* blockObject, int row, int col);
    void bring_new_block(Block *&block);
    void move_right(Block *&block);
    void move_left(Block *&block);
    void move_down(Block *&block);
    void rotate_right(Block *&block);
    void rotate_left(Block *&block);

    void drop_block(Block *&block,vector<vector<bool>>& power_up);
    void drop_block_with_gravity(Block *&block,vector<vector<bool>>& power_up);
    void extra_work_for_drop(Block *&block,vector<vector<bool>>& power_up);
    bool is_powerup(vector<vector<bool>>& power_up);
    void apply_gravity_to_grid();
    void clear_grid();
    bool check_for_line();
    void clear_full_row(int rowNumber);

    void print_grid(vector<vector<int> > grid);
    void print_block(vector<vector<bool> > grid);
    void print_score();

    void print_to_file(const string& text, const string& filename);
    void print_to_file(const int& text, const string& filename);
    void clearFile(const std::string& filename);

    const char* whitespaces = " \t\n\r\f\v";
    inline std::string& rtrim(std::string& s, const char* t);
    inline std::string& ltrim(std::string& s, const char* t);
    inline std::string& trim(std::string& s, const char* t);
};


#endif //PA2_GAMECONTROLLER_H
