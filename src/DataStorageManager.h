//
// Created by wzy1999 on 2020/12/8.
//

#ifndef PROJECT_DATASTORAGEMANAGER_H
#define PROJECT_DATASTORAGEMANAGER_H                                          4r

#include "BCB.h"
#include "StorageStructure.h"
#include <iostream>
#include <cstring>
#include <random>


class DSMgr {

public:
    DSMgr();
    ~DSMgr();
    int OpenFile(std::string filename);
    int CloseFile();
    void InitializeRandom();
    bFrame ReadPage(int page_id);
    int WritePage(int frame_id, bFrame frm);
    int Seek(int offset, int pos);
    int GetNewPage();
    FILE * GetFile();
    void IncNumPages();
    int GetNumPages();
    void SetUse(int index, int use_bit);
    int GetUse(int index);

private:
    FILE *currFile;
    int numPages;
    int pages[MAXPAGES];
    int index[MAXPAGES];
};


#endif //PROJECT_DATASTORAGEMANAGER_H
