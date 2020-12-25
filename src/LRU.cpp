//
// Created by wzy1999 on 2020/12/9.
//

#include "LRU.h"

LRU::LRU() {
    head = nullptr;
    rear = nullptr;
}

void
LRU::InsertRear(int page_id, int frame_id, bool moveFlag) {
    if (moveFlag){
        LRUNode *headPtr = head, *secPtr = head->next;
        if (headPtr->page_id == page_id){
            DeleteHead();
            InsertRear(page_id, frame_id, false);
        }
        else{
            while(secPtr != nullptr && secPtr->page_id != page_id){
                headPtr = secPtr;
                secPtr = secPtr->next;
            }
            if (secPtr->page_id == page_id){
                headPtr->next = secPtr->next;
                rear->next = secPtr;
                rear = rear->next;
                secPtr->next = nullptr;
            }
        }
    }
    else{
        LRUNode *Ptr = new LRUNode (page_id, frame_id);
        if (head == nullptr) {
            head = Ptr;
            rear = Ptr;
        }
        else{
            rear->next = Ptr;
            rear = Ptr;
        }
    }
}

LRU::LRUNode
*LRU::GetHead() {
    return head;
}

int
LRU::DeleteFid(int frame_id) {
    if (head == nullptr)
        return -1;
    else if (head == rear){
        if (head->frame_id = frame_id){
            LRUNode *Ptr = head;
            int page_id = head->page_id;
            head = nullptr;
            rear = nullptr;
            delete Ptr;
            return page_id;
        }
        else {
            return -1;
        }
    }
    else if (head->frame_id == frame_id){
        LRUNode *Ptr = head;
        int page_id = head->page_id;
        head = head->next;
        delete Ptr;
        return page_id;
    }
    else{
        LRUNode *Ptr = head;
        LRUNode *sPtr = head->next;
        while(sPtr != nullptr && sPtr->frame_id != frame_id){
            sPtr = sPtr->next;
            Ptr = Ptr->next;
        }
        if (sPtr == nullptr){
            return 0;
        }
        else if (sPtr->frame_id == frame_id){
            int page_id = sPtr->page_id;
            Ptr->next = Ptr->next->next;
            delete sPtr;
            return page_id;
        }
    }
}

int
LRU::DeleteHead() {
    if (head == nullptr){
        return -1;
    } else if(head == rear){
        int page_id = head->page_id;
        LRUNode *Ptr = head;
        head = nullptr;
        rear = nullptr;
        delete Ptr;
        return page_id;
    }else{
        int page_id = head->page_id;
        LRUNode *Ptr = head;
        head = head->next;
        delete Ptr;
        return page_id;
    }
}