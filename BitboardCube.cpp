#include "BitboardCube.h"
#include <iostream>
#include <iomanip>

BitboardCube::BitboardCube() {
    setToSolved();
}

void BitboardCube::setToSolved() {
    corner_pos = 0x76543210ULL;
    corner_ori = 0x0ULL;
    edge_pos   = 0xBA9876543210ULL;
    edge_ori   = 0x0ULL;
}

bool BitboardCube::isSolved() const {
    return (corner_pos == SOLVED_CORNER_POS &&
            corner_ori == SOLVED_CORNER_ORI &&
            edge_pos   == SOLVED_EDGE_POS   &&
            edge_ori   == SOLVED_EDGE_ORI);
}

void BitboardCube::printBits() const {
    std::cout << "-------------------------------------------\n";
    std::cout << "Corner Pos: 0x" << std::hex << corner_pos << std::dec << "\n";
    std::cout << "Corner Ori: 0x" << std::hex << corner_ori << std::dec << "\n";
    std::cout << "Edge Pos:   0x" << std::hex << edge_pos   << std::dec << "\n";
    std::cout << "Edge Ori:   0x" << std::hex << edge_ori   << std::dec << "\n";
    std::cout << "-------------------------------------------\n";
}

RubiksCube& BitboardCube::move(MOVE ind) {
    switch (ind) {
        case MOVE::U:       move_U(); break;
        case MOVE::U_PRIME: move_U(); move_U(); move_U(); break;
        case MOVE::U2:      move_U(); move_U(); break;
        case MOVE::D:       move_D(); break;
        case MOVE::D_PRIME: move_D(); move_D(); move_D(); break;
        case MOVE::D2:      move_D(); move_D(); break;
        case MOVE::R:       move_R(); break;
        case MOVE::R_PRIME: move_R(); move_R(); move_R(); break;
        case MOVE::R2:      move_R(); move_R(); break;
        case MOVE::L:       move_L(); break;
        case MOVE::L_PRIME: move_L(); move_L(); move_L(); break;
        case MOVE::L2:      move_L(); move_L(); break;
        case MOVE::F:       move_F(); break;
        case MOVE::F_PRIME: move_F(); move_F(); move_F(); break;
        case MOVE::F2:      move_F(); move_F(); break;
        case MOVE::B:       move_B(); break;
        case MOVE::B_PRIME: move_B(); move_B(); move_B(); break;
        case MOVE::B2:      move_B(); move_B(); break;
    }
    return *this;
}

void BitboardCube::move_U() {
    uint64_t c0 = (corner_pos >> (0 * 3)) & 0x7;
    uint64_t c1 = (corner_pos >> (1 * 3)) & 0x7;
    uint64_t c2 = (corner_pos >> (2 * 3)) & 0x7;
    uint64_t c3 = (corner_pos >> (3 * 3)) & 0x7;
    corner_pos &= ~0xFFFULL; 
    corner_pos |= (c1 << (0 * 3)) | (c2 << (1 * 3)) | (c3 << (2 * 3)) | (c0 << (3 * 3));

    uint64_t co0 = (corner_ori >> (0 * 2)) & 0x3;
    uint64_t co1 = (corner_ori >> (1 * 2)) & 0x3;
    uint64_t co2 = (corner_ori >> (2 * 2)) & 0x3;
    uint64_t co3 = (corner_ori >> (3 * 2)) & 0x3;
    corner_ori &= ~0xFFULL; 
    corner_ori |= (co1 << (0 * 2)) | (co2 << (1 * 2)) | (co3 << (2 * 2)) | (co0 << (3 * 2));

    uint64_t e0 = (edge_pos >> (0 * 4)) & 0xF;
    uint64_t e1 = (edge_pos >> (1 * 4)) & 0xF;
    uint64_t e2 = (edge_pos >> (2 * 4)) & 0xF;
    uint64_t e3 = (edge_pos >> (3 * 4)) & 0xF;
    edge_pos &= ~0xFFFFULL;
    edge_pos |= (e1 << (0 * 4)) | (e2 << (1 * 4)) | (e3 << (2 * 4)) | (e0 << (3 * 4));

    uint64_t eo0 = (edge_ori >> 0) & 0x1;
    uint64_t eo1 = (edge_ori >> 1) & 0x1;
    uint64_t eo2 = (edge_ori >> 2) & 0x1;
    uint64_t eo3 = (edge_ori >> 3) & 0x1;
    edge_ori &= ~0xFULL;
    edge_ori |= (eo1 << 0) | (eo2 << 1) | (eo3 << 2) | (eo0 << 3);
}

