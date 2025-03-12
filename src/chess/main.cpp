#include <iostream>
#include <string>
#include <vector>

#include "chess.h"
#include "attacks.h"
#include "bitboard.h"
#include "move.h"
#include "movegen.h"



int main() {
    //std::cout << Move(e2, e4).source() << " " << Move(e2, e4).target() << "\n"; print_bitboard(0x8181818181818181ULL);
    const Move variation[7] = { Move(e2, e4), Move(e7, e5), Move(d1, f3), Move(a7, a5), Move(f1, c4), Move(a5, a4), Move(f3, f7) };

    std::string_view fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    Chess game(fen);
    game.fancy_print();

    /*
    while (!game.get_status()) { 
        std::vector<Move> moves = generate_moves(&game);
        game.make_move(moves[rand() % moves.size()]);
        game.fancy_print();
    }
    */

    /* 
    game.make_move(Move(f3, f4)); 
    game.make_move(Move(c7, c6));
    game.make_move(Move(f4, f5));
    game.print(); 
    std::cout << game.is_attacked(game.white_king); exit(1);
    */

    //std::vector<Move> moves = generate_moves(&game);
    
    //std::cout << moves.size() << " moves\n";

    //for (const Move &m: moves) {
    //    std::cout << std::string(m) << std::endl;
    //}
    
    std::cout << "perft\n";
    /*
    std::cout << perft(&game, 1) << std::endl;
    std::cout << perft(&game, 2) << std::endl;
    std::cout << perft(&game, 3) << std::endl;
    std::cout << perft(&game, 4) << std::endl; 
    std::cout << perft(&game, 5) << std::endl;
    */
    
    /*
    std::cout << detail_perft(&game, 1) << std::endl;
    std::cout << detail_perft(&game, 2) << std::endl;
    std::cout << detail_perft(&game, 3) << std::endl;
    std::cout << detail_perft(&game, 4) << std::endl; 
    std::cout << detail_perft(&game, 5) << std::endl;
    */
    for (int i = 0; i < 12; i++) {
        std::cout << detail_perft(&game, i) << std::endl;
    }
    
    

    //std::cout << perft(&game, 3) << std::endl;

    return 0;

    /*

    std::vector<Move> moves = generate_moves(&game);

    std::cout << moves.size() << " moves\n";

    for (const Move &m: moves) {
        std::cout << std::string(m) << std::endl;
    }

    std::cout << perft(&game, 2) << std::endl;

    return 0;
    
    */

    //for (int i = 0; i < 100; i++) {
    while (!game.get_status()) {
        /*
        print_bitboard(game.occupied());
        for (const U64 &bb: game.bitboard) {
            print_bitboard(bb);
        }
        */

        std::vector<Move> moves = generate_moves(&game);

        std::cout << moves.size() << " moves\n";

        for (const Move &m: moves) {
            std::cout << std::string(m) << std::endl;
        }

        std::cout << "making move " << std::string(variation[game.get_ply()]) <<std::endl;
        game.make_move(variation[game.get_ply()]);//moves[rand() % moves.size()]);
        game.print();
    }    
    std::cout << game.get_status(); game.print();

    for (int i = 0; i < 7; i++) {
        game.undo_move();
        game.print();
    }
    
    U64 occ = 0ULL;

    occ = 0xFF000000000000FFULL;
    
    //bitboard::set_bit(occ, b3);
    //bitboard::set_bit(occ, f6);
    //bitboard::set_bit(occ, e4);
    //bitboard::set_bit(occ, d4);
    //bitboard::set_bit(occ, c5);
    
    //occ = 0ULL;

    //for (Square sq = a1; sq <= a8; ++sq) {
        //std::cout << sq << std::endl;

        //bitboard::set_bit(occ, sq);
        //print_bitboard(Attacks::bishop_attacks(occ, sq));
        //bitboard::clear_bit(occ, sq);
        //print_bitboard(DIAG_MASK[static_cast<int>(sq)] | ANTI_DIAG_MASK[static_cast<int>(sq)]);
        //print_bitboard(FILE_MASK[static_cast<int>(sq)] | RANK_MASK[static_cast<int>(sq)]);
        //print_bitboard(DIAG_MASK[static_cast<int>(sq)]);
        //BIT_MASK[static_cast<int>(sq)]);
    //}
    

    return 0;
}
