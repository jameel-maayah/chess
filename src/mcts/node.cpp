#include "node.h"


template <typename GameType>
void Node<GameType>::expand(GameType *game) { // change to reference
    std::vector<Move> moves = generate_moves(game); // abstract this

    if (moves.size()) {
        children.reserve(moves.size());
        num_children = moves.size();

        for (const Move &move : moves) {
            children.push_back(new Node(this, move, ~color));
        }
    } else {
        num_children = 0;
    }
}

template <typename GameType>
Node<GameType> *Node<GameType>::best_uct() const {
    Node *best = children[rand() % num_children];
    double max_score = -INFINITY;

    for (int i = 0; i < num_children; i++) {
        double score = children[i]->puct_score();

        if (score > max_score) {
            max_score = score;
            best = children[i];   
        }
    }
    return best;
}

template <typename GameType>
double Node<GameType>::uct_score() const {
    if (visits == 0) {
        if (!parent->parent) {
            return INFINITY;
        } else {
            // FPU = Q(parent) + exploration term
            return parent->value / parent->visits * ((parent->color == Color::WHITE) ? 1 : -1) + // exploitation
            ((parent) ? CPUCT : ROOT_CPUCT) * sqrt(parent->visits) / (visits + 1);//* prob; // exploration
        }
    }
    return (value / visits) * ((parent->color == Color::WHITE) ? 1 : -1) + // exploitation
        ((parent) ? CPUCT : ROOT_CPUCT) * sqrt(parent->visits) / (visits + 1);// * prob; // exploration
}

template <typename GameType>
double Node<GameType>::puct_score() const {
    if (visits == 0) {
        if (!parent->parent) {
            return INFINITY;
        } else {
            return parent->value / parent->visits * ((parent->color == Color::WHITE) ? 1 : -1) + // exploitation
            ((parent) ? CPUCT : ROOT_CPUCT) * sqrt(parent->visits) / (visits + 1) * prob; // exploration
        }
    }
    return (value / visits) * ((parent->color == Color::WHITE) ? 1 : -1) + // exploitation
        ((parent) ? CPUCT : ROOT_CPUCT) * sqrt(parent->visits) / (visits + 1) * prob; // exploration
}

template <typename GameType>
void Node<GameType>::print_subtree(int depth) const {
    if (!visits) return;
    for (int i = 0; i < depth; i++) printf(i == depth - 1 ? "╰━━━━━>" : "       "); //"⤷"
    std::cout << std::format("{}: Visits={} Value={:.2f} Q={:.2f} Prob={:.6f} Index={} Children={} Color={} P={} V={}", std::string(move), visits, value, Q(), prob, policy_index, num_children, (color == WHITE ? "White" : "Black"), checksum_p, checksum_v) << std::endl;

    for (const Node *child : children) {
        child->print_subtree(depth + 1);
    }
}

template <>
int move_to_index<Chess>(Chess::Move move, bool invert) {
    int from_index, to_index;
    int piece_x = move.source() % 8, piece_y = move.source() / 8,
        target_x = move.target() % 8, target_y = move.target() / 8; 

    if (!invert) {
        from_index = piece_y * 8 + piece_x;
        to_index = target_y * 8 + target_x;
    } else {
        from_index = (7-piece_y) * 8 + piece_x;
        to_index = (7-target_y) * 8 + target_x;
    }

    return (from_index * 64) + to_index;
}


template class Node<Chess>;
