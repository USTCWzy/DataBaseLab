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
        LRUNode *headPtr = head, *secPtr;
        if (headPtr->page_id == page_id){
            DeleteHead();
            InsertRear(page_id, frame_id, false);
        }
        else{
            while ( headPtr->next != nullptr && headPtr->next->page_id != page_id){
                headPtr = headPtr->next;
            }
            if (headPtr->next == nullptr){
                Print();
            }
            else if (headPtr->next->page_id == page_id){
                if (headPtr->next != rear) {
                    secPtr = headPtr->next;
                    headPtr->next = secPtr->next;
                    rear->next = secPtr;
                    rear = secPtr;
                    secPtr->next = nullptr;
                }
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
        LRUNode *Ptr = head, *sPtr;
        while(Ptr->next != nullptr && Ptr->next->frame_id != frame_id){
            Ptr = Ptr->next;
        }
        if (Ptr->next == nullptr){
            return 0;
        } else {
            sPtr = Ptr->next;
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

void
LRU::Print() {
    LRUNode *Ptr = head;
    while(Ptr != rear){
        std::cout << Ptr->page_id << std::endl;
        Ptr = Ptr->next;
    }
    std::cout << rear->page_id << std::endl;
}