#include "BitboardCube.h"
#include "CornerDB.h"
#include "EdgeDB.h"
#include "Solver.h"
#include <iostream>
#include <vector>

int main() {
    // 1. Initialize and Load/Generate Pattern Databases
    CornerDB corner_db;
    if (!corner_db.loadFromFile("corners.db")) {
        corner_db.generateDB();
        corner_db.saveToFile("corners.db");
    }

    EdgeDB edge_db;
    if (!edge_db.loadFromFile("edges.db")) {
        edge_db.generateDB();
        edge_db.saveToFile("edges.db");
    }

    // 2. Instantiate a clean Solved Cube
    BitboardCube cube;

    // 3. SCRAMBLE THE CUBE
    // A heavy 14-move scramble to feed directly into the visual engine
    std::vector<MOVE> scramble = {
        MOVE::R, MOVE::U, MOVE::F, MOVE::B, MOVE::L2, MOVE::D_PRIME, 
        MOVE::R_PRIME, MOVE::U2, MOVE::F_PRIME, MOVE::L, MOVE::B2, 
        MOVE::D, MOVE::R, MOVE::U_PRIME
    };

    for (MOVE m : scramble) {
        cube.move(m);
    }

    // 4. Run the Optimal Korf Heuristic Solver
    Solver solver(cube, corner_db, edge_db);
    solver.solveIDAStar();
    solver.printSolution();

    return 0;
}