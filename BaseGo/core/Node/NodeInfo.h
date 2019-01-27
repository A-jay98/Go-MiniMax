//
// Created by Ali Jamadi on 2019-01-26.
//

#ifndef BASEGO_NODEINFO_H
#define BASEGO_NODEINFO_H


struct NodeInfo {
    int mycapture;
    int opcapture;
    int mylibcnt;
    int oplibcnt;
    int mygp;
    int opgp;
    int myeyecnt;
    int opeyecnt;
    int myGrand;
    int opGrand;
    int myCount,opCount;
    int myEdge ,opEdge;

    double value() const;

    NodeInfo(int x);

    NodeInfo() {

    }

    friend bool operator==(const NodeInfo &lhs, const NodeInfo &rhs);

    friend bool operator<(const NodeInfo &lhs, const NodeInfo &rhs);

    friend bool operator<=(const NodeInfo &lhs, const NodeInfo &rhs);

    friend bool operator>(const NodeInfo &lhs, const NodeInfo &rhs);

};


#endif //BASEGO_NODEINFO_H
