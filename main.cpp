//
// Created by wzy1999 on 2020/12/10.
//

#include <iostream>
#include <fstream>
#include <windows.h>

#include "src\BufferManager.h"

using namespace std;

void Judge(){
    ifstream file1("hit.txt");
    ifstream file2("if_hit.txt");

    int ifHit, Hit;

    int numf = 0;

    while(file1 >> Hit){
        numf++;
        file2 >> ifHit;
        if (ifHit != Hit){
            cout << numf << endl;
        }
    }
}

int main(){


    //Judge();


    int IOFlag, page_id, frame_id, iTimes = 0, oTimes = 0, iHitTimes = 0, oHitTimes = 0, nums = 0;

    char c;
    BMgr *bMgr = new BMgr();


    LARGE_INTEGER  num;
    long long start,end,freq;
    QueryPerformanceFrequency(&num);
    freq = num.QuadPart;

    ifstream dataFile("..//Dataset//data-5w-50w-zipf.txt");
    if(!dataFile){
        cerr << "open data-5w-50w-zipf.txt error" << endl;
        exit(1);
    }

    QueryPerformanceCounter(&num);
    start = num.QuadPart;

    while(dataFile >> IOFlag >> c >> page_id){
        nums++;

        frame_id = bMgr->FindPage(page_id);
        if (IOFlag){
            if (frame_id != -1){
                bMgr->UpdateLRU(page_id, frame_id);
                bMgr->SetDirty(frame_id);
                oTimes++;
                oHitTimes++;
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
                iHitTimes++;
                bMgr->UpdateLRU(page_id, frame_id);
            }
        }
    }

    QueryPerformanceCounter(&num);
    end = num.QuadPart;

    cout << "iTimes : " << iTimes << endl;
    cout << "oTimes : " << oTimes << endl;
    cout << "iHitTimes : " << iHitTimes << endl;
    cout << "oHitTimes : " << oHitTimes << endl;
    cout << "Time : " << (end - start) * 1000 / freq  << endl;
}