//
// Created by wzy1999 on 2020/12/8.
//

#ifndef PROJECT_BCB_H
#define PROJECT_BCB_H


class BCB {

public:
    BCB();
    BCB(int pid, int fid, int lat = 0, int cou = 0, int dit = 0);
    int page_id;
    int frame_id;
    int latch;
    int count;
    int dirty;
    BCB * next;
};


#endif //PROJECT_BCB_H
