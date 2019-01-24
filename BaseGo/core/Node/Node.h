//
// Created by Ali Jamadi on 2019-01-23.
//

#ifndef BASEGO_NODE_H
#define BASEGO_NODE_H

#include <vector>
#include "board.h"
#include <iostream>


#define UITILITY_VAL std::pair<int,std::pair<int,int>>
#define UVAL UITILITY_VAL
#define MOVE int

class Node{

public:
    int depth;
    int move;
    int alpha,beta;

    Node(int depth, int move, int alpha, int beta);

    Node(int depth, int move);

    std::vector<Node *> genChildren(int color);


    UVAL utility(int playerC);



private:
    std::pair<int, int> countLib(int color);
    void dfs(int color, int i, int j, int &libCount, bool *mark, int &gpCount);




};

#endif //BASEGO_NODE_H
