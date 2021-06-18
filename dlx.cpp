#include "dlx.h"

dlx::dlx(QObject *parent) : QObject(parent)
{

}

void dlx:: createDLX(int i, int j) {
    idx = 0;
    head = &col[0];
    colSum = j, rowSum = i;
    DLXNode* ptr = head;
    for (int i = 1; i <= rowSum; i++) {
        row[i].l = row[i].r = &row[i];
        row[i].rowNum = i;
    }
    for (int i = 1; i <= colSum; i++) {
        col[i].colNum = i;
        col[i].colCnt = 0;
        col[i].dn = col[i].up = &col[i];
        ptr->r = &col[i];
        col[i].l = ptr;
        ptr = ptr->r;
    }
    col[colSum].r = head;
    head->l = &col[colSum];
}


void dlx::link(int j, int i) {
    nodeG[idx].rowNum = j;
    col[i].colCnt++;
    nodeG[idx].colHead = &col[i];

    nodeG[idx].up = col[i].up;
    nodeG[idx].dn = &col[i];
    col[i].up->dn = &nodeG[idx];
    col[i].up = &nodeG[idx];

    nodeG[idx].l = row[j].l;
    nodeG[idx].r = &row[j];
    row[j].l->r = &nodeG[idx]; //用数组来放新结点 不采用动态分配 提高速度
    row[j].l = &nodeG[idx];

    idx++;
}

void dlx::remove(DLXNode* target) {
    DLXNode* h = target->colHead;
    h->r->l = h->l;
    h->l->r = h->r;
    DLXNode* ctmp = h->dn;
    while (ctmp != h) {
        for (DLXNode* rtmp = ctmp->r; rtmp != ctmp; rtmp = rtmp->r) {
            if (rtmp != &row[ctmp->rowNum]) {
                rtmp->up->dn = rtmp->dn;
                rtmp->dn->up = rtmp->up;
                rtmp->colHead->colCnt--;
            }
        }
        ctmp = ctmp->dn;
    }
}

void dlx::resume(DLXNode* target) {
    DLXNode* h = target->colHead;
    DLXNode* ctmp = h->up;
    while (ctmp != h) {
        for (DLXNode* rtmp = ctmp->l; rtmp != ctmp; rtmp = rtmp->l) {
            if (rtmp != &row[ctmp->rowNum]) {
                rtmp->up->dn = rtmp;
                rtmp->dn->up = rtmp;
                rtmp->colHead->colCnt++;
            }
        }
        ctmp = ctmp->up;
    }
    h->r->l = h;
    h->l->r = h;
}

DLXNode* dlx::finMinCol() {
    DLXNode* minCol = head->r;
    for (DLXNode* ptr = head->r; ptr != head; ptr = ptr->r) {
        if (ptr->colCnt < minCol->colCnt && minCol->colCnt) {
            minCol = ptr;
        }
    }
    return minCol;
}

bool dlx::dance() {
    if (head->r == head) {
        return 1;
    }
    DLXNode* minCol = finMinCol();
    for (DLXNode* target = minCol->dn; target != minCol; target = target->dn) {
        ans[(target->rowNum - 1) / 81][(target->rowNum - 1) / 9 % 9] = target->rowNum % 9 ? target->rowNum % 9 : 9; //记录每一次尝试的答案
        if(mAnimationFlag){
            QThread::msleep(mSpeed);
            emit changeValue((target->rowNum - 1)/9,target->rowNum % 9 ? target->rowNum % 9 : 9);
        }
        for (DLXNode* ptr = row[target->rowNum].r; ptr != &row[target->rowNum]; ptr = ptr->r) {
            remove(ptr);
        }
        if (dance()) return 1;
        //dlx->ansidx--;
        for (DLXNode* ptr = row[target->rowNum].l; ptr != &row[target->rowNum]; ptr = ptr->l) {
            resume(ptr);
        }
    }
    return 0;
}

bool dlx::dlxSolve(std::array<std::array<size_t, 9>, 9>& mPuzzle) {
    int val;
    createDLX(729, 324);
    for (int i = 1; i <= 9; i++)
        for (int j = 1; j <= 9; j++) {
            val = mPuzzle[i-1][j-1];
            if (val) {
                link(((i - 1) * 9 + j - 1) * 9 + val, (i - 1) * 9 + j);
                link(((i - 1) * 9 + j - 1) * 9 + val, 81 + (i - 1) * 9 + val);
                link(((i - 1) * 9 + j - 1) * 9 + val, 162 + (j - 1) * 9 + val);
                link(((i - 1) * 9 + j - 1) * 9 + val, 243 + (((i - 1) / 3) * 3 + (j - 1) / 3) * 9 + val);
            }
            else for (int k = 1; k <= 9; k++) {
                    link(((i - 1) * 9 + j - 1) * 9 + k, (i - 1) * 9 + j);
                    link(((i - 1) * 9 + j - 1) * 9 + k, 81 + (i - 1) * 9 + k);
                    link(((i - 1) * 9 + j - 1) * 9 + k, 162 + (j - 1) * 9 + k);
                    link(((i - 1) * 9 + j - 1) * 9 + k, 243 + (((i - 1) / 3) * 3 + (j - 1) / 3) * 9 + k);
                }
        }
    bool ok = dance();
    idx = 0;
    if(!mAnimationFlag) emit changeAll(ans);
    for(size_t i=0;i<9;++i){
        for(size_t j=0;j<9;++j){
            mPuzzle[i][j]=ans[i][j];
        }
    }
    return ok;
}

void dlx::setAnimationFlag(bool animationFlag){
    mAnimationFlag=animationFlag;
}

void dlx::setSpeed(int speed){
    mSpeed=speed;
}
