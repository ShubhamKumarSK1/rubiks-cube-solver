#include "CornerDB.h"
#include <iostream>
#include <fstream>
#include <queue>

CornerDB::CornerDB() {
    database.assign(DB_SIZE, 0xFF); // Initialize all states as unvisited (0xFF)
}

// Converts a BitboardCube state into a single unique index
size_t CornerDB::getIndex(const BitboardCube& cube) const {
    // 1. Extract corner positions and orientations from bitboard
    int perm[8];
    int ori[8];
    for (int i = 0; i < 8; ++i) {
        perm[i] = (cube.corner_pos >> (i * 3)) & 0x7;
        ori[i]  = (cube.corner_ori >> (i * 2)) & 0x3;
    }

    // 2. Compute Permutation Index using Lehmer Code (Factorial Number System)
    int perm_idx = 0;
    for (int i = 0; i < 8; ++i) {
        int count = 0;
        for (int j = i + 1; j < 8; ++j) {
            if (perm[j] < perm[i]) count++;
        }
        perm_idx += count * fact[7 - i];
    }

    // 3. Compute Orientation Index (Base-3 representation of first 7 corners)
    int ori_idx = 0;
    for (int i = 0; i < 7; ++i) {
        ori_idx = ori_idx * 3 + ori[i];
    }

    // Combine into a unique flat database coordinate
    return perm_idx * 2187 + ori_idx;
}

uint8_t CornerDB::getHeuristic(const BitboardCube& cube) const {
    return database[getIndex(cube)];
}

// Full backward BFS to populate the database
void CornerDB::generateDB() {
    std::cout << "Generating Corner Pattern Database via BFS...\n";

    BitboardCube solvedCube; // Starts solved by default
    size_t solved_idx = getIndex(solvedCube);
    
    std::queue<BitboardCube> q;
    q.push(solvedCube);
    database[solved_idx] = 0;

    size_t visited = 1;
    int current_depth = 0;

    while (!q.empty()) {
        size_t level_size = q.size();
        std::cout << "Depth " << current_depth << ": " << level_size << " nodes processed. Total visited: " << visited << "\n";

        for (size_t i = 0; i < level_size; ++i) {
            BitboardCube curr = q.front();
            q.pop();

            uint8_t curr_dist = database[getIndex(curr)];

            // Branch into all 18 standard moves
            for (int m = 0; m < 18; ++m) {
                BitboardCube next_state = curr;
                next_state.move(static_cast<MOVE>(m)); //

                size_t next_idx = getIndex(next_state);
                if (database[next_idx] == 0xFF) { // Unvisited state found
                    database[next_idx] = curr_dist + 1;
                    visited++;
                    q.push(next_state);
                }
            }
        }
        current_depth++;
    }
    std::cout << "Database generation complete! Total states mapped: " << visited << "\n";
}

bool CornerDB::saveToFile(const std::string& filename) {
    std::ofstream out(filename, std::ios::binary);
    if (!out) return false;
    out.write(reinterpret_cast<const char*>(database.data()), DB_SIZE);
    return true;
}

bool CornerDB::loadFromFile(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) return false;
    in.read(reinterpret_cast<char*>(database.data()), DB_SIZE);
    return true;
}