void BitboardCube::move_D() {
    uint64_t c4 = (corner_pos >> (4 * 3)) & 0x7;
    uint64_t c5 = (corner_pos >> (5 * 3)) & 0x7;
    uint64_t c6 = (corner_pos >> (6 * 3)) & 0x7;
    uint64_t c7 = (corner_pos >> (7 * 3)) & 0x7;
    corner_pos &= ~0xFFF000ULL; 
    corner_pos |= (c7 << (4 * 3)) | (c4 << (5 * 3)) | (c5 << (6 * 3)) | (c6 << (7 * 3));

    uint64_t co4 = (corner_ori >> (4 * 2)) & 0x3;
    uint64_t co5 = (corner_ori >> (5 * 2)) & 0x3;
    uint64_t co6 = (corner_ori >> (6 * 2)) & 0x3;
    uint64_t co7 = (corner_ori >> (7 * 2)) & 0x3;
    corner_ori &= ~0xFF00ULL; 
    corner_ori |= (co7 << (4 * 2)) | (co4 << (5 * 2)) | (co5 << (6 * 2)) | (co6 << (7 * 2));

    uint64_t e8  = (edge_pos >> (8 * 4))  & 0xF;
    uint64_t e9  = (edge_pos >> (9 * 4))  & 0xF;
    uint64_t e10 = (edge_pos >> (10 * 4)) & 0xF;
    uint64_t e11 = (edge_pos >> (11 * 4)) & 0xF;
    edge_pos &= ~0xFFFF00000000ULL;
    edge_pos |= (e11 << (8 * 4)) | (e8 << (9 * 4)) | (e9 << (10 * 4)) | (e10 << (11 * 4));

    uint64_t eo8  = (edge_ori >> 8)  & 0x1;
    uint64_t eo9  = (edge_ori >> 9)  & 0x1;
    uint64_t eo10 = (edge_ori >> 10) & 0x1;
    uint64_t eo11 = (edge_ori >> 11) & 0x1;
    edge_ori &= ~0xF00ULL;
    edge_ori |= (eo11 << 8) | (eo8 << 9) | (eo9 << 10) | (eo10 << 11);
}

void BitboardCube::move_R() {
    uint64_t c2 = (corner_pos >> (2 * 3)) & 0x7;
    uint64_t c3 = (corner_pos >> (3 * 3)) & 0x7;
    uint64_t c6 = (corner_pos >> (6 * 3)) & 0x7;
    uint64_t c7 = (corner_pos >> (7 * 3)) & 0x7;
    corner_pos &= ~((0x7ULL << 6) | (0x7ULL << 9) | (0x7ULL << 18) | (0x7ULL << 21));
    corner_pos |= (c3 << (2 * 3)) | (c7 << (3 * 3)) | (c2 << (6 * 3)) | (c6 << (7 * 3));

    uint64_t co2 = (corner_ori >> (2 * 2)) & 0x3;
    uint64_t co3 = (corner_ori >> (3 * 2)) & 0x3;
    uint64_t co6 = (corner_ori >> (6 * 2)) & 0x3;
    uint64_t co7 = (corner_ori >> (7 * 2)) & 0x3;
    corner_ori &= ~((0x3ULL << 4) | (0x3ULL << 6) | (0x3ULL << 12) | (0x3ULL << 14));
    corner_ori |= (((co3 + 1) % 3) << (2 * 2)) | (((co7 + 2) % 3) << (3 * 2)) | 
                  (((co2 + 2) % 3) << (6 * 2)) | (((co6 + 1) % 3) << (7 * 2));

    uint64_t e2  = (edge_pos >> (2 * 4))  & 0xF;
    uint64_t e6  = (edge_pos >> (6 * 4))  & 0xF;
    uint64_t e7  = (edge_pos >> (7 * 4))  & 0xF;
    uint64_t e10 = (edge_pos >> (10 * 4)) & 0xF;
    edge_pos &= ~((0xFULL << 8) | (0xFULL << 24) | (0xFULL << 28) | (0xFULL << 40));
    edge_pos |= (e6 << (2 * 4)) | (e10 << (6 * 4)) | (e2 << (7 * 4)) | (e7 << (10 * 4));

    uint64_t eo2  = (edge_ori >> 2)  & 0x1;
    uint64_t eo6  = (edge_ori >> 6)  & 0x1;
    uint64_t eo7  = (edge_ori >> 7)  & 0x1;
    uint64_t eo10 = (edge_ori >> 10) & 0x1;
    edge_ori &= ~((1ULL << 2) | (1ULL << 6) | (1ULL << 7) | (1ULL << 10));
    edge_ori |= (eo6 << 2) | (eo10 << 6) | (eo2 << 7) | (eo7 << 10);
}

