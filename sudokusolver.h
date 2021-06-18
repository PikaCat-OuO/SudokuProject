#ifndef SUDOKUSOLVER_H
#define SUDOKUSOLVER_H

#include <QObject>
#include <thread>
#include <QEventLoop>
#include <QThread>
#include "dlx.h"

class SudokuSolver : public QObject
{
    Q_OBJECT
public:
    explicit SudokuSolver(QObject *parent = nullptr);
    void solve(std::array<std::array<size_t,9>,9>&);
    bool check(std::array<std::array<size_t,9>,9>&);
    void setAlgo(int);
    void setAnimation(bool);
    void setSpeed(int);
    void stop();

private:
    void dfsSolve(std::array<std::array<size_t,9>,9>&,size_t=0);
    dlx* mDlx=new dlx(this);
    int mSpeed{1};
    bool mAnimationFlag{true};
    QEventLoop loop;
    int mAlgoType{1};//算法类型
    std::atomic_bool quit{false};
    bool dfsDone{false};//dfs算法的解题完成标志

signals:
    void changeValue(const size_t& now,const size_t& maxNum);
    void changeAll(const std::array<std::array<size_t,9>,9>&);
    void taskEnds();

public slots:
    void sig1ref(const size_t& now,const size_t& maxNum);
    void sig2ref(const std::array<std::array<size_t,9>,9>&);
};

#endif // SUDOKUSOLVER_H
