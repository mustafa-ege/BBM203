#include "SpaceSectorBST.h"

using namespace std;

SpaceSectorBST::SpaceSectorBST() : root(nullptr) {}

SpaceSectorBST::~SpaceSectorBST() {
    // Free any dynamically allocated memory in this class.
    deleteTree(root);

}
void SpaceSectorBST::deleteTree(Sector* root) {
    if (root != nullptr) {
        deleteTree(root->left);
        deleteTree(root->right);
        delete root;
    }
}

void SpaceSectorBST::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the BST sector map
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

void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z) {
    // Instantiate and insert a new sector into the space sector BST map according to the
    // coordinates-based comparison criteria.
    root = insertSector(root, x, y, z);
}

Sector* SpaceSectorBST::insertSector(Sector* current, int x, int y, int z){
    if (current == nullptr) {
        Sector* new_sector = new Sector(x, y , z);
        sector_code_map.insert({new_sector->sector_code, vector<int>({x, y, z})});
        return new_sector;
    }
    int comparisonResult = compareCoordinates(x, y, z , current);
    if (comparisonResult < 0)
        current->left = insertSector(current->left, x, y, z);
    else if (comparisonResult > 0)
        current->right = insertSector(current->right, x, y, z);

    return current;
}

int SpaceSectorBST::compareCoordinates(int x, int y, int z, const Sector* sector) {
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
void SpaceSectorBST::deleteSector(const std::string& sector_code) {
//    // TODO: Delete the sector given by its sector_code from the BST.
//    print_to_file(to_string(sector_code_map[sector_code][0])+" "+to_string(sector_code_map[sector_code][1])+" "+to_string(sector_code_map[sector_code][2])+"\n","output.txt");

    root = deleteSectorRecursive(root, sector_code);
}

Sector* SpaceSectorBST::deleteSectorRecursive(Sector* current, const std::string& sector_code) {
    for (const auto &keyValue : sector_code_map) {
        const std::string &currentKey = keyValue.first;
    }
    if (sector_code_map.count(sector_code) != 0){
        vector<int> coords = sector_code_map[sector_code];
        int x = coords[0];
        int y = coords[1];
        int z = coords[2];
        if (current == nullptr)
            return nullptr;
        int comparisonResult = compareCoordinates(x, y, z, current);
        if (comparisonResult < 0){
            current->left = deleteSectorRecursive(current->left, sector_code);
        }
        else if (comparisonResult > 0) {
            current->right = deleteSectorRecursive(current->right, sector_code);
        } else {
            //// leaf
            if (current->left == nullptr && current->right == nullptr) {
                sector_code_map.erase(sector_code);
                delete current;
                return nullptr;
            }
            //// one child
            else if (current->left == nullptr) {
                Sector* temp = current->right;
                sector_code_map.erase(sector_code);
                delete current;
                return temp;
            } else if (current->right == nullptr) {
                Sector* temp = current->left;
                sector_code_map.erase(sector_code);
                delete current;
                return temp;
            }
            //// two children
            else {
                Sector* successor = findMinSector(current->right);
                Sector temp = *current;
                temp.x = successor->x;
                temp.y = successor->y;
                temp.z = successor->z;
                temp.sector_code = successor->sector_code;
                temp.distance_from_earth = successor->distance_from_earth;
                successor->x = current->x;
                successor->y = current->y;
                successor->z = current->z;
                successor->sector_code = current->sector_code;
                successor->distance_from_earth = current->distance_from_earth;
                current->x = temp.x;
                current->y = temp.y;
                current->z = temp.z;
                current->sector_code = temp.sector_code;
                current->distance_from_earth = temp.distance_from_earth;
                current->right = deleteSectorRecursive(current->right, successor->sector_code);
            }
        }
    }
    return current;
}

Sector* SpaceSectorBST::findMinSector(Sector* node) {
    while (node->left != nullptr)
        node = node->left;
    return node;
}

void SpaceSectorBST::displaySectorsInOrder() {
    // TODO: Traverse the space sector BST map in-order and print the sectors
    // to STDOUT in the given format.
    cout << "Space sectors inorder traversal:" << endl;
    traverseInorder(root);
    cout << endl ;
}

void SpaceSectorBST::traverseInorder(Sector* current) {
    if (current != nullptr) {
        traverseInorder(current->left);
        cout << current->sector_code << endl;
        traverseInorder(current->right);
    }
}

void SpaceSectorBST::displaySectorsPreOrder() {
    // TODO: Traverse the space sector BST map in pre-order traversal and print
    // the sectors to STDOUT in the given format.
    cout << "Space sectors preorder traversal:" << endl;
    traversePreorder(root);
    cout << endl ;
}
void SpaceSectorBST::traversePreorder(Sector* current) {
    if (current != nullptr) {
        cout << current->sector_code << endl;
        traversePreorder(current->left);
        traversePreorder(current->right);
    }
}

void SpaceSectorBST::displaySectorsPostOrder() {
    // TODO: Traverse the space sector BST map in post-order traversal and print
    // the sectors to STDOUT in the given format.
    cout << "Space sectors postorder traversal:" << endl;
    traversePostorder(root);
    cout << endl ;
}
void SpaceSectorBST::traversePostorder(Sector* current) {
    if (current != nullptr) {
        traversePostorder(current->left);
        traversePostorder(current->right);
        cout << current->sector_code << endl;
    }
}

std::vector<Sector*> SpaceSectorBST::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!
    if (sector_code_map.count(sector_code) == 0)
        return path;
    vector<int> coords = sector_code_map[sector_code];
    int x = coords[0], y = coords[1], z = coords[2];
    Sector* current = root;
    while (current!= nullptr ) {
        path.push_back(current);
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
    return path;
}


void SpaceSectorBST::printStellarPath(const std::vector<Sector*>& path) {
    // TODO: Print the stellar path obtained from the getStellarPath() function
    // to STDOUT in the given format.
    if (!path.empty()){
        cout << "The stellar path to Dr. Elara: ";
        for (int i = 0; i < path.size() - 1; ++i)
            cout << path[i]->sector_code << "->";
        cout << path.back()->sector_code << endl;
    } else
        cout << "A path to Dr. Elara could not be found." << endl ;
}

void SpaceSectorBST::print_to_file(const string& text, const string& filename){
    std::ofstream outputFile(filename,std::ios::app);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(-1);
    }
    outputFile << text;
}