#include <stdlib.h>
#include <cstdint>
#include <iostream>

#include <string>

#include "bitboard.h"
#include "types.h"

Square bitboard::popLSB(U64 &bb) { // Rewrite as class instead of namespace?
    int rshift = __builtin_ctzll(bb);
    bb &= bb - 1;
    return static_cast<Square>(rshift);
}

std::ostream& operator<<(std::ostream& os, const U64& bb) noexcept {
    for (int j = 0; j < 8; j++) {
        for (int i = 7; i >= 0; i--) {
            os << (bitboard::get_bit(bb, static_cast<Square>(8 * i + j)) ? 1 : 0) << " ";
        }
        os << std::endl;
    }
    return os;
}

void print_bitboard(const U64 bb) {
    std::cout << std::endl;

    std::cout << bitboard::get_bit(bb, a8) << " ";
    std::cout << bitboard::get_bit(bb, b8) << " ";
    std::cout << bitboard::get_bit(bb, c8) << " ";
    std::cout << bitboard::get_bit(bb, d8) << " ";
    std::cout << bitboard::get_bit(bb, e8) << " ";
    std::cout << bitboard::get_bit(bb, f8) << " ";
    std::cout << bitboard::get_bit(bb, g8) << " ";
    std::cout << bitboard::get_bit(bb, h8) << " ";
    std::cout << std::endl;

    std::cout << bitboard::get_bit(bb, a7) << " ";
    std::cout << bitboard::get_bit(bb, b7) << " ";
    std::cout << bitboard::get_bit(bb, c7) << " ";
    std::cout << bitboard::get_bit(bb, d7) << " ";
    std::cout << bitboard::get_bit(bb, e7) << " ";
    std::cout << bitboard::get_bit(bb, f7) << " ";
    std::cout << bitboard::get_bit(bb, g7) << " ";
    std::cout << bitboard::get_bit(bb, h7) << " ";
    std::cout << std::endl;

    std::cout << bitboard::get_bit(bb, a6) << " ";
    std::cout << bitboard::get_bit(bb, b6) << " ";
    std::cout << bitboard::get_bit(bb, c6) << " ";
    std::cout << bitboard::get_bit(bb, d6) << " ";
    std::cout << bitboard::get_bit(bb, e6) << " ";
    std::cout << bitboard::get_bit(bb, f6) << " ";
    std::cout << bitboard::get_bit(bb, g6) << " ";
    std::cout << bitboard::get_bit(bb, h6) << " ";
    std::cout << std::endl;

    std::cout << bitboard::get_bit(bb, a5) << " ";
    std::cout << bitboard::get_bit(bb, b5) << " ";
    std::cout << bitboard::get_bit(bb, c5) << " ";
    std::cout << bitboard::get_bit(bb, d5) << " ";
    std::cout << bitboard::get_bit(bb, e5) << " ";
    std::cout << bitboard::get_bit(bb, f5) << " ";
    std::cout << bitboard::get_bit(bb, g5) << " ";
    std::cout << bitboard::get_bit(bb, h5) << " ";
    std::cout << std::endl;

    std::cout << bitboard::get_bit(bb, a4) << " ";
    std::cout << bitboard::get_bit(bb, b4) << " ";
    std::cout << bitboard::get_bit(bb, c4) << " ";
    std::cout << bitboard::get_bit(bb, d4) << " ";
    std::cout << bitboard::get_bit(bb, e4) << " ";
    std::cout << bitboard::get_bit(bb, f4) << " ";
    std::cout << bitboard::get_bit(bb, g4) << " ";
    std::cout << bitboard::get_bit(bb, h4) << " ";
    std::cout << std::endl;

    std::cout << bitboard::get_bit(bb, a3) << " ";
    std::cout << bitboard::get_bit(bb, b3) << " ";
    std::cout << bitboard::get_bit(bb, c3) << " ";
    std::cout << bitboard::get_bit(bb, d3) << " ";
    std::cout << bitboard::get_bit(bb, e3) << " ";
    std::cout << bitboard::get_bit(bb, f3) << " ";
    std::cout << bitboard::get_bit(bb, g3) << " ";
    std::cout << bitboard::get_bit(bb, h3) << " ";
    std::cout << std::endl;

    std::cout << bitboard::get_bit(bb, a2) << " ";
    std::cout << bitboard::get_bit(bb, b2) << " ";
    std::cout << bitboard::get_bit(bb, c2) << " ";
    std::cout << bitboard::get_bit(bb, d2) << " ";
    std::cout << bitboard::get_bit(bb, e2) << " ";
    std::cout << bitboard::get_bit(bb, f2) << " ";
    std::cout << bitboard::get_bit(bb, g2) << " ";
    std::cout << bitboard::get_bit(bb, h2) << " ";
    std::cout << std::endl;

    std::cout << bitboard::get_bit(bb, a1) << " ";
    std::cout << bitboard::get_bit(bb, b1) << " ";
    std::cout << bitboard::get_bit(bb, c1) << " ";
    std::cout << bitboard::get_bit(bb, d1) << " ";
    std::cout << bitboard::get_bit(bb, e1) << " ";
    std::cout << bitboard::get_bit(bb, f1) << " ";
    std::cout << bitboard::get_bit(bb, g1) << " ";
    std::cout << bitboard::get_bit(bb, h1) << " ";
    std::cout << std::endl;
}