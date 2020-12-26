//
// Created by wzy1999 on 2020/12/8.
//

#include "DataStorageManager.h"


DSMgr::DSMgr() {
    FILE *fp = fopen("DataSet\\data.dbf", "rb");
    if (!fp){
        InitializeRandom();
    }else{
        fclose(fp);
    }

    currFile = nullptr;
    numPages = 0;
    for (int i = 0; i < MAXPAGES; i++){
        pages[i] = 1;
        index[i] = i;
        blocks[i] = 1;
    }
}

DSMgr::~DSMgr() {
    CloseFile();
}

int
DSMgr::OpenFile(std::string filename) {
    if((currFile = fopen(filename.data(), "rb")) == nullptr){
        return -1;
    }
    return 0;
}

void
DSMgr::InitializeRandom() {
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 9);


    FILE *fp = fopen("DataSet\\data.dbf", "wb");
    if (!fp){
        std::cerr << "open data.dbf error!" << std::endl;
        exit(1);
    }

    bFrame *data = new bFrame();
    int addr[1024] = {0};

    for (int i = 0; i < 49; i++){
        addr[0] = (i + 1);
        for (int j = 0; j < 1023; j++){
            addr[j + 1] = 49 + j + i * 1023;
        }
        fwrite(addr, sizeof(int), 1024, fp);
    }

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

    int pageAddr = 0, dicAddr = 0;
    page_id = index[page_id];
    dicAddr = page_id / 1023 * 1024 + page_id % 1023 + 1;
    Seek(dicAddr * sizeof(int), 0);
    fread(&pageAddr, sizeof(int), 1, currFile);
    Seek(pageAddr * FRAMESIZE, 0);

    fread(data, sizeof(char), FRAMESIZE, currFile);
    for (int i = 0; i < FRAMESIZE; i++){
        page.field[i] = data[i];
    }
    return page;
}

int
DSMgr::WritePage(int frame_id, bFrame frm) {
    int pageAddr, dicAddr = 0;
    frame_id = index[frame_id];
    dicAddr = frame_id / 1023 * 1024 + frame_id % 1023 + 1;
    Seek(dicAddr * sizeof(int), 0);
    fread(&pageAddr, sizeof(int), 1, currFile);
    Seek(pageAddr * FRAMESIZE, 0);
    return fwrite(frm.field, sizeof (char), FRAMESIZE, currFile);
}

int
DSMgr::Seek(int offset, int pos) {
    return fseek(currFile, offset, pos);
}

int
DSMgr::GetNewPage() {
    int i, j;
    for (i = 0; i < MAXPAGES; i++){
        if (!pages[i]){
            break;
        }
    }
    for (j = 0; j < MAXPAGES; j++){
        if (!blocks[j]){
            break;
        }
    }
    if (i == MAXPAGES){
        std::cout << "No extra pages!" << std::endl;
        return -1;
    }
    if (j == MAXPAGES){
        std::cout << "No extra blocks!" << std::endl;
        return -1;
    }
    index[j] = i;
    blocks[j] = 1;
    pages[i] = 1;
    return j;
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