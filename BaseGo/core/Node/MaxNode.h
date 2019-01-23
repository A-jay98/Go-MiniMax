//
// Created by Ali Jamadi on 2019-01-23.
//

#ifndef BASEGO_MAXNODE_H
#define BASEGO_MAXNODE_H

#include "Node.h"


class MaxNode:public Node {

public:
    MaxNode(int depth, int move);

    UVAL value();

};


#endif //BASEGO_MAXNODE_H
