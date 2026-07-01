#include "EdgeDB.h"
#include <queue>
#include <iostream>
#include <fstream>

EdgeDB::EdgeDB() {
    db.assign(DB_SIZE, 0xFF); // Initialize all states as unvisited (0xFF)
}

uint32_t EdgeDB::getIndex(const BitboardCube& cube) const {
    // Direct access via friendly tracking configuration
    uint64_t ep = cube.edge_pos;
    uint64_t eo = cube.edge_ori;

    int pieces[12];
    int orientations[12];

    // Extract all 12 edge locations and orientations from the bitboards
    for (int i = 0; i < 12; ++i) {
        pieces[i] = (ep >> (i * 4)) & 0xF;
        orientations[i] = (eo >> i) & 0x1;
    }

    // Find positions of targeted pieces 0, 1, 2, 3, 4, 5
    int pos[6] = {0};
    int ori_bits = 0;
    for (int i = 0; i < 12; ++i) {
        if (pieces[i] >= 0 && pieces[i] <= 5) {
            pos[pieces[i]] = i;
            if (orientations[i]) {
                ori_bits |= (1 << pieces[i]);
            }
        }
    }

    // Compute Lehmer permutation rank for the 6 positions out of 12 choices
    int rank = 0;
    bool used[12] = {false};
    for (int i = 0; i < 6; ++i) {
        int count = 0;
        for (int j = 0; j < pos[i]; ++j) {
            if (!used[j]) count++;
        }
        used[pos[i]] = true;
        
        int choices = 12 - i;
        int permutations = 1;
        for(int k = 0; k < (5 - i); k++) permutations *= (choices - 1 - k);
        if (i == 5) permutations = 1;

        rank += count * permutations;
    }

    // Combine position rank and orientation bit-vector safely
    return (rank << 6) | ori_bits;
}

void EdgeDB::generateDB() {
    if (db[0] == 0) return; // Already generated

    std::queue<BitboardCube> q;
    BitboardCube root;
    uint32_t root_idx = getIndex(root);
    db[root_idx] = 0;
    q.push(root);

    uint32_t visited = 1;
    uint8_t depth = 0;

    std::cout << "Generating Edge Pattern Database via BFS...\n";

    MOVE moves[] = { MOVE::U, MOVE::U_PRIME, MOVE::U2, MOVE::D, MOVE::D_PRIME, MOVE::D2,
                     MOVE::R, MOVE::R_PRIME, MOVE::R2, MOVE::L, MOVE::L_PRIME, MOVE::L2,
                     MOVE::F, MOVE::F_PRIME, MOVE::F2, MOVE::B, MOVE::B_PRIME, MOVE::B2 };

    while (!q.empty()) {
        uint32_t current_level_size = q.size();
        std::cout << "Depth " << (int)depth << ": " << current_level_size 
                  << " nodes processed. Total visited: " << visited << "\n";

        for (uint32_t i = 0; i < current_level_size; ++i) {
            BitboardCube curr = q.front();
            q.pop();

            if (db[getIndex(curr)] < depth) continue;

            for (MOVE m : moves) {
                BitboardCube next_state = curr;
                next_state.move(m);
                uint32_t next_idx = getIndex(next_state);

                if (db[next_idx] == 0xFF) { // First time visiting this state
                    db[next_idx] = depth + 1;
                    visited++;
                    q.push(next_state);
                }
            }
        }
        depth++;
    }
    std::cout << "Edge Database complete! Total states mapped: " << visited << "\n";
}

uint8_t EdgeDB::getHeuristic(const BitboardCube& cube) const {
    return db[getIndex(cube)];
}

bool EdgeDB::saveToFile(const std::string& filename) {
    std::ofstream out(filename, std::ios::binary);
    if (!out) return false;
    out.write(reinterpret_cast<const char*>(db.data()), db.size());
    return true;
}

bool EdgeDB::loadFromFile(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) return false;
    in.read(reinterpret_cast<char*>(db.data()), db.size());
    return true;
}