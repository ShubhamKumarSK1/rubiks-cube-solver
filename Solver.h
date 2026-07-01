#ifndef SOLVER_H
#define SOLVER_H

#include "BitboardCube.h"
#include "CornerDB.h"
#include "EdgeDB.h"
#include <vector>

class Solver {
private:
    BitboardCube& cube;
    const CornerDB& corner_db;
    const EdgeDB& edge_db;
    std::vector<MOVE> solution;
    
    std::uint64_t nodes_evaluated;
    // Private core execution helpers
    int getHeuristicValue(const BitboardCube& current_cube);
    bool idaStarSearch(int depth, int g_score, std::vector<MOVE>& moves_performed);

public:
    // Updated Constructor signature matching Solver.cpp
    Solver(BitboardCube& c, const CornerDB& c_db, const EdgeDB& e_db);
    
    // Updated return type to void matching Solver.cpp
    void solveIDAStar(); 
    void printSolution() const;
};

#endif // SOLVER_H