//
// Created by Ali Jamadi on 2019-01-23.
//

#include <iostream>
#include "Node.h"

using namespace std;


Node::Node(int depth, int move) : depth(depth), move(move) {}

vector<Node *> Node::genChildren(int color) {
    vector<Node *> childs;
    int move;

    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            move = POS(i, j);
            if (is_legal(move, color) && !is_suicide(move, color)) {
                Node *node;
                node = new Node(this->depth + 1, move);
                childs.push_back(node);
            }
        }
    }
    Node *n = new Node(this->depth + 1, PASS_MOVE);
    childs.push_back(n);
    return childs;

}

/*UVAL Node::utility(int playerC) {

    if(playerC == 2 && abs(black_captured - white_captured ) == 2){
        simple_showboard(stderr);
        exit(0);
    }
    return (playerC == 2) ? black_captured - white_captured : white_captured - black_captured;

}*/

UVAL Node::utility(int playerC) {
//    int countA=0,countB = 0;
//    int op = (playerC == 1)? 2: 1;
//    for(int i = 0 ; i<board_size ; i++){
//        for(int j=0;j<board_size;j++){
//            if(BOARD(i,j)==playerC) countA++;
//            else if(BOARD(i,j) == op)countB++;
//
//        }
//    }
//    int res;
//    if(playerC == WHITE){
//        res = black_captured - white_captured;
//    } else
//        res = white_captured - black_captured;

    if (playerC == WHITE) {
        NodeInfo ret;
        pair<int, int> p = countLib(WHITE);
        pair<int, int> q = countgp(WHITE);
        pair<int, int> r = countGrand(WHITE);
        if (r.first == 1000 || r.first == -1000) {
            std::cerr << "we fucked up";
            exit(0);
        }

        ret.mycapture = blackLastCap;
        ret.opcapture = whiteLastCap;

        ret.mylibcnt = p.first;
        ret.oplibcnt = p.second;

        ret.mygp = q.first;
        ret.opgp = q.second;

        ret.myGrand = r.first;
        ret.opGrand = r.second;


        return ret;
    } else {
        NodeInfo ret;
        pair<int, int> p = countLib(BLACK);
        pair<int, int> q = countgp(BLACK);
        pair<int, int> r = countGrand(BLACK);
        ret.mycapture = whiteLastCap;
        ret.opcapture = blackLastCap;

        ret.mylibcnt = p.first;
        ret.oplibcnt = p.second;

        ret.mygp = q.first;
        ret.opgp = q.second;

        ret.myGrand = r.first;
        ret.opGrand = r.second;


        return ret;
    }

}


std::pair<int, int> Node::countgp(int color) {
    pair<int, int> bw = {0, 0};
    bool ismark[84];
    memset(ismark, 0, sizeof(ismark));
    int libMine = 0;
    int libOp = 0;
    int myGp = 0;
    int opGp = 0;
    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            if (board[POS(i, j)] == color and ismark[i * 9 + j] == false) {
                dfs(color, i, j, libMine, ismark, myGp);
                myGp++;
            }
        }
    }

    memset(ismark, 0, sizeof(ismark));
    color = OTHER_COLOR(color);
    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            if (board[POS(i, j)] == color and ismark[i * 9 + j] == false) {
                dfs(color, i, j, libOp, ismark, opGp);
                opGp++;
            }
        }
    }

    bw = {myGp, opGp};
    return bw;
}

std::pair<int, int> Node::countLib(int color) {
    pair<int, int> bw = {0, 0};
    bool ismark[84];
    memset(ismark, 0, sizeof(ismark));
    int libMine = 0;
    int libOp = 0;
    int myGp = 0;
    int opGp = 0;
    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            if (board[POS(i, j)] == color and ismark[i * 9 + j] == false) {
                dfs(color, i, j, libMine, ismark, myGp);

            }
        }
    }

    memset(ismark, 0, sizeof(ismark));
    color = OTHER_COLOR(color);
    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            if (board[POS(i, j)] == color and ismark[i * 9 + j] == false) {
                dfs(color, i, j, libOp, ismark, opGp);
            }
        }
    }

    bw = {libMine, libOp};
    return bw;
}

