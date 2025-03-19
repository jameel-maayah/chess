#pragma once

#include <stdlib.h>

#include "chess.h"
#include "move.h"
#include "piece.h"

class Zobrist {
    private:
        // add castling + EP later
        static uint64_t zobrist_table[64][16];
        static uint64_t zobrist_stm;
        static uint64_t prng(uint64_t seed);

    public:
        static void init_zobrist();
        static uint64_t init_hash(Chess &game);
        static uint64_t update_hash(uint64_t hash, const Move move, const Piece piece, const Piece capture);
};