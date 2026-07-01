#ifndef CORNER_DB_H
#define CORNER_DB_H

#include "BitboardCube.h" //
#include <vector>
#include <string>
#include <cstdint>

class CornerDB {
private:
    // 88,179,840 bytes ~ 84 MB
    std::vector<uint8_t> database;
    const size_t DB_SIZE = 88179840;

    // Precomputed factorials for Lehmer code matching
    const int fact[8] = {1, 1, 2, 6, 24, 120, 720, 5040};

public:
    CornerDB();

    // Converts a BitboardCube state into a single unique index
    size_t getIndex(const BitboardCube& cube) const;

    // Generates the database via a backward BFS from the solved state
    void generateDB();

    // Gets the heuristic value (minimum moves to solve corners) for a given cube state
    uint8_t getHeuristic(const BitboardCube& cube) const;

    // Save/Load to disk so we don't have to recompute it every execution
    bool saveToFile(const std::string& filename);
    bool loadFromFile(const std::string& filename);
};

#endif // CORNER_DB_H