void BitboardCube::move_L() {
    uint64_t c0 = (corner_pos >> (0 * 3)) & 0x7;
    uint64_t c1 = (corner_pos >> (1 * 3)) & 0x7;
    uint64_t c4 = (corner_pos >> (4 * 3)) & 0x7;
    uint64_t c5 = (corner_pos >> (5 * 3)) & 0x7;
    corner_pos &= ~((0x7ULL << 0) | (0x7ULL << 3) | (0x7ULL << 12) | (0x7ULL << 15));
    corner_pos |= (c4 << (0 * 3)) | (c0 << (1 * 3)) | (c5 << (4 * 3)) | (c1 << (5 * 3));

    uint64_t co0 = (corner_ori >> (0 * 2)) & 0x3;
    uint64_t co1 = (corner_ori >> (1 * 2)) & 0x3;
    uint64_t co4 = (corner_ori >> (4 * 2)) & 0x3;
    uint64_t co5 = (corner_ori >> (5 * 2)) & 0x3;
    corner_ori &= ~((0x3ULL << 0) | (0x3ULL << 2) | (0x3ULL << 8) | (0x3ULL << 10));
    corner_ori |= (((co4 + 1) % 3) << (0 * 2)) | (((co0 + 2) % 3) << (1 * 2)) |
                  (((co5 + 2) % 3) << (4 * 2)) | (((co1 + 1) % 3) << (5 * 2));

    uint64_t e0 = (edge_pos >> (0 * 4)) & 0xF;
    uint64_t e4 = (edge_pos >> (4 * 4)) & 0xF;
    uint64_t e5 = (edge_pos >> (5 * 4)) & 0xF;
    uint64_t e8 = (edge_pos >> (8 * 4)) & 0xF;
    edge_pos &= ~((0xFULL << 0) | (0xFULL << 16) | (0xFULL << 20) | (0xFULL << 32));
    edge_pos |= (e5 << (0 * 4)) | (e8 << (4 * 4)) | (e0 << (5 * 4)) | (e4 << (8 * 4));

    uint64_t eo0 = (edge_ori >> 0) & 0x1;
    uint64_t eo4 = (edge_ori >> 4) & 0x1;
    uint64_t eo5 = (edge_ori >> 5) & 0x1;
    uint64_t eo8 = (edge_ori >> 8) & 0x1;
    edge_ori &= ~((1ULL << 0) | (1ULL << 4) | (1ULL << 5) | (1ULL << 8));
    edge_ori |= (eo5 << 0) | (eo8 << 4) | (eo0 << 5) | (eo4 << 8);
}

void BitboardCube::move_F() {
    uint64_t c0 = (corner_pos >> (0 * 3)) & 0x7;
    uint64_t c3 = (corner_pos >> (3 * 3)) & 0x7;
    uint64_t c4 = (corner_pos >> (4 * 3)) & 0x7;
    uint64_t c7 = (corner_pos >> (7 * 3)) & 0x7;
    corner_pos &= ~((0x7ULL << 0) | (0x7ULL << 9) | (0x7ULL << 12) | (0x7ULL << 21));
    corner_pos |= (c3 << (0 * 3)) | (c0 << (3 * 3)) | (c7 << (4 * 3)) | (c4 << (7 * 3));

    uint64_t co0 = (corner_ori >> (0 * 2)) & 0x3;
    uint64_t co3 = (corner_ori >> (3 * 2)) & 0x3;
    uint64_t co4 = (corner_ori >> (4 * 2)) & 0x3;
    uint64_t co7 = (corner_ori >> (7 * 2)) & 0x3;
    corner_ori &= ~((0x3ULL << 0) | (0x3ULL << 6) | (0x3ULL << 8) | (0x3ULL << 14));
    corner_ori |= (((co3 + 1) % 3) << (0 * 2)) | (((co0 + 2) % 3) << (3 * 2)) |
                  (((co7 + 2) % 3) << (4 * 2)) | (((co4 + 1) % 3) << (7 * 2));

    uint64_t e3  = (edge_pos >> (3 * 4))  & 0xF;
    uint64_t e4  = (edge_pos >> (4 * 4))  & 0xF;
    uint64_t e7  = (edge_pos >> (7 * 4))  & 0xF;
    uint64_t e11 = (edge_pos >> (11 * 4)) & 0xF;
    edge_pos &= ~((0xFULL << 12) | (0xFULL << 16) | (0xFULL << 28) | (0xFULL << 44));
    edge_pos |= (e4 << (3 * 4)) | (e11 << (4 * 4)) | (e3 << (7 * 4)) | (e7 << (11 * 4));

    uint64_t eo3  = (edge_ori >> 3)  & 0x1;
    uint64_t eo4  = (edge_ori >> 4)  & 0x1;
    uint64_t eo7  = (edge_ori >> 7)  & 0x1;
    uint64_t eo11 = (edge_ori >> 11) & 0x1;
    edge_ori &= ~((1ULL << 3) | (1ULL << 4) | (1ULL << 7) | (1ULL << 11));
    edge_ori |= ((eo4 ^ 1) << 3) | ((eo11 ^ 1) << 4) | ((eo3 ^ 1) << 7) | ((eo7 ^ 1) << 11);
}

