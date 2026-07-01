#ifndef EDGE_DB_H
#define EDGE_DB_H

#include "BitboardCube.h"
#include <vector>
#include <string>
#include <cstdint>

class EdgeDB {
private:
    std::vector<uint8_t> db;
    const uint32_t DB_SIZE = 42577920; // P(12,6) * 2^6

    // Factorial helper for Lehmer code / ranking
    int factorial(int n) const {
        return (n <= 1) ? 1 : n * factorial(n - 1);
    }

public:
    EdgeDB();
    uint32_t getIndex(const BitboardCube& cube) const;
    void generateDB();
    uint8_t getHeuristic(const BitboardCube& cube) const;
    
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename);
};

#endif