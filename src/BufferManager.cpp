//
// Created by wzy1999 on 2020/12/8.
//

#include "BufferManager.h"

BMgr::BMgr() {

    //std::cout << "hello bmgr()" << std::endl;

    for (int i = 0; i < DEFBUFSIZE; i++){
        ftop[i] = -1;
        ptof[i] = nullptr;
    }

    DataFile = new DSMgr();
    DataFile->OpenFile("DataSet//data.dbf");

    lru = new LRU();

    numPages = DEFBUFSIZE;
}

BMgr::~BMgr(){
    WriteDirtys();

    for (int i = 0; i < DEFBUFSIZE; i++){
        if (ftop[i] != -1){
            delete ptof[i];
        }
    }

    delete lru;
    delete DataFile;
}

int
BMgr::FixPage(int page_id, int prot) {

    int new_frame_id, frame_id = Hash(page_id);
    BCB *bPtr = ptof[frame_id];
    if (bPtr == nullptr ){
        if (ftop[frame_id] != -1) {
            new_frame_id = SelectVictim();
        } else{
            new_frame_id = frame_id;
        }
        buf[new_frame_id] = DataFile->ReadPage(page_id);
        ftop[new_frame_id] = page_id;
        BCB *bNode = new BCB(page_id, new_frame_id, 0, 0, 0);
        ptof[frame_id] = bNode;
        lru->InsertRear(page_id, new_frame_id, 0);
        if (numPages > 0)
            numPages--;
        return new_frame_id;

    }
    else{
        while(bPtr->next != nullptr && bPtr->page_id != page_id){
            bPtr = bPtr->next;
        }
        if (bPtr->next == nullptr && bPtr->page_id != page_id){
            frame_id = SelectVictim();
            buf[frame_id] = DataFile->ReadPage(page_id);
            ftop[frame_id] = page_id;
            BCB *bNode = new BCB(page_id, frame_id, 0, 0, 0);
            bPtr->next = bNode;
            lru->InsertRear(page_id, frame_id, 0);
            return frame_id;
        }
        else{
            lru->InsertRear(page_id, bPtr->frame_id, 1);
            //bPtr->latch = 1;
            //bPtr->count++;
            return bPtr->frame_id;
        }
    }
}

NewPage
BMgr::FixNewPage() {
    int page_id = DataFile->GetNewPage();
    int frame_id = Hash(page_id);
    if (ftop[frame_id] != -1) {
        frame_id = SelectVictim();
    }
    NewPage data(page_id, frame_id);
    return data;
}

int
BMgr::UnfixPage(int page_id) {
    int frame_id = Hash(page_id);
    BCB *bPtr = ptof[frame_id];
    while(bPtr != nullptr && bPtr->page_id != page_id){
        bPtr = bPtr->next;
    }
    if (bPtr != nullptr){
        bPtr->count--;
        if (!bPtr->count){
            bPtr->latch = 0;
        }
    }
    return bPtr->frame_id;
}

int
BMgr::NumFreeFrames() {
    return numPages;
}

int
BMgr::SelectVictim() {

    int data_frame_id = -1, frame_id = -1;

    if (numPages){
        for (int i = 0; i < DEFBUFSIZE; i++){
            if (ftop[i] == -1){
                numPages--;
                return  i;
            }
        }
    }
    else {
        LRU::LRUNode *lPtr = lru->GetHead();
         while(lPtr != nullptr){
            frame_id = Hash(lPtr->page_id);
            BCB *bPtr = ptof[frame_id];
            while(bPtr != nullptr && bPtr->page_id != lPtr->page_id){
                bPtr = bPtr->next;
            }
            if (bPtr != nullptr){
                if ((!bPtr->latch) && (!bPtr->count)){
                    if(bPtr->dirty){
                        DataFile->WritePage(bPtr->page_id, buf[bPtr->frame_id]);
                    }

                    data_frame_id = bPtr->frame_id;

                    RemoveLRUEle(data_frame_id);
                    RemoveBCB(bPtr, bPtr->page_id);

                    return data_frame_id;
                }
            }
            lPtr = lPtr->next;
        }
    }
}

int
BMgr::Hash(int page_id) {
    return page_id % DEFBUFSIZE;
}


int
BMgr::FindPage(int page_id) {
    int frame_id = Hash(page_id);
    if (page_id > 50000){
        std::cout << "page_id error!" << std::endl;
        return -1;
    }
    BCB *bPtr = ptof[frame_id];
    while(bPtr != nullptr && bPtr->page_id != page_id){
        bPtr = bPtr->next;
    }
    if (bPtr != nullptr){
        return bPtr->frame_id;
    }else{
        return -1;
    }
}

void
BMgr::RemoveBCB(BCB *ptr, int page_id) {

    int frame_id = Hash(page_id);
    BCB *bPtr = ptof[frame_id];
    if (bPtr == nullptr){
        return ;
    }
    else if(bPtr ==  ptr){
        ptof[frame_id] = bPtr->next;
        delete bPtr;
    }
    else{
        while(bPtr->next != nullptr && bPtr->next != ptr){
            bPtr = bPtr->next;
        }
        if (bPtr->next == ptr){
            bPtr->next = ptr->next;
            delete ptr;
        }
    }
}

void
BMgr::RemoveLRUEle(int frid) {
    lru->DeleteFid(frid);
}

void
BMgr::SetDirty(int frame_id) {
    int page_id = ftop[frame_id];
    BCB* bPtr = ptof[frame_id];
    while(bPtr != nullptr){
        if (bPtr->page_id == page_id){
            break;
        }
        else
            bPtr = bPtr->next;
    }
    if (bPtr != nullptr)
        ptof[frame_id]->dirty = 1;
}

void
BMgr::UnsetDirty(int frame_id) {
    int page_id = ftop[frame_id];
    BCB* bPtr = ptof[frame_id];
    while(bPtr != nullptr){
        if (bPtr->page_id == page_id){
            break;
        }
        else
            bPtr = bPtr->next;
    }
    if (bPtr != nullptr)
        ptof[frame_id]->dirty = 0;
}

void
BMgr::WriteDirtys() {
    BCB *bPtr;
    for (int i = 0; i < FRAMESIZE; i++){
        bPtr = ptof[i];
        while(bPtr != nullptr){
            if (bPtr->dirty){
                DataFile->WritePage(bPtr->page_id, buf[bPtr->frame_id]);
            }
            bPtr = bPtr->next;
        }

    }
}

void
BMgr::PrintFrame(int frame_id) {
    for (int i = 0; i < FRAMESIZE; i++){
        putchar(buf[frame_id].field[i]);
    }
}


