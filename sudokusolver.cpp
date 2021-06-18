#include "sudokusolver.h"

SudokuSolver::SudokuSolver(QObject *parent) : QObject(parent),loop(parent)
{
    connect(mDlx,SIGNAL(changeValue(const size_t&,const size_t&)),this,SLOT(sig1ref(const size_t&,const size_t&)));
    connect(mDlx,SIGNAL(changeAll(const std::array<std::array<size_t,9>,9>&)),this,SLOT(sig2ref(const std::array<std::array<size_t,9>,9>&)));
    connect(this,SIGNAL(taskEnds()),&loop,SLOT(quit()));
}

void SudokuSolver::stop(){
    quit=true;
}

bool SudokuSolver::check(std::array<std::array<size_t,9>,9>& puzzle){
    for(size_t i=0;i<9;++i){//检查行列
        for(size_t k=1;k<=9;++k){
            size_t rowCount=0,colCount=0;//行列重复数字计数器
            for(size_t j=0;j<9;++j){
                if(puzzle[i][j]==k) rowCount++;
                if(puzzle[j][i]==k) colCount++;
            }
            if(rowCount>1||colCount>1) return false;//单行单列重复数字
        }
    }
    for(size_t i=0;i<9;++i){//检查九宫格
        for(size_t l=1;l<=9;++l){
            size_t count=0;//九宫格重复数字计数器
            for(size_t j=(i*3)%9;j<(i*3+3)%9;++j){
                for(size_t k=i/3*3;k<i/3*3+3;++k){
                    if(puzzle[j][k]==l) count++;
                }
            }
            if(count>1) return false;
        }
    }
    return true;
}

//dfs解数独
void SudokuSolver::dfsSolve(std::array<std::array<size_t,9>,9>& puzzle,size_t now){
    if(quit) return;
    //迭代结束条件
    if(now==81){
        dfsDone=true;//解答完成，标志设为真
        if(!mAnimationFlag){
            emit changeAll(puzzle);
        }
        return;
    }
    if(puzzle[now/9][now%9]!=0){//跳过已有部分
        dfsSolve(puzzle,now+1);return;
    }
    size_t maxNum=9;
    for(;(maxNum!=0)&&!dfsDone;maxNum--){
        bool flag=true;
        for(size_t i=0;i<9;i++){//检查行列
            if(puzzle[now/9][i]==maxNum){flag=false;break;}
            if(puzzle[i][now%9]==maxNum){flag=false;break;}
        }
        for(size_t i=now/9/3*3;(i<now/9/3*3+3)&&flag;i++)//检查九宫格
            for(size_t j=now%9/3*3;(j<now%9/3*3+3)&&flag;j++)
                if(puzzle[i][j]==maxNum){flag=false;break;}
        if(flag){
            puzzle[now/9][now%9]=maxNum;
            if(mAnimationFlag){
                QThread::msleep(mSpeed);
                emit changeValue(now,maxNum);
            }
            dfsSolve(puzzle,now+1);
        }
    }
    if(maxNum==0&&!dfsDone){//回溯清零
        puzzle[now/9][now%9]=0;return;
    }
}

void SudokuSolver::solve(std::array<std::array<size_t,9>,9>& puzzle){
    if(mAlgoType==0){
        std::thread task([&]{
            dfsSolve(puzzle);
            emit taskEnds();
        });
        task.detach();
        loop.exec();
        dfsDone=false;//算法完成，恢复标志准备下一次解题
    }else{
        std::thread task([&]{
            mDlx->dlxSolve(puzzle);
            emit taskEnds();
        });
        task.detach();
        loop.exec();
    }
}

void SudokuSolver::setAlgo(int algoType){
    mAlgoType=algoType;
}

void SudokuSolver::setAnimation(bool animationFlag){
    mAnimationFlag=animationFlag;
    mDlx->setAnimationFlag(mAnimationFlag);
}

void SudokuSolver::setSpeed(int speed){
    if(speed==0){
        mSpeed=1;
    }else if(speed==1){
        mSpeed=20;
    }else if(speed==2){
        mSpeed=50;
    }
    mDlx->setSpeed(mSpeed);
}

void SudokuSolver::sig1ref(const size_t& now,const size_t& maxNum){
    emit changeValue(now,maxNum);
}

void SudokuSolver::sig2ref(const std::array<std::array<size_t,9>,9>& puzzle){
    emit changeAll(puzzle);
}
