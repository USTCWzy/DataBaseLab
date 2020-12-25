//
// Created by wzy1999 on 2020/12/8.
//

#include "DataStorageManager.h"


DSMgr::DSMgr() {
    FILE *fp = fopen("DataSet//data.dbf", "r");
    if (!fp){
        InitializeRandom();
    }else{
        fclose(fp);
    }

    currFile = nullptr;
    numPages = 0;
    for (int i = 0; i < MAXPAGES; i++){
        pages[i] = 0;
    }
}

DSMgr::~DSMgr() {
    CloseFile();
}

int
DSMgr::OpenFile(std::string filename) {
    if((currFile = fopen(filename.data(), "r")) == nullptr){
        return -1;
    }
    return 0;
}

void
DSMgr::InitializeRandom() {
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 9);


    FILE *fp = fopen("DataSet//data.dbf", "w");
    if (!fp){
        std::cerr << "open data.dbf error!" << std::endl;
        exit(1);
    }

    bFrame *data = new bFrame();
    for (int i = 0; i < 50000; i++){
        char initRandom = dist(rng) + '0';
        memset(data->field, initRandom, FRAMESIZE);
        fwrite(data->field, sizeof(char), FRAMESIZE, fp);
    }

    fclose(fp);
    delete data;
}

int
DSMgr::CloseFile() {
    return fclose(currFile);
}

bFrame
DSMgr::ReadPage(int page_id) {
    bFrame page;
    char data[FRAMESIZE];
    Seek(FRAMESIZE * page_id, 0);
    fread(data, sizeof(char), FRAMESIZE, currFile);
    for (int i = 0; i < FRAMESIZE; i++){
        page.field[i] = data[i];
    }
    return page;
}

int
DSMgr::WritePage(int frame_id, bFrame frm) {
    Seek(FRAMESIZE * frame_id, 0);
    return fwrite(frm.field, sizeof (char), FRAMESIZE, currFile);
}

int
DSMgr::Seek(int offset, int pos) {
    return fseek(currFile, offset, pos);
}

int
DSMgr::GetNewPage() {
    for (int i = 0; i < MAXPAGES; i++){
        if (!pages[i]){
            return i;
        }
    }
    return 0;
}

FILE
*DSMgr::GetFile() {
    return currFile;
}

void
DSMgr::IncNumPages() {
    numPages += 1;
}

int
DSMgr::GetNumPages() {
    return numPages;
}

void
DSMgr::SetUse(int index, int use_bit) {
    pages[index] = use_bit;
}

int
DSMgr::GetUse(int index) {
    return pages[index];
}