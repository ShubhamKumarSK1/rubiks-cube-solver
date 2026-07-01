#ifndef BITBOARD_CUBE_H
#define BITBOARD_CUBE_H

#include "RubiksCube.h"
#include <cstdint>

class BitboardCube : public RubiksCube {
    // ALLOW BOTH DATABASES TO SAFELY READ BITS FOR INDEX MAPPING
    friend class EdgeDB; 
    friend class CornerDB; // <-- ADD THIS LINE HERE

private:
    uint64_t corner_pos; // 8 pieces * 3 bits = 24 bits
    uint64_t corner_ori; // 8 pieces * 2 bits = 16 bits
    uint64_t edge_pos;   // 12 pieces * 4 bits = 48 bits
    uint64_t edge_ori;   // 12 pieces * 1 bit = 12 bits

    const uint64_t SOLVED_CORNER_POS = 0x76543210ULL;       
    const uint64_t SOLVED_CORNER_ORI = 0x0ULL;           
    const uint64_t SOLVED_EDGE_POS   = 0xBA9876543210ULL;   
    const uint64_t SOLVED_EDGE_ORI   = 0x0ULL;        

    void move_U();
    void move_D();
    void move_R();
    void move_L();
    void move_F();
    void move_B();

public:
    BitboardCube();
    ~BitboardCube() override = default;

    bool isSolved() const override;
    void printBits() const override;
    void setToSolved();
    RubiksCube& move(MOVE ind) override;
    bool isPhase1Ready() const; // True if CO, EO, and Mid-edges are in position
    void copyStateFrom(const BitboardCube& other);
};

#endif // BITBOARD_CUBE_H