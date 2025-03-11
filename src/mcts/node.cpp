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
        double score = children[i]->uct_score();

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
            return parent->value / parent->visits * ((color == WHITE) ? 1 : -1) + // exploitation
            ((parent) ? CPUCT : ROOT_CPUCT) * sqrt(parent->visits) / (visits + 1); // exploration
        }
    }
    return (value / visits) * ((color == WHITE) ? 1 : -1) + // exploitation
        ((parent) ? CPUCT : ROOT_CPUCT) * sqrt(parent->visits) / (visits + 1); // exploration
}

template <typename GameType>
double Node<GameType>::puct_score() const {
    if (visits == 0) {
        if (!parent->parent) {
            return INFINITY;
        } else {
            return parent->value / parent->visits * ((color == WHITE) ? 1 : -1) + // exploitation
            ((parent) ? CPUCT : ROOT_CPUCT) * sqrt(parent->visits) / (visits + 1) * prob; // exploration
        }
    }
    return (value / visits) * ((color == WHITE) ? 1 : -1) + // exploitation
        ((parent) ? CPUCT : ROOT_CPUCT) * sqrt(parent->visits) / (visits + 1) * prob; // exploration
}

template <typename GameType>
void Node<GameType>::print_subtree(int depth) const {
    if (!visits) return;
    for (int i = 0; i < depth; i++) printf(i == depth - 1 ? "╰━━━━━>" : "       "); //"⤷"
    std::cout << std::format("{}: Visits={} Value={:.2f} Q={:.2f} Children={} Color={}", std::string(move), visits, value, Q(), num_children, (color == WHITE ? "White" : "Black")) << std::endl;

    for (const Node *child : children) {
        child->print_subtree(depth + 1);
    }
}

template class Node<Chess>;
