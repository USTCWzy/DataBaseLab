//
// Created by wzy1999 on 2020/12/8.
//

#ifndef PROJECT_BUFFERMANAGER_H
#define PROJECT_BUFFERMANAGER_H

#include "BCB.h"
#include "LRU.h"
#include "StorageStructure.h"
#include "DataStorageManager.h"


class BMgr {

public:
    BMgr();
    ~BMgr();
// Interface functions
    int FixPage(int page_id, int prot);
    //void NewPage();
    NewPage FixNewPage();
    int UnfixPage(int page_id);
    int NumFreeFrames();
// Internal Functions
    int SelectVictim();
    int Hash(int page_id);
    int FindPage(int page_id);
    void RemoveBCB(BCB * ptr, int page_id);
    void RemoveLRUEle(int frid);
    void SetDirty(int frame_id);
    void UnsetDirty(int frame_id);
    void WriteDirtys();
    void PrintFrame(int frame_id);

    bFrame buf[DEFBUFSIZE];
    DSMgr *DataFile;
    LRU *lru;
private:
// Hash Table
    int ftop[DEFBUFSIZE];
    BCB* ptof[DEFBUFSIZE];

    int numPages;
};


#endif //PROJECT_BUFFERMANAGER_H
