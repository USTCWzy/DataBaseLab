//
// Created by wzy1999 on 2020/12/8.
//

#ifndef PROJECT_STORAGESTRUCTURE_H
#define PROJECT_STORAGESTRUCTURE_H

#define FRAMESIZE 4096
#define DEFBUFSIZE 1024
#define MAXPAGES 50000


typedef struct bFrame{
    char field[FRAMESIZE];
}bFrame;

typedef struct NewPage{
    int page_id;
    int frame_id;
    NewPage(int pid, int fid):page_id(pid), frame_id(fid){};
}NewPage;

#endif //PROJECT_STORAGESTRUCTURE_H
