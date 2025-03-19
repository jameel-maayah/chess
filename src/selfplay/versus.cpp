#include <torch/torch.h>
#include <torch/script.h>
#include <ATen/ATen.h>

#include <algorithm>
#include <stdexcept>

#include "chess.h"
#include "types.h"
#include "zobrist.h"

#include "montecarlo.h"
#include "tree.h"

#include "network.h"


using torch::Tensor;

extern ChessNet model;
torch::Device device(torch::kCPU);
constexpr torch::ScalarType precision = torch::kFloat16;


// Move these somewhere else
template<typename GameType>
Tensor to_tensor(const GameType *game) {
    throw std::logic_error("to_tensor: Unsupported GameType");
}

template<>
Tensor to_tensor<Chess>(const Chess *game) {
    static Piece::Type pieces[13] = {
        Piece::WHITE_KING, Piece::WHITE_QUEEN, Piece::WHITE_ROOK, Piece::WHITE_BISHOP,
        Piece::WHITE_KNIGHT, Piece::WHITE_PAWN, Piece::EMPTY, Piece::BLACK_PAWN, Piece::BLACK_KNIGHT,
        Piece::BLACK_BISHOP, Piece::BLACK_ROOK, Piece::BLACK_QUEEN, Piece::BLACK_KING
    };    

    Tensor state_tensor = torch::zeros({13, 8, 8}, torch::TensorOptions().dtype(precision).device(torch::kCPU));
    auto accessor = state_tensor.accessor<at::Half, 3>();

    if (game->side_to_move() == Chess::Color::WHITE) {
        for (int piece = 0; piece < 13; piece++) {
            U64 bb = game->piece_mask(pieces[piece]);
            int ix = 0;

            while (bb) {
                int rshift = __builtin_ctzll(bb);
                bb >>= rshift;
                ix += rshift;

                accessor[piece][7 - ix/8][ix%8] = 1.0f;
                bb &= bb - 1;
            }
        }
        
    } else {
        for (int piece = 0; piece < 13; piece++) {
            U64 bb = game->piece_mask(pieces[piece]);
            int ix = 0;

            while (bb) {
                int rshift = __builtin_ctzll(bb);
                bb >>= rshift;
                ix += rshift;

                accessor[12-piece][ix/8][ix%8] = 1.0f;
                bb &= bb - 1;
            }
        }
    }
    //state_tensor[6].fill_(0); // Unused plane; will eventually use it for castling and en passant
    return state_tensor;
}


template <typename GameType> 
void MCTsearch(GameType *game, MCTree<GameType> *tree, int iterations) {
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; i++) {
        Node<GameType> *leaf = tree->traverse();

        if (!leaf->is_terminal()) { //leaf->is_ready
            game->bitboard[Piece::EMPTY] = ~(game->occupied_mask());
            Tensor state_tensor = to_tensor<Chess>(game).unsqueeze(0);
            auto [policy_tensor, value_tensor] = model->forward(state_tensor.to(device));

            value_tensor = torch::softmax(value_tensor, -1);      
            policy_tensor = policy_tensor.to(torch::kCPU);
            value_tensor = value_tensor.to(torch::kCPU); 
            
            /*
            torch::TensorAccessor policy_accessor;// = policy_tensor.accessor<float, 2>();
            torch::TensorAccessor value_accessor;// = value_tensor.accessor<float, 2>();
        
            if constexpr (precision == torch::kFloat16) {
                policy_accessor = policy_tensor.accessor<at::Half, 2>();
                value_accessor = value_tensor.accessor<at::Half, 2>();
            } else if constexpr (precision == torch::kFloat32) {
                policy_accessor = policy_tensor.accessor<float, 2>();
                value_accessor = value_tensor.accessor<float, 2>();
            }
            */

            auto policy_accessor = policy_tensor.accessor<at::Half, 2>();
            auto value_accessor = value_tensor.accessor<at::Half, 2>();
            
            float sum_logits = 0;
            float pst = 1;//(leaf->parent ? PST : ROOT_PST);

            for (int c = 0; c < leaf->num_children; c++) {
                Node<GameType> *child = leaf->children[c];
                float prob = exp(policy_accessor[0][child->policy_index] / pst);

                child->prob = prob;
                sum_logits += prob;
            }

            for (int c = 0; c < leaf->num_children; c++) {
                leaf->children[c]->prob /= sum_logits;
            }

            // Value = pWin - pLose
            leaf->value = (value_accessor[0][0] - value_accessor[0][2]) * ((game->side_to_move() == WHITE) ? 1 : -1); // WDL evaluation
            //leaf->value = (value_accessor[0][0]) * ((game->side_to_move() == WHITE) ? 1 : -1); // Centipawn evaluation
            leaf->is_ready = true;

            tree->backpropagate(leaf, leaf->value);
        } else {
            switch (leaf->terminal_status) {
                case WIN_WHITE:
                    tree->backpropagate(leaf, 1.0f);
                    break;
                case WIN_BLACK: 
                    tree->backpropagate(leaf, -1.0f);
                    break;
                default:
                    tree->backpropagate(leaf, 0.0f);
                    break;

                // Value of a draw is the contempt factor
            }
        }
        
    }
    auto end = std::chrono::high_resolution_clock::now();
    printf("%d iterations in %ld\n", iterations, std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
}


