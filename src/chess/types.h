#pragma once

/******************/

enum Status : int { // make this class
    IN_PROGRESS = 0,

    WIN_WHITE,
    WIN_BLACK,
    STALEMATE,
    THREEFOLD,
    FIFTY_MOVE,
    INSUFFICIENT_MATERIAL
};

enum Color : int { // make this class
	WHITE = 0, 
    BLACK = 1
};

constexpr Color operator~(Color color) {
    return static_cast<Color>(color ^ BLACK);
}

enum File : int {
	A_FILE = 0,
    B_FILE,
    C_FILE,
    D_FILE,
    E_FILE,
    F_FILE,
    G_FILE,
    H_FILE
};

enum Rank : int {
	RANK_1 = 0,
    RANK_2,
    RANK_3,
    RANK_4,
    RANK_5,
    RANK_6,
    RANK_7,
    RANK_8
};

constexpr inline Rank& operator++(Rank& rank)     {   return rank = static_cast<Rank>(int(rank) + 1);     }
constexpr inline Rank& operator--(Rank& rank)     {   return rank = static_cast<Rank>(int(rank) - 1);     }

enum Square : int {
    a1 = 0, 
        b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8,
    NULL_SQ = 64
};

constexpr inline bool is_peripheral(const Square sq) {
    return (sq & 0x07) == 0 ||
           (sq & 0x07) == 7 ||
           (sq & 0x38) == 0 ||
           (sq & 0x38) == 0x38;
}

constexpr inline Rank get_rank(Square sq) {
    return static_cast<Rank>(sq / 8);
}

constexpr inline Rank get_file(Square sq) {
    return static_cast<Rank>(sq % 8); // sq & 7
}

enum class Direction : int {
/*
a8                  h8

    +7    +8    +9
        \  |  /
    -1 <-  0 -> +1  
        /  |  \
    -9    -8    -7

a1                  h1
*/
    N = 8,
    NE = 9,
    E = 1,
    SE = -7,
    S = -8,
    SW = -9,
    W = -1,
    NW = 7,

    // for pawns
    NN = 16,
    SS = -16
};

constexpr inline Square operator+(Square sq, Direction dir) {   return static_cast<Square>(int(sq) + int(dir));    }
constexpr inline Square operator-(Square sq, Direction dir) {   return static_cast<Square>(int(sq) - int(dir));    }

constexpr inline Square& operator++(Square& sq)                       {   return sq = static_cast<Square>(int(sq) + 1); }
constexpr inline Square& operator--(Square& sq)                       {   return sq = static_cast<Square>(int(sq) - 1); }
constexpr inline Square& operator+=(Square& sq, Direction dir)        {   return sq = sq + dir;   }
constexpr inline Square& operator-=(Square& sq, Direction dir)        {   return sq = sq - dir;   }