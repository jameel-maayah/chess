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

        using Move = ::Move;
        using Color = ::Color;
        using Status = ::Status;

        void make_move(const Move move);
        void undo_move();

        void pseudo_move(const Move move);
        void pseudo_undo();

        [[nodiscard]] inline int get_ply() const                { return ply; }
        [[nodiscard]] inline Status get_status() const          { return status; }
        [[nodiscard]] inline Piece at(const Square sq) const    { return board[sq]; }
        [[nodiscard]] inline Color side_to_move() const         { return stm; }

        [[nodiscard]] inline Square get_white_king() const      { return white_king; }
        [[nodiscard]] inline Square get_black_king() const      { return black_king; }
        [[nodiscard]] bool is_attacked(const Square sq);
        
        [[nodiscard]] inline U64 occupied_mask() const {
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

        [[nodiscard]] inline U64 white_mask() const {
            return (bitboard[Piece::WHITE_PAWN] | 
                    bitboard[Piece::WHITE_KNIGHT] | 
                    bitboard[Piece::WHITE_BISHOP] | 
                    bitboard[Piece::WHITE_ROOK] | 
                    bitboard[Piece::WHITE_QUEEN] | 
                    bitboard[Piece::WHITE_KING]);
        }

        [[nodiscard]] inline U64 black_mask() const {
            return (bitboard[Piece::BLACK_PAWN] | 
                    bitboard[Piece::BLACK_KNIGHT] | 
                    bitboard[Piece::BLACK_BISHOP] | 
                    bitboard[Piece::BLACK_ROOK] | 
                    bitboard[Piece::BLACK_QUEEN] | 
                    bitboard[Piece::BLACK_KING]);
        }

        [[nodiscard]] inline U64 piece_mask(const Piece piece) const {
            return bitboard[piece.type()];
        }
        
        void print();
        void fancy_print();
    private:
        Color stm;
        int ply;
        int fullmoves;

        Piece board[NUM_SQUARES];
        U64 bitboard[16]; //NUM_PIECES

        Square white_king, black_king;

        int halfmove_stack[MAX_GAME_LEN];
        int halfmove_counter;

        Move move_history[MAX_GAME_LEN];
        Piece captures[MAX_GAME_LEN];
        Status status;
        
        uint64_t zobrist_hash;
        uint64_t zobrist_history[MAX_GAME_LEN];
};
