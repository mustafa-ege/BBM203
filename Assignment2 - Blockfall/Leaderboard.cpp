#include "Leaderboard.h"
#include <fstream>
#include <sstream>

void Leaderboard::insert_new_entry(LeaderboardEntry * new_entry) {
    // TODO: Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
    //       is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
    //       top 10 all-time high-scores should be kept in descending order by the score).

    if (!head_leaderboard_entry) {
        head_leaderboard_entry = new_entry;
        head_leaderboard_entry->next_leaderboard_entry = nullptr;
        return;
    }

    LeaderboardEntry *currentEntry = head_leaderboard_entry;
    while (currentEntry){
        if (new_entry->score > currentEntry->score){
            head_leaderboard_entry = new_entry;
            new_entry->next_leaderboard_entry = currentEntry;
            break;
        } else if (currentEntry->next_leaderboard_entry && new_entry->score == currentEntry->next_leaderboard_entry->score){
            currentEntry = currentEntry->next_leaderboard_entry;
            continue;
        }else if (currentEntry->next_leaderboard_entry && new_entry->score > currentEntry->next_leaderboard_entry->score){
            new_entry->next_leaderboard_entry = currentEntry->next_leaderboard_entry;
            currentEntry->next_leaderboard_entry = new_entry;
            break;
        } else if (!currentEntry->next_leaderboard_entry){
            currentEntry->next_leaderboard_entry = new_entry;
            new_entry->next_leaderboard_entry = nullptr;
            break;
        }
        if (currentEntry->next_leaderboard_entry)
            currentEntry = currentEntry->next_leaderboard_entry;
    }
    int number_of_entries = 0;
    currentEntry = head_leaderboard_entry;
    while (currentEntry){
        number_of_entries++;
        if (number_of_entries >= 10){
            delete currentEntry->next_leaderboard_entry;
            currentEntry->next_leaderboard_entry = nullptr;
            break;
        }
        currentEntry= currentEntry->next_leaderboard_entry;
    }
}

void Leaderboard::write_to_file(const string& filename) {
    // TODO: Write the latest leaderboard status to the given file in the format specified in the PA instructions
    std::ofstream outputFile(filename,std::ios::trunc);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(-1);
    }
    LeaderboardEntry *currentEntry = head_leaderboard_entry;
    while (currentEntry) {
        outputFile << currentEntry->score << " " << currentEntry->last_played << " " << currentEntry->player_name << endl;
        currentEntry = currentEntry->next_leaderboard_entry;
    }
}

void Leaderboard::read_from_file(const string& filename) {
    // TODO: Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
    //       variable will point to the highest all-times score, and all other scores will be reachable from it
    //       via the "next_leaderboard_entry" member variable pointer.
    std::ifstream leaderboardFile(filename);
    if (!leaderboardFile.is_open()) {
        return;
    }
    string line;
    while (std::getline(leaderboardFile, line)) {
        stringstream playerLine(line);
        int score, time;
        string name;
        playerLine >> score >> time >> name;
        insert_new_entry(new LeaderboardEntry(score,time, name));
    }

}


void Leaderboard::print_leaderboard() {
    // TODO: Print the current leaderboard status to the standard output in the format specified in the PA instructions
    LeaderboardEntry *currentEntry = head_leaderboard_entry;
    cout<< "Leaderboard\n-----------\n";
    int rank = 1;
//    cout<< currentEntry->next_leaderboard_entry->player_name << endl;
    while (currentEntry){
        cout << rank++ << ". ";
        cout << currentEntry->player_name << " ";
        cout << currentEntry->score << " ";

        time_t timestamp = currentEntry->last_played;
        char timeString[100];

        strftime(timeString, sizeof(timeString), "%H:%M:%S/%d.%m.%Y", localtime(&timestamp));
        std::cout << timeString << std::endl;
        currentEntry = currentEntry->next_leaderboard_entry;
    }


}

Leaderboard::~Leaderboard() {
    // TODO: Free dynamically allocated memory used for storing leaderboard entries
    LeaderboardEntry *currentEntry = head_leaderboard_entry;
    LeaderboardEntry *nextEntry;
    while (currentEntry) {
        nextEntry = currentEntry->next_leaderboard_entry;
        delete currentEntry;
        currentEntry = nextEntry;
    }
    head_leaderboard_entry = nullptr;

}

