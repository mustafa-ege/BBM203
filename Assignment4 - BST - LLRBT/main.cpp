#include "SpaceSectorBST.h"
#include "SpaceSectorLLRBT.h"

#include <iostream>
#include <string>

using namespace std;

int main(int argc, char**argv) {

    //************ Task 1: Space Sector Mapping ***********//
//    SpaceSectorBST sector_map_BST;
//    sector_map_BST.readSectorsFromFile(argv[1]);
//    sector_map_BST.displaySectorsInOrder();
//    sector_map_BST.displaySectorsPreOrder();
//    sector_map_BST.displaySectorsPostOrder();
//
//    std::vector<Sector*> stellar_path = sector_map_BST.getStellarPath("45RDF");
//    sector_map_BST.printStellarPath(stellar_path);
//
//    cout << endl;
//
//    stellar_path = sector_map_BST.getStellarPath("31SUF");
//    sector_map_BST.printStellarPath(stellar_path);
//
//    cout << endl;
    //************ Task 2: Optimizing Space Sector Mapping ***********//
    SpaceSectorLLRBT sector_map_LLRBT;
    sector_map_LLRBT.readSectorsFromFile(argv[1]);
    sector_map_LLRBT.displaySectorsInOrder();
    sector_map_LLRBT.displaySectorsPreOrder();
    sector_map_LLRBT.displaySectorsPostOrder();

    std::vector<Sector*> stellar_path_2 = sector_map_LLRBT.getStellarPath("121RDB");
    sector_map_LLRBT.printStellarPath(stellar_path_2);

    cout << endl;

    stellar_path_2 = sector_map_LLRBT.getStellarPath("45RDF");
    sector_map_LLRBT.printStellarPath(stellar_path_2);

    cout << endl;


    return 0;
}


