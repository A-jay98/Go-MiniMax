//
// Created by Ali Jamadi on 2019-01-19.
//

#include "MiniMax.h"
#include <iostream>
#include <vector>
#include "Node/Node.h"


using namespace std;


void dumbfunc() {
    cerr << board_size;
}

int player;


MOVE alpha_beta_search(int color) {
    player = color;
    Node *root = new Node(0, 0);
    return Max_value(root, color, NodeInfo(-1), NodeInfo(1)).second;


}


/*MOVE MINIMAX_Decison(int color){
    player = color;
    Node *root = new MaxNode(0, 0);
    vector<Node*> res = root->genChildren(color);


    MOVE move = PASS_MOVE;
    UVAL max = UVAL_MIN;
    cerr<<max<<endl;
    for(Node * node : res){
        trymove(node->move,color,NULL,0);
        if (int a = Min_Value(node, (color == 1) ? 2 : 1) > max){
            max = a;
            move = node->move;
        }
        popgo();
    }


    return move;
}*/

UVAL Min_Value(Node *node, int color, UVAL alpha, UVAL beta) {
    if (node->depth == MAX_DEPTH) return node->utility(player);
    UVAL v = NodeInfo(1);
    int wc = white_captured,bc=black_captured;
    for (Node *a : node->genChildren(color)) {
        trymove(a->move, color, NULL, PASS_MOVE);
        a->whiteLastCap = white_captured - wc;
        a->blackLastCap = black_captured - bc;
        v = min(v, Max_value(a, OTHER_COLOR(color), alpha, beta).first);
        beta = min(beta, v);
        if (beta <= alpha) {
//            cerr<<"cutOff"<<node->depth<<endl;
            popgo();
            break;
        }
        popgo();
    }

    return v;
}

pair<UVAL, MOVE> Max_value(Node *node, int color, UVAL alpha, UVAL beta) {
    if (node->depth == MAX_DEPTH) {
//        cerr<<node->depth<<"  "<<node->move;
        return {node->utility(player), 0};
    }
    UVAL v = NodeInfo(-1);
    MOVE move;
    int wc = white_captured,bc=black_captured;
    for (Node *a : node->genChildren(color)) {
        trymove(a->move, color, NULL, 0);
        a->whiteLastCap = white_captured - wc;
        a->blackLastCap = black_captured - bc;
        UVAL tmp = Min_Value(a, OTHER_COLOR(color), alpha, beta);
        if (tmp > v) {
            v = tmp;
            move = a->move;
        }
        alpha = max(alpha, v);
        if (beta <= alpha) {
//            cerr<<"cutOff D"<<node->depth<<endl;
            popgo();
            break;
        }

        popgo();
    }

    return {v, move};
}










