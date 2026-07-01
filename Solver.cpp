#include "Solver.h"
#include <iostream>
#include <algorithm> // For std::max
#include "EdgeDB.h"
// Define the 18 possible standard face adjustments globally
const MOVE ALL_MOVES[] = {
    MOVE::U, MOVE::U_PRIME, MOVE::U2, MOVE::D, MOVE::D_PRIME, MOVE::D2,
    MOVE::R, MOVE::R_PRIME, MOVE::R2, MOVE::L, MOVE::L_PRIME, MOVE::L2,
    MOVE::F, MOVE::F_PRIME, MOVE::F2, MOVE::B, MOVE::B_PRIME, MOVE::B2
};

// =========================================================================
// STEP 1.1: MOVE PRUNING HELPER FUNCTION
// =========================================================================
// Returns true if a move sequence is redundant or creates an undo loop.
bool shouldPruningAllow(MOVE current_move, MOVE last_move) {
    if (last_move == current_move) return true; // Skip identical back-to-back moves

    // Group moves by their physical face axis (0=U, 1=D, 2=R, 3=L, 4=F, 5=B)
    int cur_face = static_cast<int>(current_move) / 3; 
    int last_face = static_cast<int>(last_move) / 3;

    // Prune if they operate on the exact same face face (e.g., U followed immediately by U')
    if (cur_face == last_face) return true;

    // Prune symmetric opposite faces to avoid dual-order redundancy 
    // Enforces a strict sequence order: allows U->D, but skips D->U 
    if ((cur_face == 1 && last_face == 0) || 
        (cur_face == 3 && last_face == 2) || 
        (cur_face == 5 && last_face == 4)) {
        return true;
    }

    return false;
}

// =========================================================================
// SOLVER CORE IMPLEMENTATION
// =========================================================================

// Constructor updated to reference both heuristic databases
Solver::Solver(BitboardCube& c, const CornerDB& c_db, const EdgeDB& e_db) 
    : cube(c), corner_db(c_db), edge_db(e_db) {}

// Heuristic utility function that updates lower bounds using maximum filtering
int Solver::getHeuristicValue(const BitboardCube& current_cube) {
    int corner_h = corner_db.getHeuristic(current_cube);
    int edge_h   = edge_db.getHeuristic(current_cube);
    
    // The safest true distance bound is the maximum of both pattern databases
    return std::max(corner_h, edge_h);
}
// 1. Update your root initializer to reset the counter
void Solver::solveIDAStar() {
    std::cout << "Starting Korf's Parallel IDA* Search Execution...\n";
    solution.clear();
    nodes_evaluated = 0; // <-- Reset the counter here

    int threshold = getHeuristicValue(cube);
    bool solved = false;

    while (!solved && threshold <= 20) {
        std::cout << "Searching threshold bound limit: " << threshold << "...\n";
        std::vector<MOVE> moves_performed;
        
        if (idaStarSearch(threshold, 0, moves_performed)) {
            solution = moves_performed;
            solved = true;
            break;
        }
        threshold++;
    }

    if (solved) {
        std::cout << "[SUCCESS] Optimal solution found!\n";
    } else {
        std::cout << "[FAILED] Unsolvable scramble state profile checked.\n";
    }
}

// 2. Update the recursive loop to increment the counter
bool Solver::idaStarSearch(int depth, int g_score, std::vector<MOVE>& moves_performed) {
    nodes_evaluated++; // <-- Increment every single time a node is visited!

    int h_score = getHeuristicValue(cube);
    if (g_score + h_score > depth) return false; 
    if (cube.isSolved()) return true;

    for (MOVE move : ALL_MOVES) {
        if (!moves_performed.empty()) {
            if (shouldPruningAllow(move, moves_performed.back())) {
                continue; 
            }
        }

        cube.move(move);
        moves_performed.push_back(move);

        if (idaStarSearch(depth, g_score + 1, moves_performed)) {
            return true; 
        }

        moves_performed.pop_back();
        cube.move(move).move(move).move(move); 
    }

    return false;
}

// 3. Update the printing function to display the real results
void Solver::printSolution() const {
    if (solution.empty()) {
        std::cout << "Solution Path: Cube is already solved or solution empty.\n";
        return;
    }

    // Print the true real-time metric
    std::cout << "Total states evaluated: " << nodes_evaluated << " nodes.\n";
    std::cout << "Solution Path (" << solution.size() << " moves): ";
    for (MOVE m : solution) {
        switch (m) {
            case MOVE::U:       std::cout << "U "; break;
            case MOVE::U_PRIME: std::cout << "U' "; break;
            case MOVE::U2:      std::cout << "U2 "; break;
            case MOVE::D:       std::cout << "D "; break;
            case MOVE::D_PRIME: std::cout << "D' "; break;
            case MOVE::D2:      std::cout << "D2 "; break;
            case MOVE::R:       std::cout << "R "; break;
            case MOVE::R_PRIME: std::cout << "R' "; break;
            case MOVE::R2:      std::cout << "R2 "; break;
            case MOVE::L:       std::cout << "L "; break;
            case MOVE::L_PRIME: std::cout << "L' "; break;
            case MOVE::L2:      std::cout << "L2 "; break;
            case MOVE::F:       std::cout << "F "; break;
            case MOVE::F_PRIME: std::cout << "F' "; break;
            case MOVE::F2:      std::cout << "F2 "; break;
            case MOVE::B:       std::cout << "B "; break;
            case MOVE::B_PRIME: std::cout << "B' "; break;
            case MOVE::B2:      std::cout << "B2 "; break;
        }
    }
    std::cout << "\n";
}