void BitboardCube::move_B() {
    uint64_t c1 = (corner_pos >> (1 * 3)) & 0x7;
    uint64_t c2 = (corner_pos >> (2 * 3)) & 0x7;
    uint64_t c5 = (corner_pos >> (5 * 3)) & 0x7;
    uint64_t c6 = (corner_pos >> (6 * 3)) & 0x7;
    corner_pos &= ~((0x7ULL << 3) | (0x7ULL << 6) | (0x7ULL << 15) | (0x7ULL << 18));
    corner_pos |= (c5 << (1 * 3)) | (c1 << (2 * 3)) | (c6 << (5 * 3)) | (c2 << (6 * 3));

    uint64_t co1 = (corner_ori >> (1 * 2)) & 0x3;
    uint64_t co2 = (corner_ori >> (2 * 2)) & 0x3;
    uint64_t co5 = (corner_ori >> (5 * 2)) & 0x3;
    uint64_t co6 = (corner_ori >> (6 * 2)) & 0x3;
    corner_ori &= ~((0x3ULL << 2) | (0x3ULL << 4) | (0x3ULL << 10) | (0x3ULL << 12));
    corner_ori |= (((co5 + 1) % 3) << (1 * 2)) | (((co1 + 2) % 3) << (2 * 2)) |
                  (((co6 + 2) % 3) << (5 * 2)) | (((co2 + 1) % 3) << (6 * 2));

    uint64_t e1 = (edge_pos >> (1 * 4)) & 0xF;
    uint64_t e5 = (edge_pos >> (5 * 4)) & 0xF;
    uint64_t e6 = (edge_pos >> (6 * 4)) & 0xF;
    uint64_t e9 = (edge_pos >> (9 * 4)) & 0xF;
    edge_pos &= ~((0xFULL << 4) | (0xFULL << 20) | (0xFULL << 24) | (0xFULL << 36));
    edge_pos |= (e5 << (1 * 4)) | (e9 << (5 * 4)) | (e1 << (6 * 4)) | (e6 << (9 * 4));

    uint64_t eo1 = (edge_ori >> 1) & 0x1;
    uint64_t eo5 = (edge_ori >> 5) & 0x1;
    uint64_t eo6 = (edge_ori >> 6) & 0x1;
    uint64_t eo9 = (edge_ori >> 9) & 0x1;
    edge_ori &= ~((1ULL << 1) | (1ULL << 5) | (1ULL << 6) | (1ULL << 9));
    edge_ori |= ((eo5 ^ 1) << 1) | ((eo9 ^ 1) << 5) | ((eo1 ^ 1) << 6) | ((eo6 ^ 1) << 9);
}
bool BitboardCube::isPhase1Ready() const {
    // 1. All corners must have 0 orientation bits
    if (corner_ori != 0x0ULL) return false;

    // 2. All edges must have 0 orientation bits
    if (edge_ori != 0x0ULL) return false;

    // 3. Scan all 12 tracking slots on the cube.
    // The equator pieces (0=FR, 1=FL, 2=BR, 3=BL) MUST occupy the 
    // equator physical slots (slots 0, 1, 2, and 3).
    for (int slot = 0; slot < 12; ++slot) {
        int piece = (edge_pos >> (slot * 4)) & 0xF;
        
        // If it's an equator piece, it MUST be sitting in an equator slot (0 to 3)
        if (piece >= 0 && piece <= 3) {
            if (slot > 3) return false; // This equator piece escaped to the top or bottom layer!
        }
    }

    return true;
}
void BitboardCube::copyStateFrom(const BitboardCube& other) {
    this->corner_pos = other.corner_pos;
    this->corner_ori = other.corner_ori;
    this->edge_pos   = other.edge_pos;
    this->edge_ori   = other.edge_ori;
}