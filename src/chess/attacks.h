#include <bit>
#include <array>

#include "chess.h"
#include "bitboard.h"
#include "types.h"

// Source https://www.chessprogramming.org/Hyperbola_Quintessence

constexpr U64 compute_diag_mask(int sq) {
    U64 mask = 0;
    int rank = sq / 8, file = sq % 8;
    for (int r = rank, f = file; r < 8 && f < 8; ++r, ++f)
        mask |= 1ULL << (r * 8 + f);
    for (int r = rank - 1, f = file - 1; r >= 0 && f >= 0; --r, --f)
        mask |= 1ULL << (r * 8 + f);
    return mask;
}

constexpr U64 compute_anti_diag_mask(int sq) {
    U64 mask = 0;
    int rank = sq / 8, file = sq % 8;
    for (int r = rank, f = file; r < 8 && f >= 0; ++r, --f)
        mask |= 1ULL << (r * 8 + f);
    for (int r = rank - 1, f = file + 1; r >= 0 && f < 8; --r, ++f)
        mask |= 1ULL << (r * 8 + f);
    return mask;
}

constexpr U64 compute_file_mask(int sq) {
    U64 mask = 0;
    int file = sq % 8;
    for (int rank = 0; rank < 8; ++rank)
        mask |= 1ULL << (rank * 8 + file);

    return mask;
}

/*
constexpr U64 compute_rank_mask(int sq) {
    U64 mask = 0;
    int rank = sq / 8;
    for (int file = 0; file < 8; ++file)
        mask |= 1ULL << (rank * 8 + file);

    return mask;
}
*/

constexpr U64 compute_knight_mask(int sq) {
    U64 mask = 0;
    int rank = sq / 8, file = sq % 8;

    constexpr int offsets[8][2] = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    };

    for (const auto& [dr, df] : offsets) {
        int r = rank + dr, f = file + df;
        if (r >= 0 && r < 8 && f >= 0 && f < 8) {
            mask |= 1ULL << (r * 8 + f);
        }
    }

    return mask;
}

constexpr U64 compute_king_mask(int sq) {
    U64 mask = 0;
    int rank = sq / 8, file = sq % 8;

    constexpr int offsets[8][2] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };

    for (const auto& [dr, df] : offsets) {
        int r = rank + dr, f = file + df;
        if (r >= 0 && r < 8 && f >= 0 && f < 8) {
            mask |= 1ULL << (r * 8 + f);
        }
    }

    return mask;
}

constexpr U64 compute_bit_mask(int sq) {
    return 1ULL << sq;
}

template <int... Indices>
constexpr std::array<U64, sizeof...(Indices)> precompute_mask(U64 (*func)(int), std::integer_sequence<int, Indices...>) {
    return {func(Indices)...};
}

static constexpr std::array<U64, 64> DIAG_MASK =           precompute_mask(compute_diag_mask,          std::make_integer_sequence<int, 64>{});
static constexpr std::array<U64, 64> ANTI_DIAG_MASK =      precompute_mask(compute_anti_diag_mask,     std::make_integer_sequence<int, 64>{});
static constexpr std::array<U64, 64> FILE_MASK =           precompute_mask(compute_file_mask,          std::make_integer_sequence<int, 64>{});
//static constexpr std::array<U64, 64> RANK_MASK =           precompute_mask(compute_rank_mask,          std::make_integer_sequence<int, 64>{});
static constexpr std::array<U64, 64> KNIGHT_MASK =         precompute_mask(compute_knight_mask,        std::make_integer_sequence<int, 64>{});
static constexpr std::array<U64, 64> KING_MASK =           precompute_mask(compute_king_mask,          std::make_integer_sequence<int, 64>{});
//constexpr std::array<U64, 64> PAWN_MASK =           precompute_mask(compute_pawn_mask,          std::make_integer_sequence<int, 64>{});
// add pawn masks

