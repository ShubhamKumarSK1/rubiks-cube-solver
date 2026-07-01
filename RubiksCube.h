#ifndef RUBIKS_CUBE_H
#define RUBIKS_CUBE_H

enum class MOVE {
    U, U_PRIME, U2,
    D, D_PRIME, D2,
    R, R_PRIME, R2,
    L, L_PRIME, L2,
    F, F_PRIME, F2,
    B, B_PRIME, B2
};

class RubiksCube {
public:
    virtual ~RubiksCube() {}
    virtual RubiksCube& move(MOVE ind) = 0;
    virtual bool isSolved() const = 0;
    virtual void printBits() const = 0;
};

#endif // RUBIKS_CUBE_H