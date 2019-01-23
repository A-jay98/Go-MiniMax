//
// Created by Ali Jamadi on 2019-01-23.
//

#ifndef BASEGO_MINNODE_H
#define BASEGO_MINNODE_H

#include "Node.h"


class MinNode:public Node {
public:
    MinNode(int depth, int move);

    UVAL value();
};

#endif //BASEGO_MINNODE_H
