#ifndef SPACESECTORBST_H
#define SPACESECTORBST_H

#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>
#include <unordered_map>

#include "Sector.h"

class SpaceSectorBST {
  
public:
    Sector *root;
    SpaceSectorBST();
    ~SpaceSectorBST();
    void readSectorsFromFile(const std::string& filename); 
    void insertSectorByCoordinates(int x, int y, int z);
    void deleteSector(const std::string& sector_code);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);

    static void print_to_file(const std::string& text, const std::string& filename);


    std::unordered_map< std::string , std::vector<int> > sector_code_map;

    int compareCoordinates(int x, int y, int z, const Sector* sector);
    Sector* insertSector(Sector* current, int x, int y, int z);
    Sector* deleteSectorRecursive(Sector* current, const std::string& sector_code);
    Sector* findMinSector(Sector* node);

    void traverseInorder(Sector* current);
    void traversePreorder(Sector* current);
    void traversePostorder(Sector* current);

    void deleteTree(Sector* root);

    };

#endif // SPACESECTORBST_H
