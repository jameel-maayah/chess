#pragma once

#include "node.h"

template <typename GameType>
class MCTree {
public:
    using Move = typename GameType::Move;
    using Node = Node<GameType>;

private:
    std::unique_ptr<Node> root;
    GameType game;

public:
    MCTree(const GameType& game);
    ~MCTree();

    Node* get_root() const { return root.get(); }
    Node* traverse(Node *node);
    void backpropagate(Node* leaf, double value);
    void make_move(const Move move);

    void print();
};
