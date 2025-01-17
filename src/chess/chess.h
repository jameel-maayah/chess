#pragma once

#include <stdlib.h>
#include <string_view>

#include "bitboard.h"
#include "constants.h"
#include "piece.h"
#include "move.h"
#include "types.h"

/*
-zobrist
status, movetype, piece, square, etc.
*/


class Chess {
    public:
        Chess(std::string_view fen = START_FEN);
        ~Chess();

        void make_move(const Move move);
        void undo_move();

        void pseudo_move(const Move move);
        void pseudo_undo();
        
        constexpr inline U64 occupied() {
            return (bitboard[Piece::WHITE_PAWN] | 
                    bitboard[Piece::WHITE_KNIGHT] | 
                    bitboard[Piece::WHITE_BISHOP] | 
                    bitboard[Piece::WHITE_ROOK] | 
                    bitboard[Piece::WHITE_QUEEN] | 
                    bitboard[Piece::WHITE_KING] | 
                    bitboard[Piece::BLACK_PAWN] | 
                    bitboard[Piece::BLACK_KNIGHT] | 
                    bitboard[Piece::BLACK_BISHOP] | 
                    bitboard[Piece::BLACK_ROOK] | 
                    bitboard[Piece::BLACK_QUEEN] | 
                    bitboard[Piece::BLACK_KING]);
        }
        
        inline Piece at(Square sq) {
            return board[sq];
        }

        inline Color side_to_move() const { return stm; }
        void print();
    //private:
        Color stm;
        int ply;
        int fullmoves;

        Piece board[NUM_SQUARES];
        U64 bitboard[16];

        Square white_king, black_king;

        int halfmove_stack[MAX_GAME_LEN];
        int halfmove_counter;

        Move move_history[MAX_GAME_LEN];
        Piece captures[MAX_GAME_LEN];
        Status status;
        
        uint64_t zobrist_hash;
        uint64_t zobrist_history[MAX_GAME_LEN];
};
