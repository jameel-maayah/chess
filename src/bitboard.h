#pragma once

#include <stdlib.h>
#include <cstdint>

typedef uint64_t U64;

namespace bitboard {
    using Square = int;
    //using U64 = uint64_t;

    inline bool get_bit(U64 bb, Square sq) { return (bb >> sq) & 1ULL; }
    inline bool set_bit(U64 &bb, Square sq) { bb |= (1ULL << sq); }
    inline bool clear_bit(U64 &bb, Square sq) { bb &= ~(1ULL << sq); }
    inline bool toggle_bit(U64 &bb, Square sq) { bb ^= (1ULL << sq); }
    inline bool toggle_bits(U64 &bb, Square sq1, Square sq2) { bb ^= ((1ULL << sq1) | (1ULL << sq2)); }

    [[nodiscard]] inline Square popLSB(U64 *bb);

    //std::ostream& operator<<(std::ostream& os, const U64& bb) noexcept;
    
}

std::ostream& operator<<(std::ostream& os, const U64& bb) noexcept;