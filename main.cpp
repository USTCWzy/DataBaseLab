//
// Created by wzy1999 on 2020/12/10.
//

#include <iostream>
#include <fstream>

#include "src\BufferManager.h"

using namespace std;

int main(){
    int IOFlag, page_id, frame_id, iTimes = 0, oTimes = 0, hitTimes = 0, nums = 0;
    int Page[1024] = {0};
    char c;
    BMgr *bMgr = new BMgr();

/*
    bMgr->lru->InsertRear(1, 1, 0)
    bMgr->lru->InsertRear(2, 2, 0)
    bMgr->lru->InsertRear(3, 3, 0)
    bMgr->lru->InsertRear(4, 4, 0)
*/
    ifstream dataFile("..//Dataset//data-5w-50w-zipf.txt");
    if(!dataFile){
        cerr << "open data-5w-50w-zipf.txt error" << endl;
        exit(1);
    }
    while(dataFile >> IOFlag >> c >> page_id){
        nums++;
        //cout << nums << " " << IOFlag << " " << page_id << " " << endl;
        if (nums == 1305 || nums == 501){
            cout << nums << " " << page_id << " " << bMgr->FindPage(12351) << endl;
            //cout << 1 << endl;
            //cout << nums << endl;
           // cout << bMgr->lru->GetHead()->page_id << endl;
        }
        frame_id = bMgr->FindPage(page_id);
        if (IOFlag){
            if (frame_id != -1){
                bMgr->UpdateLRU(page_id, frame_id);
                bMgr->SetDirty(frame_id);
                oTimes++;
                hitTimes++;
            }
            else{
                frame_id = bMgr->FixPage(page_id, 1);
                bMgr->SetDirty(frame_id);
                iTimes++;
                oTimes++;
            }
        }else{
            if (frame_id == -1){
                iTimes++;
                frame_id = bMgr->FixPage(page_id, 1);
            }
            else{
                hitTimes++;
                bMgr->UpdateLRU(page_id, frame_id);
            }
        }
    }
    cout << iTimes << endl;
    cout << oTimes << endl;
    cout << hitTimes << endl;
}