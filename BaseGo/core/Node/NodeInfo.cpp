//
// Created by Ali Jamadi on 2019-01-26.
//

#include "NodeInfo.h"
#include <iostream>
#include "board.h"

double NodeInfo::value() const {
    double ret = 0;
    ret = 10 * mycapture - 4 * opcapture;
    ret += 1.5 * mylibcnt - oplibcnt;
    ret += -4 * mygp + 2 * opgp;

    ret += myeyecnt - opeyecnt;
    return ret;
}

NodeInfo::NodeInfo(int x) {
    if (x == -1) {
        mycapture = mylibcnt = mygp = myeyecnt = -1000;
        opcapture = oplibcnt = opgp = opeyecnt = 1000;
    } else {
        mycapture = mylibcnt = mygp = myeyecnt = 1000;
        opcapture = oplibcnt = opgp = opeyecnt = -1000;
    }

}


bool operator<(const NodeInfo &a, const NodeInfo &b) {
    return a.value() < b.value();
}

bool operator==(const NodeInfo &a, const NodeInfo &b) {
    return a.value() == b.value();
}

bool operator<=(const NodeInfo &a, const NodeInfo &b) {
    return a.value() <= b.value();
}

bool operator>(const NodeInfo &a, const NodeInfo &b) {
    return a.value() > b.value();
}



