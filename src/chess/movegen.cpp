#include <vector>

#include <iostream>
#include <string>

#include <cassert>
#include <algorithm>

#include "movegen.h"

#include "chess.h"
#include "bitboard.h"
#include "constants.h"
#include "types.h"
#include "piece.h"
#include "move.h"
#include "attacks.h"


using std::vector;


int perft(Chess *game, int depth) {
    if (!depth) {
        //for (int i = 0; i < game->ply; i++) {
        //    std::cout << std::string(game->move_history[i]) << " ";
        //} std::cout << std::endl;
        return 1;
    }
    int sum = 0;
    std::vector<Move> moves = generate_moves(game);

    for (const Move &move : moves) {
        U64 wp = game->bitboard[Piece::WHITE_PAWN];
        U64 bp = game->bitboard[Piece::BLACK_PAWN];
        U64 wn = game->bitboard[Piece::WHITE_KNIGHT];
        U64 bn = game->bitboard[Piece::BLACK_KNIGHT];
        U64 wb = game->bitboard[Piece::WHITE_BISHOP];
        U64 bb = game->bitboard[Piece::BLACK_BISHOP];
        U64 wr = game->bitboard[Piece::WHITE_ROOK];
        U64 br = game->bitboard[Piece::BLACK_ROOK];
        U64 wq = game->bitboard[Piece::WHITE_QUEEN];
        U64 bq = game->bitboard[Piece::BLACK_QUEEN];
        U64 wk = game->bitboard[Piece::WHITE_KING];
        U64 bk = game->bitboard[Piece::BLACK_KING];

        Piece old[64];
        memcpy(old, game->board, sizeof(Piece) * 64);

        game->make_move(move);
        sum += perft(game, depth-1);
        game->undo_move();

        assert(wp == game->bitboard[Piece::WHITE_PAWN] && "WHITE_PAWN bitboard does not match!");
        assert(bp == game->bitboard[Piece::BLACK_PAWN] && "BLACK_PAWN bitboard does not match!");
        assert(wn == game->bitboard[Piece::WHITE_KNIGHT] && "WHITE_KNIGHT bitboard does not match!");
        assert(bn == game->bitboard[Piece::BLACK_KNIGHT] && "BLACK_KNIGHT bitboard does not match!");
        assert(wb == game->bitboard[Piece::WHITE_BISHOP] && "WHITE_BISHOP bitboard does not match!");
        assert(bb == game->bitboard[Piece::BLACK_BISHOP] && "BLACK_BISHOP bitboard does not match!");
        assert(wr == game->bitboard[Piece::WHITE_ROOK] && "WHITE_ROOK bitboard does not match!");
        assert(br == game->bitboard[Piece::BLACK_ROOK] && "BLACK_ROOK bitboard does not match!");
        assert(wq == game->bitboard[Piece::WHITE_QUEEN] && "WHITE_QUEEN bitboard does not match!");
        assert(bq == game->bitboard[Piece::BLACK_QUEEN] && "BLACK_QUEEN bitboard does not match!");
        assert(wk == game->bitboard[Piece::WHITE_KING] && "WHITE_KING bitboard does not match!");
        assert(bk == game->bitboard[Piece::BLACK_KING] && "BLACK_KING bitboard does not match!");

        assert(std::equal(old, old + 64, game->board) && "Boards are not equal!");
    }
    return sum;
}

int detail_perft(Chess *game, int depth) {
    if (!depth) {
        /*
        for (int i = 0; i < game->ply; i++) {
            std::cout << std::string(game->move_history[i]) << " ";
        } std::cout << std::endl;
        */
        //game->print(); std::cout << std::endl; 
        //if (!game->captures[2].empty()) std::cout << "CAPTURE\n";
        //if (game->is_attacked(game->black_king)) std::cout << "CHECK\n";
        return 1;
    }
    int sum = 0;
    std::vector<Move> moves = generate_moves(game);

    for (const Move &move : moves) {
        U64 occ = game->occupied();
        
        game->make_move(move);

        int variation = perft(game, depth-1);
        std::cout << std::string(move) << ":" << variation << std::endl;
        sum += variation;

        game->undo_move();

        assert(game->occupied() == occ);
    }
    return sum;
}


void add_to_movelist(Chess *game, vector<Move> &moves, const Move move) {

    game->pseudo_move(move);
    if (game->side_to_move() == BLACK && game->is_attacked(game->white_king)) {
        game->pseudo_undo();
        return;
    }
    if (game->side_to_move() == WHITE && game->is_attacked(game->black_king)) {
        game->pseudo_undo();
        return;
    }
    game->pseudo_undo();
    
    moves.push_back(move);
}


