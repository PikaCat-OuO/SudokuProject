#ifndef DLX_H
#define DLX_H

#include <QObject>
#include <QThread>

struct DLXNode {
    DLXNode* l, * r, * up, * dn; //对于行表头结点 up，dn是无用的
    //仅用于列表头结点
    int colCnt;//每列列结点数
    int colNum;//列编号
    //用于所有节点
    DLXNode* colHead;//记录列表头位置
    //用于行表头节点和普通节点
    int rowNum;//记录第几行
};

class dlx : public QObject
{
    Q_OBJECT
public:
    explicit dlx(QObject *parent = nullptr);
    void setAnimationFlag(bool);
    void setSpeed(int);
    std::array<std::array<size_t, 9>, 9> ans;
    void createDLX(int rowSum, int colSum);
    void link(int j, int i);
    void remove(DLXNode* target);
    void resume(DLXNode* target);
    DLXNode* finMinCol();
    bool dance();
    bool dlxSolve(std::array<std::array<size_t, 9>, 9>&);

private:
    static const int maxN = 248530;
    static const int maxC = 325;
    static const int maxR = 730;
    DLXNode* head;
    int colSum, rowSum;
    DLXNode col[maxC];
    DLXNode row[maxR];
    DLXNode nodeG[maxN];
    int idx;
    bool mAnimationFlag{true};
    int mSpeed=1;//控制速度

signals:
    void changeValue(const size_t& now,const size_t& maxNum);
    void changeAll(const std::array<std::array<size_t,9>,9>&);
};

#endif // DLX_H
