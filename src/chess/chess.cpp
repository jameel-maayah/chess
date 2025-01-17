#include <iostream>

#include <string_view>
#include <sstream>
#include <ranges>

#include <vector>

#include "chess.h"
#include "bitboard.h"
#include "constants.h"
#include "types.h"
#include "piece.h"
#include "move.h"

#define IX(i, j) (((8) * (i)) + (j))

Chess::Chess(std::string_view fen) {
    const std::string_view delimiter = " ";
    auto args = std::views::split(fen, delimiter);

    for (const auto& arg : args) {
        std::string token(arg.begin(), arg.end()); 
        std::cout << token << std::endl;
    }
    // todo: fen parsing

    memset(board, 0, sizeof(board)); // EMPTY
    
    for (Square sq = a2; sq <= h2; sq += E) {
        //board[sq] = WHITE_PAWN;
        board[sq] = Piece(Piece::WHITE_PAWN);
    }
    for (Square sq = a7; sq <= h7; sq += E) {
        //board[sq] = BLACK_PAWN;
        board[sq] = Piece(Piece::BLACK_PAWN);
    }
    /*
    board[a1] = WHITE_ROOK;
    board[b1] = WHITE_KNIGHT;
    board[c1] = WHITE_BISHOP;
    board[d1] = WHITE_QUEEN;
    board[e1] = WHITE_KING;
    board[f1] = WHITE_BISHOP;
    board[g1] = WHITE_KNIGHT;
    board[h1] = WHITE_ROOK;

    board[a8] = BLACK_ROOK;
    board[b8] = BLACK_KNIGHT;
    board[c8] = BLACK_BISHOP;
    board[d8] = BLACK_QUEEN;
    board[e8] = BLACK_KING;
    board[f8] = BLACK_BISHOP;
    board[g8] = BLACK_KNIGHT;
    board[h8] = BLACK_ROOK;
    */

    // ugly please fix
    board[a1] = Piece(Piece::WHITE_ROOK);
    board[b1] = Piece(Piece::WHITE_KNIGHT);
    board[c1] = Piece(Piece::WHITE_BISHOP);
    board[d1] = Piece(Piece::WHITE_QUEEN);
    board[e1] = Piece(Piece::WHITE_KING);
    board[f1] = Piece(Piece::WHITE_BISHOP);
    board[g1] = Piece(Piece::WHITE_KNIGHT);
    board[h1] = Piece(Piece::WHITE_ROOK);

    board[a8] = Piece(Piece::BLACK_ROOK);
    board[b8] = Piece(Piece::BLACK_KNIGHT);
    board[c8] = Piece(Piece::BLACK_BISHOP);
    board[d8] = Piece(Piece::BLACK_QUEEN);
    board[e8] = Piece(Piece::BLACK_KING);
    board[f8] = Piece(Piece::BLACK_BISHOP);
    board[g8] = Piece(Piece::BLACK_KNIGHT);
    board[h8] = Piece(Piece::BLACK_ROOK);

    ply = 0;

    move_history[0] = Move();
    captures[0] = Piece(Piece::EMPTY);

    halfmove_stack[0] = 0;
    halfmove_counter = 0;
    status = IN_PROGRESS;
    zobrist_hash = 0;

    white_king = e1;
    black_king = e8;

    memset(bitboard, 0ULL, sizeof(bitboard));

    for (Square sq = a1; sq <= h8; ++sq) {
        bitboard::set_bit(bitboard[board[sq].type()], sq);
        //zobrist_hash ^= zobrist_table[sq][board[sq].type()];
    }

    zobrist_history[ply] = zobrist_hash;
}

Chess::~Chess() { /* might need later? */ }

void Chess::make_move(const Move move) {
    //todo
}

void Chess::undo_move() {
    //todo
}

//pseudo move & unmove

void Chess::print() {
    printf("status %d, hash %d\n", status, zobrist_hash);

    // todo: stop printing board upside down
    for (Square sq = a1; sq < NULL_SQ; ++sq) {
        std::cout << std::string(board[sq]);
        if (int(sq+1) % 8 == 0) std::cout << std::endl;
    }
    // todo: ascii piece representations
}