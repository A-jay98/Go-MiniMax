//
// Created by Ali Jamadi on 2019-01-23.
//

#ifndef BASEGO_NODE_H
#define BASEGO_NODE_H

#include <vector>
#include "board.h"

#define MIN_NODE_TYPE 0
#define MAX_NODE_TYPE 1

#define UITILITY_VAL int
#define UVAL UITILITY_VAL
#define MOVE int

class Node{

public:
    int depth;
    int move;
    int type;
    int alpha,beta;

    Node(int depth, int move, int alpha, int beta);

    Node(int depth, int move);

    std::vector<Node *> genChildren(int color);

    virtual UVAL value() = 0;

    UVAL utility(int playerC);

private:




};

#endif //BASEGO_NODE_H
