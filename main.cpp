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

    ifstream dataFile("Dataset//data-5w-50w-zipf.txt");
    if(!dataFile){
        cerr << "open data-5w-50w-zipf.txt error" << endl;
        exit(1);
    }
    while(dataFile >> IOFlag >> c >> page_id){
        nums++;
        cout << nums << " " << IOFlag << " " << page_id << " " << endl;
        if (page_id == 676){
            cout << nums << " " << page_id << " " << bMgr->FindPage(12351) << endl;
            //cout << 1 << endl;
            //cout << nums << endl;
           // cout << bMgr->lru->GetHead()->page_id << endl;
        }
        if (IOFlag){
            frame_id = bMgr->FindPage(page_id);
            if (frame_id != -1){
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
            if (bMgr->FindPage(page_id) == -1){
                iTimes++;
                frame_id = bMgr->FixPage(page_id, 1);
            }
            else{
                hitTimes++;
            }
        }
    }
    cout << iTimes << endl;
    cout << oTimes << endl;
    cout << hitTimes << endl;
}