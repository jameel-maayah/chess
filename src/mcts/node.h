#pragma once

#include <memory>
#include <vector>
#include <string>
#include <format>
#include <iostream>

#include "chess.h"
#include "movegen.h"

#define ROOT_PST 1.
#define PST 1.1
#define ROOT_CPUCT 4
#define CPUCT 3


// Move this somewhere else
template <typename GameType>
int move_to_index(typename GameType::Move move, bool invert);


//#pragma pack(1)
template <typename GameType>
class Node {
public:
    using Move = typename GameType::Move;
    using Color = typename GameType::Color;
    using Status = typename GameType::Status;

    Node(Node* parent, Move move, Color color)
        : visits(0),
          value(0.0f),
          move(move),
          policy_index(move_to_index<GameType>(move, color==Color::WHITE)),
          is_ready(false),
          terminal_status(IN_PROGRESS),
          color(color),
          num_children(-1),
          parent(parent) {}

    ~Node() {
        for (auto *child : children) {
            delete child;
        }
    }

    void expand(GameType *game);
    inline bool fully_expanded() const { return num_children != -1; }
    inline bool is_terminal() const { return terminal_status; }
    inline void set_terminal(Status status) { terminal_status = status; }

    Node *best_uct() const;

    // move these to score.h ?
    double uct_score() const;
    double puct_score() const;


    void print_subtree(int depth) const;
    
    
    inline int get_visits() const { return visits; }
    inline double get_value() const { return value; }
    inline double Q() const { return value / visits; }
    //inline double get_prob() const { return prob; }
    inline Move get_move() const { return move; }
    //bool get_is_ready() const { return is_ready; }
    //bool get_is_white() const { return is_white; }

    void set_value(double val) { value = val; }
    //void set_prob(double prob_val) { prob = prob_val; }
    //void set_ready(bool ready) { is_ready = ready; }

    // make private?
//private:

    int visits;
    double value;

    Move move;
    Color color;

    double prob;
    int policy_index;

    bool is_ready;
    Status terminal_status;

    Node *parent;
    std::vector<Node *> children;
    int num_children;
};