Square file_rank_to_square(int file, int rank) {
    return static_cast<Square>(rank * 8 + file);
}



int main() {
    Zobrist::init_zobrist();

    Chess game = Chess(START_FEN);
    torch::load(model,  "../weights/pv_6b96_epoch_14.pt");  //pv_6b96_epoch_14.pt

    if (torch::mps::is_available()) {
        device = torch::Device(torch::kMPS);
        std::cout << "MPS backend is available. Using MPS device." << std::endl;
    } else if (torch::cuda::is_available()) {
        device = torch::Device(torch::kCUDA);
        std::cout << "CUDA backend is available. Using CUDA device." << std::endl;
    } else {
        device = torch::Device(torch::kCPU);
        std::cout << "No GPU backend available. Using CPU device." << std::endl;
    }

    model->to(device);
    model->to(precision);
    model->eval();

    std::cout << count_parameters(model) << " Trainable parameters" << std::endl;

    /*
    game.make_move(Move(e2, e4));
    game.make_move(Move(e7, e5));
    game.make_move(Move(d1, f3));
    game.make_move(Move(a7, a6));
    game.make_move(Move(f1, c4));
    //game.make_move(Move(a6, a5));
    */

    MCTree<Chess> tree(game);
    MCTsearch<Chess>(&game, &tree, 801);

    tree.print();
    game.fancy_print();
    Move predicted_move = (tree.best_child()) ? tree.best_child()->move : Move();
    
    while (!game.get_status()) {
        Move move;
        if (game.side_to_move() == WHITE) {
            int from_y, to_y;
            int from_x, to_x;
            char promo;

            std::string movestr;
            std::cin >> movestr;

            from_x = movestr[0] - 'a';
            to_x = movestr[2] - 'a';
            from_y = movestr[1] - '0' - 1;
            to_y = movestr[3] - '0' - 1;
            if (movestr.length() > 4) promo = movestr[4];

            printf("%d %d %d %d\n", from_x, to_x, from_y, to_y);

            move = Move(file_rank_to_square(from_x, from_y), file_rank_to_square(to_x, to_y));

            switch (promo) {
                case 'n':
                    move.set_flag(Move::Flag::PROMOTION_KNIGHT);
                    break;
                case 'b':
                    move.set_flag(Move::Flag::PROMOTION_BISHOP);
                    break;
                case 'r':
                    move.set_flag(Move::Flag::PROMOTION_ROOK);
                    break;
                case 'q':
                    move.set_flag(Move::Flag::PROMOTION_QUEEN);
                    break;
                default:
                    break;
            }

            std::cout << "Predicted: " << (char) ('a' + (predicted_move.source() % 8)) << (predicted_move.source() / 8) + 1 << (char) ('a' + (predicted_move.target() % 8)) << (predicted_move.target() / 8) + 1 << std::endl;
        } else {
            MCTsearch<Chess>(&game, &tree, 801);
            tree.print();
            move = tree.best_child()->move;
        }

        std::cout << "Principal Variation: " << (char) ('a' + (move.source() % 8)) << (move.source() / 8) + 1 << (char) ('a' + (move.target() % 8)) << (move.target() / 8) + 1 << std::endl;
        std::cout << "Eval: " << tree.get_root()->value / tree.get_root()->visits << std::endl;

        double brilliance_factor = tree.best_child()->visits / tree.best_child()->prob; std::cout << brilliance_factor << std::endl;
        if (brilliance_factor > 10000) {
            std::cout << "The model thinks this is a brilliant move (brilliance factor " << brilliance_factor << ")\n";
        }

        game.make_move(move);
        tree.update_root(move);

        predicted_move = (tree.best_child()) ? tree.best_child()->move : Move();
        game.fancy_print(game.side_to_move());
        game.print();
    }
}