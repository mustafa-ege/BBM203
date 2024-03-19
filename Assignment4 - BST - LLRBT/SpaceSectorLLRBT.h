#ifndef SPACESECTORLLRBT_H
#define SPACESECTORLLRBT_H

#include "Sector.h"
#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>
#include <unordered_map>

class SpaceSectorLLRBT {
public:
    Sector* root;
    SpaceSectorLLRBT();
    ~SpaceSectorLLRBT();
    void readSectorsFromFile(const std::string& filename);
    void insertSectorByCoordinates(int x, int y, int z);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);

    std::unordered_map< std::string , std::vector<int> > sector_code_map;

    Sector* insertSector(Sector* current, int x, int y, int z);
    int compareCoordinates(int x, int y, int z, const Sector* sector);
    Sector* rotateLeft(Sector* sec);
    Sector* rotateRight(Sector* sec);
    void flipColors(Sector* sec);
    bool isRed(Sector* node);

    void traverseInorder(Sector* current);
    void traversePreorder(Sector* current);
    void traversePostorder(Sector* current);
    std::vector<Sector*> deepFirstSearch(Sector* sector, Sector* targetSector, std::vector<Sector*> path);
    void deleteTree(Sector* root);


};

#endif // SPACESECTORLLRBT_H
