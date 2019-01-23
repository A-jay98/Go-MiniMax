//
// Created by Ali Jamadi on 2019-01-19.
//

#ifndef BASEGO_MINIMAX_H
#define BASEGO_MINIMAX_H

#include <limits>
#include "board.h"
#include "Node/Node.h"
#define MAX_DEPTH 5
//FIXME : changeit later to inf
#define UVAL_MAX  {10000,{10000,-10000}}
#define UVAL_MIN  {-10000,{-10000,10000}}




//MOVE MINIMAX_Decison(int color);

std::pair<UVAL, MOVE> Max_value(Node *node, int color, UVAL alpha , UVAL beta);

UVAL Min_Value(Node *node, int color, UVAL alpha, UVAL beta);
MOVE alpha_beta_search(int color);




void dumbfunc();

#endif //BASEGO_MINIMAX_H
