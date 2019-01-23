//
// Created by Ali Jamadi on 2019-01-19.
//

#ifndef BASEGO_MINIMAX_H
#define BASEGO_MINIMAX_H

#include <limits>
#include "board.h"
#include "Node/Node.h"
#define MAX_DEPTH 4

#define UVAL_MAX  std::numeric_limits<UVAL>::max()
#define UVAL_MIN  std::numeric_limits<UVAL>::min()




//MOVE MINIMAX_Decison(int color);

std::pair<UVAL, MOVE> Max_value(Node *node, int color, UVAL alpha , UVAL beta);

UVAL Min_Value(Node *node, int color, UVAL alpha, UVAL beta);
MOVE alpha_beta_search(int color);




void dumbfunc();

#endif //BASEGO_MINIMAX_H
