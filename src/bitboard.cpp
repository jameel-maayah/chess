#include <stdlib.h>
#include <cstdint>
#include <iostream>

#include "bitboard.h"

namespace bitboard {
    using Square = int;
    
    [[nodiscard]] inline Square popLSB(U64 *bb) {
        int rshift = __builtin_ctzll(*bb);
        *bb &= *bb - 1;
        return rshift;
    }

}

std::ostream& operator<<(std::ostream& os, const U64& bb) noexcept {
    for (int j = 7; j >= 0; j--) {
        for (int i = 0; i < 8; i++) {
            os << (bitboard::get_bit(bb, 8 * i + j) ? 1 : 0) << " ";
        }
        os << std::endl;
    }
    return os;
}