static constexpr std::array<U64, 64> BIT_MASK =            precompute_mask(compute_bit_mask,           std::make_integer_sequence<int, 64>{});



class Attacks {

    public:
        static U64 bishop_attacks(U64 occ, Square sq) {
            return get_diagonal(occ, sq)
                 | get_anti_diagonal(occ, sq);
        }

        static U64 rook_attacks(U64 occ, Square sq) {
            return get_file(occ, sq)
                 | get_rank(occ, sq);
        }

        static U64 queen_attacks(U64 occ, Square sq) {
            return get_diagonal(occ, sq)
                 | get_anti_diagonal(occ, sq)
                 | get_file(occ, sq)
                 | get_rank(occ, sq);
        }

        static inline U64 knight_attacks(Square sq) {
            return KNIGHT_MASK[static_cast<int>(sq)];
        }

        static inline U64 king_attacks(Square sq) {
            return KING_MASK[static_cast<int>(sq)];
        }

        static inline U64 pawn_attacks(U64 occ, Square sq, Color color) { // Add field for EP later
            /*
            switch (color) {
                case WHITE:

                case BLACK:

            }
            */
            return 0ULL;

            //write this
        }

    private:
        static inline U64 get_diagonal(U64 occ, const Square sq) { 
            U64 forward, reverse;
            //occ &= ~0xFF818181818181FFULL; // what the hell?
            bitboard::clear_bit(occ, sq);

            forward = occ & DIAG_MASK[static_cast<int>(sq)];
            reverse  = std::byteswap(forward);
            forward -= BIT_MASK[static_cast<int>(sq)];
            reverse -= std::byteswap(BIT_MASK[static_cast<int>(sq)]);
            forward ^= std::byteswap(reverse);
            forward &= DIAG_MASK[static_cast<int>(sq)];
            return forward;
        }

        static inline U64 get_anti_diagonal(U64 occ, const Square sq) { 
            U64 forward, reverse;
            bitboard::clear_bit(occ, sq);

            forward  = occ & ANTI_DIAG_MASK[static_cast<int>(sq)];
            reverse  = std::byteswap(forward);
            forward -= BIT_MASK[static_cast<int>(sq)];
            reverse -= std::byteswap(BIT_MASK[static_cast<int>(sq)]);
            forward ^= std::byteswap(reverse);
            forward &= ANTI_DIAG_MASK[static_cast<int>(sq)];
            return forward;
        }

        static inline U64 get_file(U64 occ, const Square sq) { 
            U64 forward, reverse;
            bitboard::clear_bit(occ, sq);
            
            forward  = occ & FILE_MASK[static_cast<int>(sq)];
            reverse  = std::byteswap(forward);
            forward -= BIT_MASK[static_cast<int>(sq)];
            reverse -= std::byteswap(BIT_MASK[static_cast<int>(sq)]);
            forward ^= std::byteswap(reverse);
            forward &= FILE_MASK[static_cast<int>(sq)];
            return forward;
        }

        static inline U64 get_rank(U64 occ, Square sq) {
            /*
            unsigned int file = sq &  7;
            unsigned int rank_x8 = sq & 56;
            unsigned int rank_occ_x2 = (occ >> rank_x8) & 2*63;
            U64 attacks = RANK_MASK[4*rank_occ_x2  + file];
            return attacks << rank_x8;
            */

            // find a faster way
            U64 attacks = 0ULL;
            occ |= 0x8181818181818181ULL;

            if (::get_file(sq) != A_FILE) {
                for (Square dest = sq + Direction::W; ; dest += Direction::W) {
                    bitboard::set_bit(attacks, dest);
                    if (bitboard::get_bit(occ, dest)) break;
                }
            }

            if (::get_file(sq) != H_FILE) {
                for (Square dest = sq + Direction::E; ; dest += Direction::E) {
                    bitboard::set_bit(attacks, dest);
                    if (bitboard::get_bit(occ, dest)) break;
                }
            }

            return attacks;
        }
};