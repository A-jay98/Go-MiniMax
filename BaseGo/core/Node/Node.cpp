//
// Created by Ali Jamadi on 2019-01-23.
//

#include <iostream>
#include "Node.h"

using namespace std;


Node::Node(int depth, int move) : depth(depth), move(move) {}

vector<Node *> Node::genChildren(int color) {
    vector<Node *> childs;
    int move;
    int minx = 0 ; int maxx =board_size;
    int miny = 0 ; int maxy = board_size;
    for(int i = minx ; i<maxx;i++){
        for(int j = miny ;j<maxy;j++){
            move = POS(i,j);
            if(is_legal(move,color) && !is_suicide(move,color)){
                Node *node;
                node = new Node(this->depth+1,move);
                childs.push_back(node);
            }
        }
    }
    Node *n = new Node(this->depth+1,PASS_MOVE);
    childs.push_back(n);
    return childs;

}

/*UVAL Node::utility(int playerC) {

    if(playerC == 2 && abs(black_captured - white_captured ) == 2){
        simple_showboard(stderr);
        exit(0);
    }
    return (playerC == 2) ? black_captured - white_captured : white_captured - black_captured;

}*/

UVAL Node::utility(int playerC) {
    int countA=0,countB = 0;
    int op = (playerC == 1)? 2: 1;
    for(int i = 0 ; i<board_size ; i++){
        for(int j=0;j<board_size;j++){
            if(BOARD(i,j)==playerC) countA++;
            else if(BOARD(i,j) == op)countB++;

        }
    }

    return {countA-countB,countLib(playerC)};

}

Node::Node(int depth, int move, int alpha, int beta) : depth(depth), move(move), alpha(alpha), beta(beta) {}

std::pair<int, int> Node::countLib(int color) {
    pair<int, int> bw = {0,0};
    bool ismark[84]={false};
    int libMine = 0;
    int libOp = 0;
    for(int i = 0 ; i<board_size ; i++) {
        for (int j = 0; j < board_size; j++) {
            if(board[POS(i,j)] == color) dfs(color, i, j, libMine, ismark);
        }
    }

    memset(ismark,0, sizeof(ismark));
    color = OTHER_COLOR(color);
    for(int i = 0 ; i<board_size ; i++) {
        for (int j = 0; j < board_size; j++) {
            if(board[POS(i,j)] == color) dfs(color, i, j, libMine, ismark);
        }
    }

    bw = {libMine, -libOp};
    return bw;
}

void Node::dfs(int color, int i, int j, int &libCount, bool *mark) {
    if(!mark[i*80+j]){
        mark[i*80+j] = true;
         if(BOARD(i,j)== EMPTY){
            libCount++;
             return;
        }
        if(board[NORTH(POS(i,j))] == color || board[NORTH(POS(i,j))] == EMPTY) dfs(color, I(NORTH(POS(i, j))), J(NORTH(POS(i, j))), libCount, mark);
        if(board[SOUTH(POS(i,j))] == color || board[SOUTH(POS(i,j))] == EMPTY) dfs(color, I(SOUTH(POS(i, j))), J(SOUTH(POS(i, j))), libCount, mark);
        if(board[WEST(POS(i,j))] == color  || board[WEST(POS(i,j))] == EMPTY) dfs(color, I(WEST(POS(i, j))), J(WEST(POS(i, j))), libCount, mark);
        if(board[EAST(POS(i,j))] == color  || board[EAST(POS(i,j))] == EMPTY) dfs(color, I(EAST(POS(i, j))), J(EAST(POS(i, j))), libCount, mark);

    }

}

