//
// Created by wzy1999 on 2020/12/9.
//

#ifndef PROJECT_LRU_H
#define PROJECT_LRU_H

class LRU {
public:
    typedef struct LRUNode{
        LRUNode *next;
        int page_id;
        int frame_id;

        LRUNode(int pid, int fid){
            this->page_id = pid;
            this->frame_id = fid;
            this->next = nullptr;

        }
    }LRUNode;

    LRU();
    void InsertRear(int page_id, int frame_id, bool moveFlag);
    LRUNode *GetHead();
    int DeleteFid(int frame_id);
    int DeleteHead();
private:
    LRUNode *head, *rear;

};


#endif //PROJECT_LRU_H
