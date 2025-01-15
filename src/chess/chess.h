#pragma once

#include <stdlib.h>
#include <string_view>

#include "bitboard.h"
#include "constants.h"
#include "piece.h"
#include "move.h"

/*
-zobrist
status, movetype, piece, square, etc.
*/


class Chess {
    public:
        Chess(std::string_view fen = START_FEN);
        ~Chess();

        void print();
    private:
        Color stm;
        int ply;
        int fullmoves;

        Piece board[NUM_SQUARES];
        U64 bitboard[13];

        Square white_king, black_king;

        int halfmove_stack[MAX_GAME_LEN];
        int halfmove_counter;

        Move move_history[MAX_GAME_LEN];
        Piece captures[MAX_GAME_LEN];
        Status status;
        
        uint64_t zobrist_hash;
        uint64_t zobrist_history[MAX_GAME_LEN];
};