void Node::dfs(int color, int i, int j, int &libCount, bool *mark, int &gpCount) {
    if (!mark[i * 9 + j]) {
        mark[i * 9 + j] = true;
        if (BOARD(i, j) == EMPTY) {
            libCount++;
            return;
        }
        if (board[NORTH(POS(i, j))] == color || board[NORTH(POS(i, j))] == EMPTY)
            dfs(color, I(NORTH(POS(i, j))), J(NORTH(POS(i, j))), libCount, mark, gpCount);
        if (board[SOUTH(POS(i, j))] == color || board[SOUTH(POS(i, j))] == EMPTY)
            dfs(color, I(SOUTH(POS(i, j))), J(SOUTH(POS(i, j))), libCount, mark, gpCount);
        if (board[WEST(POS(i, j))] == color || board[WEST(POS(i, j))] == EMPTY)
            dfs(color, I(WEST(POS(i, j))), J(WEST(POS(i, j))), libCount, mark, gpCount);
        if (board[EAST(POS(i, j))] == color || board[EAST(POS(i, j))] == EMPTY)
            dfs(color, I(EAST(POS(i, j))), J(EAST(POS(i, j))), libCount, mark, gpCount);

    }

}

int Node::dfs2(int color, int i, int j, bool *mark) {
    mark[i * 9 + j] = true;
    int sz = 1;
    bool ret = false;

    if (board[WEST(POS(i, j))] == OTHER_COLOR(color))
        ret = true;
    if (board[EAST(POS(i, j))] == OTHER_COLOR(color))
        ret = true;
    if (board[NORTH(POS(i, j))] == OTHER_COLOR(color))
        ret = true;
    if (board[SOUTH(POS(i, j))] == OTHER_COLOR(color))
        ret = true;

    if (board[SOUTH(POS(i, j))] == EMPTY and !mark[I(SOUTH(POS(i, j))) * 9 + J(SOUTH(POS(i, j)))])
        sz += dfs2(color, I(SOUTH(POS(i, j))), J(SOUTH(POS(i, j))), mark);
    if (board[NORTH(POS(i, j))] == EMPTY and !mark[I(NORTH(POS(i, j))) * 9 + J(NORTH(POS(i, j)))])
        sz += dfs2(color, I(NORTH(POS(i, j))), J(NORTH(POS(i, j))), mark);
    if (board[EAST(POS(i, j))] == EMPTY and !mark[I(EAST(POS(i, j))) * 9 + J(EAST(POS(i, j)))])
        sz += dfs2(color, I(EAST(POS(i, j))), J(EAST(POS(i, j))), mark);
    if (board[WEST(POS(i, j))] == EMPTY and !mark[I(WEST(POS(i, j))) * 9 + J(WEST(POS(i, j)))])
        sz += dfs2(color, I(WEST(POS(i, j))), J(WEST(POS(i, j))), mark);

    if (ret)
        return -100000;
    else
        return sz;


}


std::pair<int, int> Node::countGrand(int color) {
    int res1 = 0;
    bool mark[85];
    memset(mark, 0, sizeof mark);
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (!mark[i * 9 + j] and board[POS(i, j)] != BLACK and board[POS(i, j)] != WHITE) {
                int tmp = dfs2(color, i, j, mark);
                if (tmp > 0)
                    res1 += tmp;
            }
    int res2 = 0;
    memset(mark, 0, sizeof mark);
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (!mark[i * 9 + j] and board[POS(i, j)] != BLACK and board[POS(i, j)] != WHITE) {
                int tmp = dfs2(OTHER_COLOR(color), i, j, mark);
                if (tmp > 0)
                    res2 += tmp;
            }
    return {res1, res2};


}

