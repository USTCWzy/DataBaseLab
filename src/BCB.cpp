//
// Created by wzy1999 on 2020/12/8.
//

#include "BCB.h"


BCB::BCB() {
    page_id = 0;
    frame_id = 0;
    latch = 0;
    count = 0;
    dirty = 0;
    next = nullptr;
}

BCB::BCB(int pid, int fid, int lat, int cou, int dit) {
    page_id = pid;
    frame_id = fid;
    latch = lat;
    count = cou;
    dirty = dit;
    next = nullptr;
}