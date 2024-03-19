#include "SpaceSectorLLRBT.h"

using namespace std;

SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {}

void SpaceSectorLLRBT::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the LLRBT sector map
    // according to the given comparison critera based on the sector coordinates.
    std::ifstream sectors_file(filename);
    if (!sectors_file.is_open()) {
        std::cerr << "error with input file";
        exit(-1);
    }
    string line;
    bool first_line_passed = false;
    while (std::getline(sectors_file, line)) {
        if (!first_line_passed) {
            first_line_passed = true;
            continue;
        }
        std::istringstream location(line);
        int x, y, z;
        char comma;
        location >> x >> comma >> y >> comma >> z;
        insertSectorByCoordinates(x, y, z);
    }
}

// Remember to handle memory deallocation properly in the destructor.
SpaceSectorLLRBT::~SpaceSectorLLRBT() {
    // TODO: Free any dynamically allocated memory in this class.
    deleteTree(root);
}

void SpaceSectorLLRBT::deleteTree(Sector* root) {
    if (root != nullptr) {
        deleteTree(root->left);
        deleteTree(root->right);
        delete root;
    }
}

void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
    // TODO: Instantiate and insert a new sector into the space sector LLRBT map 
    // according to the coordinates-based comparison criteria.
    root = insertSector(root, x, y, z);
    root->color = BLACK;
}

Sector* SpaceSectorLLRBT::insertSector(Sector* current, int x, int y, int z){
    if (current == nullptr) {
        Sector* new_sector = new Sector(x, y , z, RED);
        sector_code_map.insert({new_sector->sector_code, vector<int>({x, y, z})});
        return new_sector;
    }
    int comparisonResult = compareCoordinates(x, y, z , current);
    if (comparisonResult < 0){
        current->left = insertSector(current->left, x, y, z);
        current->left->parent = current;
    }
    else if (comparisonResult > 0) {
        current->right = insertSector(current->right, x, y, z);
        current->right->parent = current;
    }
    if (isRed(current->right) && !isRed(current->left)) {
        current = rotateLeft(current);
    }
    if (isRed(current->left) && isRed(current->left->left)) {
        current = rotateRight(current);
    }
    if (isRed(current->left) && isRed(current->right)) {
        flipColors(current);
    }
    return current;
}
bool SpaceSectorLLRBT::isRed(Sector* node) {
    return node != nullptr && node->color == RED;
}

int SpaceSectorLLRBT::compareCoordinates(int x, int y, int z, const Sector* sector) {
    if (x < sector->x) {
        return -1;
    } else if (x > sector->x) {
        return 1;
    } else {
        if (y < sector->y) {
            return -1;
        } else if (y > sector->y) {
            return 1;
        } else {
            if (z < sector->z) {
                return -1;
            } else if (z > sector->z) {
                return 1;
            } else {
                return 0;
            }
        }
    }
}
Sector* SpaceSectorLLRBT::rotateLeft(Sector* sec) {
    Sector* rightSec = sec->right;
    sec->right = rightSec->left;
    if (sec->right != nullptr) {
        sec->right->parent = sec;
    }
    rightSec->parent = sec->parent;
    if (sec->parent == nullptr) {
        root = rightSec;
    } else if (sec == sec->parent->left) {
        sec->parent->left = rightSec;
    } else {
        sec->parent->right = rightSec;
    }

    rightSec->left = sec;
    sec->parent = rightSec;


    rightSec->color = sec->color;
    sec->color = RED;
    return rightSec;

}

Sector* SpaceSectorLLRBT::rotateRight(Sector* sec) {
    Sector* leftSec = sec->left;
    sec->left = leftSec->right;

//    leftSec->left->parent = sec;

    if (sec->left != nullptr) {
        sec->left->parent = sec;
    }
    leftSec->parent = sec->parent;
    if (sec->parent == nullptr) {
        root = leftSec;
    } else if (sec == sec->parent->left) {
        sec->parent->left = leftSec;
    } else {
        sec->parent->right = leftSec;
    }

    leftSec->right = sec;
    sec->parent = leftSec;

    leftSec->color = sec->color;
    sec->color = RED;
    return leftSec;
}

void SpaceSectorLLRBT::flipColors(Sector* sec) {
    sec->color = RED;
    sec->left->color = BLACK;
    sec->right->color = BLACK;
}


