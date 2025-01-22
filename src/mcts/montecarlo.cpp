#include "montecarlo.h"
#include "chess.h"
#include "types.h"
#include "tree.h"

int count_material(const Chess &game) {
    int sum = 0;
    for (Square sq = a1; sq <= h8; ++sq) {
        switch (game.at(sq).type()) {
            case Piece::WHITE_PAWN:
                sum += 1;
                break;
            case Piece::WHITE_KNIGHT:
                sum += 3;
                break;
            case Piece::WHITE_BISHOP:
                sum += 3;
                break;
            case Piece::WHITE_ROOK:
                sum += 5;
                break;
            case Piece::WHITE_QUEEN:
                sum += 9;
                break;
            case Piece::WHITE_KING:
                break;
            case Piece::BLACK_PAWN:
                sum -= 1;
                break;
            case Piece::BLACK_KNIGHT:
                sum -= 3;
                break;
            case Piece::BLACK_BISHOP:
                sum -= 3;
                break;
            case Piece::BLACK_ROOK:
                sum -= 5;
                break;
            case Piece::BLACK_QUEEN:
                sum -= 9;
                break;
            case Piece::BLACK_KING:
                break;
            case Piece::EMPTY:
                break;
        }
    }
    return sum;
}


int main() {
    Chess game = Chess(START_FEN);

    game.make_move(Move(e2, e4));
    game.make_move(Move(e7, e5));
    game.make_move(Move(d1, f3));
    game.make_move(Move(a7, a5));
    game.make_move(Move(f1, c4));
    game.make_move(Move(a5, a4));
    //game.make_move(Move(a2, a3));

    game.print();

    MCTree<Chess> tree(game);

    //tree.traverse(tree.get_root());
    for (int i = 0; i < 8000; i++) {
        tree.backpropagate(tree.traverse(tree.get_root()), (double) count_material(game) / 200);
    }

    tree.print();

}
