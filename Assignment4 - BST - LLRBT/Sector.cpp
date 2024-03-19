#include "Sector.h"
#include <cmath>

// Constructor implementation

Sector::Sector(int x, int y, int z) : x(x), y(y), z(z), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
        // TODO: Calculate the distance to the Earth, and generate the sector code
    distance_from_earth = sqrt((x*x) + (y*y) + (z*z));
    sector_code = generateSectorCode(x, y, z);
}
Sector::Sector(int x, int y, int z, bool color) : x(x), y(y), z(z), left(nullptr), right(nullptr), parent(nullptr), color(color) {
    // TODO: Calculate the distance to the Earth, and generate the sector code
    distance_from_earth = sqrt((x*x) + (y*y) + (z*z));
    sector_code = generateSectorCode(x, y, z);
}

std::string Sector::generateSectorCode(int x, int y, int z) {
    std::string codeX = (x > 0) ? "R" : ((x < 0) ? "L" : "S");
    std::string codeY = (y > 0) ? "U" : ((y < 0) ? "D" : "S");
    std::string codeZ = (z > 0) ? "F" : ((z < 0) ? "B" : "S");
    return std::to_string((int)sqrt((x*x) + (y*y) + (z*z))) + codeX + codeY + codeZ;
}

Sector::~Sector() {
    // TODO: Free any dynamically allocated memory if necessary
}

Sector& Sector::operator=(const Sector& other) {
    // TODO: Overload the assignment operator
    if (this != &other) {
        x = other.x;
        y = other.y;
        z = other.z;
        distance_from_earth = other.distance_from_earth;
        sector_code = other.sector_code;
        right = other.right;
        left = other.left;
        parent = other.parent;
        color = other.color;
    }
    return *this;
}

bool Sector::operator==(const Sector& other) const {
    return (x == other.x && y == other.y && z == other.z);
}

bool Sector::operator!=(const Sector& other) const {
    return !(*this == other);
}