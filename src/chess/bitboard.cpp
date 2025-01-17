#include <stdlib.h>
#include <cstdint>
#include <iostream>

#include <string>

#include "bitboard.h"
#include "types.h"

namespace bitboard {// rewrite as class instead of namespace?    
    [[nodiscard]] inline Square popLSB(U64 *bb) {
        int rshift = __builtin_ctzll(*bb);
        *bb &= *bb - 1;
        return static_cast<Square>(rshift);
    }
}

std::ostream& operator<<(std::ostream& os, const U64& bb) noexcept {
    for (int j = 0; j < 8; j++) {
        for (int i = 7; i >= 0; i--) {
            os << (bitboard::get_bit(bb, 8 * i + j) ? 1 : 0) << " ";
        }
        os << std::endl;
    }
    return os;
}

void print_bitboard(const U64 bb) {
    /*
    for (int j = 0; j < 8; j++) {
        for (int i = 7; i >= 0; i--) {
            std::cout << (bitboard::get_bit(bb, 8 * i + j) ? 1 : 0) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    */

    std::cout << std::endl;

    std::cout << bitboard::get_bit(bb, static_cast<int>(a8)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(b8)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(c8)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(d8)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(e8)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(f8)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(g8)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(h8)) << " ";
    std::cout << std::endl;

    std::cout << bitboard::get_bit(bb, static_cast<int>(a7)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(b7)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(c7)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(d7)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(e7)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(f7)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(g7)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(h7)) << " ";
    std::cout << std::endl;

    std::cout << bitboard::get_bit(bb, static_cast<int>(a6)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(b6)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(c6)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(d6)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(e6)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(f6)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(g6)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(h6)) << " ";
    std::cout << std::endl;

    std::cout << bitboard::get_bit(bb, static_cast<int>(a5)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(b5)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(c5)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(d5)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(e5)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(f5)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(g5)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(h5)) << " ";
    std::cout << std::endl;

    std::cout << bitboard::get_bit(bb, static_cast<int>(a4)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(b4)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(c4)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(d4)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(e4)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(f4)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(g4)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(h4)) << " ";
    std::cout << std::endl;

    std::cout << bitboard::get_bit(bb, static_cast<int>(a3)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(b3)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(c3)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(d3)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(e3)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(f3)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(g3)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(h3)) << " ";
    std::cout << std::endl;

    std::cout << bitboard::get_bit(bb, static_cast<int>(a2)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(b2)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(c2)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(d2)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(e2)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(f2)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(g2)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(h2)) << " ";
    std::cout << std::endl;

    std::cout << bitboard::get_bit(bb, static_cast<int>(a1)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(b1)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(c1)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(d1)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(e1)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(f1)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(g1)) << " ";
    std::cout << bitboard::get_bit(bb, static_cast<int>(h1)) << " ";
    std::cout << std::endl;
}