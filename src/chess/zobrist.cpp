#pragma once

#include <stdlib.h>

#include "zobrist.h"
#include "chess.h"
#include "move.h"
#include "piece.h"
#include "bitboard.h"

uint64_t Zobrist::zobrist_table[64][16];
uint64_t Zobrist::zobrist_stm;

uint64_t Zobrist::prng(uint64_t seed) {
    return (seed * 6364136223846793005ULL + 1) >> 32;
}

void Zobrist::init_zobrist() {
    uint64_t seed = 11847;

    for (int square = 0; square < 64; square++) {
        for (int piece = 0; piece < 16; piece++) {
            seed = prng(seed);
            zobrist_table[square][piece] = seed;
        }
    }
    zobrist_stm = prng(seed);
}

uint64_t Zobrist::init_hash(Chess &game) {
    U64 occ = game.occupied_mask();
    uint64_t hash = 0;
    
    while (occ) {
        Square sq = bitboard::popLSB(occ);
        hash ^= zobrist_table[sq][game.at(sq).type()];
    }

    return hash;
}

uint64_t Zobrist::update_hash(uint64_t hash, const Move move, const Piece piece, const Piece capture) {
    hash ^= zobrist_table[move.source()][static_cast<int>(piece.type())];
    hash ^= zobrist_table[move.target()][static_cast<int>(piece.type())]; // todo: handle promotions
    if (!capture.empty()) {
        hash ^= zobrist_table[move.target()][static_cast<int>(capture.type())];
    }
    hash ^= zobrist_stm;

    return hash;
}