void SpaceSectorLLRBT::displaySectorsInOrder() {
    // TODO: Traverse the space sector LLRBT map in-order and print the sectors 
    // to STDOUT in the given format.
    cout << "Space sectors inorder traversal:" << endl;
    traverseInorder(root);
    cout << endl ;
}
void SpaceSectorLLRBT::traverseInorder(Sector* current) {
    if (current != nullptr) {
        traverseInorder(current->left);
        cout << (current->color ? "RED" : "BLACK") << " sector: ";
        cout << current->sector_code << endl;
        traverseInorder(current->right);
    }
}
void SpaceSectorLLRBT::displaySectorsPreOrder() {
    // TODO: Traverse the space sector LLRBT map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.
    cout << "Space sectors preorder traversal:" << endl;
    traversePreorder(root);
    cout << endl ;
}
void SpaceSectorLLRBT::traversePreorder(Sector* current) {
    if (current != nullptr) {
        cout << (current->color ? "RED" : "BLACK") << " sector: ";
        cout << current->sector_code << endl;
        traversePreorder(current->left);
        traversePreorder(current->right);
    }
}
void SpaceSectorLLRBT::displaySectorsPostOrder() {
    // TODO: Traverse the space sector LLRBT map in post-order traversal and print 
    // the sectors to STDOUT in the given format.
    cout << "Space sectors postorder traversal:" << endl;
    traversePostorder(root);
    cout << endl ;
}
void SpaceSectorLLRBT::traversePostorder(Sector* current) {
    if (current != nullptr) {
        traversePostorder(current->left);
        traversePostorder(current->right);
        cout << (current->color ? "RED" : "BLACK") << " sector: ";
        cout << current->sector_code << endl;
    }
}
std::vector<Sector*> SpaceSectorLLRBT::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector Sectors that are on
    // the path. Make sure that there are no duplicate Sector Sectors in the path!
    // Find paths for both nodes
    if (sector_code_map.count(sector_code) == 0)
        return path;
    Sector* earth = root;
    while (earth && !(earth->x == 0 && earth->y == 0 && earth->z == 0)) {
        if (compareCoordinates(0, 0, 0, earth) < 0) {
            earth = earth->left;
        } else if (compareCoordinates(0, 0, 0, earth) > 0) {
            earth = earth->right;
        }
    }
    vector<int> coords = sector_code_map[sector_code];
    int x = coords[0], y = coords[1], z = coords[2];
    Sector* current = root;
    while (current!= nullptr ) {
        if (current->sector_code != sector_code) {
            int comparisonResult = compareCoordinates(x, y, z, current);
            if (comparisonResult < 0) {
                current = current->left;
            } else if (comparisonResult > 0) {
                current = current->right;
            }
        } else
            break;
    }
    vector<Sector*> firstPath = deepFirstSearch(root, earth, {});
    vector<Sector*> secondPath = deepFirstSearch(root, current, {});

    // If either node is not found, return an empty vector
    if (firstPath.empty() || secondPath.empty())
        return {};
    // Find the common ancestor and construct the final path
    Sector* commonAncestor;
    for (size_t i = 0; i < min(firstPath.size(), secondPath.size()); ++i) {
        if (firstPath[i] == secondPath[i]) {
            commonAncestor = firstPath[i];
        } else {
            break;
        }
    }
    int commonIndex1 = 0;
    for (int i = 0; i < firstPath.size(); i++)
        if (firstPath[i] == commonAncestor)
            commonIndex1 = i;
    for (int i = firstPath.size() - 1; i > commonIndex1 ; i--){
        path.push_back(firstPath[i]);
    }
    path.push_back(commonAncestor);
    int commonIndex2 = 0;
    for (int i = 0; i < secondPath.size(); i++)
        if (secondPath[i] == commonAncestor)
            commonIndex2 = i;
    for (int i = commonIndex2+1; i < secondPath.size() ; i++){
        path.push_back(secondPath[i]);
    }
    return path;
}

vector<Sector*> SpaceSectorLLRBT::deepFirstSearch(Sector* sector, Sector* targetSector, std::vector<Sector *> path) {
    if (sector == nullptr) {
        return {};
    }
    path.push_back(sector);
    if (sector == targetSector) {
        return path;
    }
    vector<Sector*> leftPath = deepFirstSearch(sector->left, targetSector, path);
    vector<Sector*> rightPath = deepFirstSearch(sector->right, targetSector, path);

    if (!leftPath.empty()) {
        return leftPath;
    } else if (!rightPath.empty()) {
        return rightPath;
    } else {
        return {};
    }
}

void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {
    // TODO: Print the stellar path obtained from the getStellarPath() function 
    // to STDOUT in the given format.
    if (!path.empty()){
        cout << "The stellar path to Dr. Elara: ";
        for (int i = 0; i < path.size() - 1; ++i){
            cout << path[i]->sector_code << "->";
        }
        cout << path.back()->sector_code << endl;
    } else
        cout << "A path to Dr. Elara could not be found." << endl << endl;
}

