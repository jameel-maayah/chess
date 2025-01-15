#pragma once

enum Piece : int {
    EMPTY = 0, 

    WHITE_PAWN,
    WHITE_KNIGHT,
    WHITE_BISHOP,
    WHITE_ROOK,
    WHITE_QUEEN,
    WHITE_KING,

    BLACK_PAWN,
    BLACK_KNIGHT,
    BLACK_BISHOP,
    BLACK_ROOK,
    BLACK_QUEEN,
    BLACK_KING

    // todo: rewrite as class
};

/*
class Piece {
    private:
        uint8_t repr;
};
*/
