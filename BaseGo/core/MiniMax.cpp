//
// Created by Ali Jamadi on 2019-01-19.
//

#include "MiniMax.h"
#include <iostream>
#include <vector>
#include "Node/Node.h"
#include "Node/MinNode.h"
#include "Node/MaxNode.h"


using namespace std;



void dumbfunc(){
    cerr<<board_size;
}
int player;



MOVE MINIMAX_Decison(int color){
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
}

UVAL Min_Value(Node *node, int color){
    if(node->depth == MAX_DEPTH) return node->utility(player);
    UVAL v = UVAL_MAX;
    for(Node* a : node->genChildren(color)){
        trymove(a->move , color, NULL , 0);
        v = min(v, Max_value(a, (color == 1) ? 2 : 1 ));
        popgo();
    }

    return v;
}

UVAL Max_value(Node *node, int color) {
    if(node->depth == MAX_DEPTH) {
        return node->utility(player);
    }
    UVAL v = UVAL_MIN;
    for(Node* a : node->genChildren(color)){
        trymove(a->move , color, NULL , 0);
        v = max(v, Min_Value(a, (color == 2) ? 1 : 2 ));
        popgo();
    }

    return v;
}










