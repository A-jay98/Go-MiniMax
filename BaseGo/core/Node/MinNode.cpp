//
// Created by Ali Jamadi on 2019-01-23.
//

#include "MinNode.h"


MinNode::MinNode(int depth, int move) : Node(depth, move) {type = MIN_NODE_TYPE;}

UVAL MinNode::value() {
    return 0;
}
