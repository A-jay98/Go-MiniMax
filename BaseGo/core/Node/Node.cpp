//
// Created by Ali Jamadi on 2019-01-23.
//

#include <iostream>
#include "MaxNode.h"
#include "MinNode.h"
#include "Node.h"

using namespace std;


Node::Node(int depth, int move) : depth(depth), move(move) {}

vector<Node *> Node::genChildren(int color) {
    vector<Node *> childs;
    int move;
    int minx = 2 ; int maxx =5;
    int miny = 2 ; int maxy = 5;
    for(int i = 0 ; i<board_size;i++) {
        for (int j = 0; j < board_size; j++) {
            if(BOARD(i,j) != 0){
                minx = min(minx,i);
                maxx = max(maxx,i);
                miny = min(miny,j);
                maxy = max(maxy,j);

            }
        }
    }
    for(int i = minx ; i<maxx;i++){
        for(int j = miny ;j<maxy;j++){
            move = POS(i,j);
            if(is_legal(move,color) && !is_suicide(move,color)){
                Node *node;
                if(this->type == MIN_NODE_TYPE)
                    node = new MaxNode(this->depth+1,move);
                else
                    node = new MinNode(this->depth+1,move);

                childs.push_back(node);
            }
        }
    }
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
    if(playerC == 2 && countB-countA == 2) {
        simple_showboard(stderr);

    }
    return countA-countB;

}