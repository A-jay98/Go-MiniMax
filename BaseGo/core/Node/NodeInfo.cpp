//
// Created by Ali Jamadi on 2019-01-26.
//

#include "NodeInfo.h"
#include <iostream>
#include "board.h"

double NodeInfo::value() const {
    double ret = 0;
    ret = 10 * mycapture - 4 * opcapture;
    ret += 3 * mylibcnt - 4 * oplibcnt;
    ret += -9 * mygp + 2 * opgp;
    ret += 8*myGrand;
//    ret =   mycapture -   opcapture;
//    ret +=  mylibcnt -   oplibcnt;
//    ret +=  -mygp +  opgp;
//    ret += myGrand - opGrand;

    return ret;
}

NodeInfo::NodeInfo(int x) {
    if (x == -1) {
        mycapture = mylibcnt = opgp = myGrand = -1000;
        opcapture = oplibcnt = mygp = opGrand = 1000;
    } else {
        mycapture = mylibcnt = opgp = myGrand = 1000;
        opcapture = oplibcnt = mygp = opGrand = -1000;
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