vector<Move> generate_moves(Chess *game) {
    Piece piece;
    Square square;

    vector<Move> moves = vector<Move>();
    moves.reserve(MAX_MOVES);

    const U64 occ = game->occupied();

    for (square = a1; square <= h8; ++square) {//todo: iterate through occupied squares only w/bitmask   --- while (occ) square = popLSB(occ)
        piece = game->at(square);

        if (piece.empty() | (piece.color() != game->side_to_move())){
            continue;
        }

        U64 attacks = 0ULL;

        switch (piece.type()) {
            //todo: concatenate black and white into single type
            case Piece::WHITE_PAWN:
                //pawn attacks - rewrite with bitboards
                //attacks = Attacks::pawn_attacks(occ, square, WHITE);
                if ((::get_rank(square) != RANK_8) && !bitboard::get_bit(occ, square + Direction::N)) {
                    add_to_movelist(game, moves, Move(square, square + Direction::N));
                    if ((::get_rank(square) == RANK_2) && !bitboard::get_bit(occ, square + Direction::NN)) {
                        add_to_movelist(game, moves, Move(square, square + Direction::NN));
                    }
                }

                if ((::get_file(square) != A_FILE) && 
                    (::get_rank(square) != RANK_8) &&
                    bitboard::get_bit(occ, square + Direction::NW) &&
                    game->at(square + Direction::NW).color() == BLACK) {
                    add_to_movelist(game, moves, Move(square, square + Direction::NW));
                }

                if ((::get_file(square) != H_FILE) && 
                    (::get_rank(square) != RANK_8) && 
                    bitboard::get_bit(occ, square + Direction::NE) &&
                    game->at(square + Direction::NE).color() == BLACK) {
                    add_to_movelist(game, moves, Move(square, square + Direction::NE));
                }

                break;

            case Piece::WHITE_KNIGHT:
                attacks = Attacks::knight_attacks(square);
                break;

            case Piece::WHITE_BISHOP:
                attacks = Attacks::bishop_attacks(occ, square);
                break;

            case Piece::WHITE_ROOK:
                attacks = Attacks::rook_attacks(occ, square);
                break;
                
            case Piece::WHITE_QUEEN:
                attacks = Attacks::queen_attacks(occ, square);
                break;

            case Piece::WHITE_KING:
                attacks = Attacks::king_attacks(square);
                break;

            case Piece::BLACK_PAWN:
                //attacks = Attacks::pawn_attacks(occ, square, BLACK);
                if ((::get_rank(square) != RANK_1) && !bitboard::get_bit(occ, square + Direction::S)) {
                    add_to_movelist(game, moves, Move(square, square + Direction::S));
                    if ((::get_rank(square) == RANK_7) && !bitboard::get_bit(occ, square + Direction::SS)) {
                        add_to_movelist(game, moves, Move(square, square + Direction::SS));
                    }
                }

                // attack set should be done w/ bitboards
                if ((::get_file(square) != A_FILE) && 
                    (::get_rank(square) != RANK_1) &&
                    bitboard::get_bit(occ, square + Direction::SW) &&
                    game->at(square + Direction::SW).color() == WHITE) {
                    add_to_movelist(game, moves, Move(square, square + Direction::SW));
                }

                if ((::get_file(square) != H_FILE) && 
                    (::get_rank(square) != RANK_1) && 
                    bitboard::get_bit(occ, square + Direction::SE) &&
                    game->at(square + Direction::SE).color() == WHITE) {
                    add_to_movelist(game, moves, Move(square, square + Direction::SE));
                }

                break;

            case Piece::BLACK_KNIGHT:
                attacks = Attacks::knight_attacks(square);
                break;

            case Piece::BLACK_BISHOP:
                attacks = Attacks::bishop_attacks(occ, square);
                break;

            case Piece::BLACK_ROOK:
                attacks = Attacks::rook_attacks(occ, square);
                break;
                
            case Piece::BLACK_QUEEN:
                attacks = Attacks::queen_attacks(occ, square);
                break;

            case Piece::BLACK_KING:
                attacks = Attacks::king_attacks(square);
                break;
        }

        while (attacks) {
            int rshift = __builtin_ctzll(attacks);
            attacks &= attacks - 1;
            Square dest = static_cast<Square>(rshift);
            //Square dest = bitboard::popLSB(attacks); // fix this function, make inline
            if (!bitboard::get_bit(occ, dest) || 
                (game->at(dest).color() != game->side_to_move())) {
                add_to_movelist(game, moves, Move(square, dest));
            }
        }
                
    }

    
    if (moves.size()) {
        // works because of RVO
        return moves;
    } else {
        return {};
    }
}