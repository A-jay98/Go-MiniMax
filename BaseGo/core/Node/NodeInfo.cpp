//
// Created by Ali Jamadi on 2019-01-26.
//

#include "NodeInfo.h"
#include <iostream>
#include "board.h"

double NodeInfo::value() const {
    double ret = 0;
//    ret = 10 * mycapture - 4 * opcapture;
//    ret += 6 * mylibcnt - 4 * oplibcnt;
//    ret += -25 * mygp + 2 * opgp;
//    ret += 8*myGrand - 5*opGrand;
//    ret =   mycapture -   opcapture;
//    ret +=  mylibcnt -   oplibcnt;
//    ret +=  -mygp +  opgp;
//    ret += myGrand - opGrand;


    if (mycapture > 0)
        ret += 10 * mycapture * mycapture;
    if (opcapture > 2) {
        ret -= opcapture * opcapture * 10;
    }
    if (mycapture == 0 && opcapture == 0) {
        if (mygp == 1 ) {
            ret += 100 * mylibcnt;
            ret -= 10 * oplibcnt;
        } else if (mygp == 2 ) {
            ret += 55 * mylibcnt / 2.0;
            ret -= 5*oplibcnt;
        } else if (mygp == 3 ) {
            ret += 20 * mylibcnt / 2.0;
            ret -= 20 * oplibcnt;
        } else if (mygp < 4) {
            ret += 70 * mylibcnt;
            ret -= oplibcnt * 40;
        } else {
            ret -= oplibcnt;
            ret += 20 * mylibcnt;
            ret -= 10 * mygp;
        }
    }

    ret += myCount*100;
    ret -= opCount*100;
    ret += myGrand*3;
    ret -= opGrand*2;
    ret += myEdge * 30;
    ret -= opEdge * 3;


    return ret;
}

NodeInfo::NodeInfo(int x) {

    if (x == -1) {

        mycapture = mylibcnt = opgp = myGrand = myCount = myEdge = -1000;
        opcapture = oplibcnt = mygp = opGrand = opCount = opEdge = 1000;
    } else {
        mycapture = mylibcnt = opgp = myGrand = myCount = myEdge = 1000;
        opcapture = oplibcnt = mygp = opGrand = opCount = opEdge = -1000;
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


