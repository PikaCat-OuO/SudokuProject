#include "grid.h"

Grid::Grid(QObject *parent) : QObject(parent)
{

}

void Grid::setPuzzle(std::array<std::array<size_t,9>,9>& puzzle){
    mPuzzle=puzzle;
}

std::array<std::array<size_t,9>,9>& Grid::getPuzzle(){
    return mPuzzle;
}

bool Grid::puzzleInput() {
    std::thread task([]{system("notepad.exe ifile.txt");});//用记事本打开输入文件
    task.detach();
    std::ifstream input("ifile.txt");
    std::ofstream output("ofile.txt", std::ofstream::trunc); //每个输出文件记录一道题目及其对应答案，记录新题目时清空文件
    int row = 0, col = 0;
    std::string t, tmp;
    output << "题目：" << std::endl; //记录题目
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++) {
            mPuzzle[i][j] = 0;
        } //初始化
    if (input&&output) {
        while (!input.eof()) {
            input >> t;
            tmp = tmp + t;
        }
        for (auto i : tmp) {
            if (row == 81) break; //超过81个格子的部分截断
            if (isdigit(i)) { //确保只有数字填入格子
                mPuzzle[(row++) / 9][(col++) % 9] = i - '0';
                output << i;
                if (row % 9 == 0)
                    output << std::endl; //数独矩阵每一行添加一个换行符
            }
        }
        input.close();
        output.close();
    }
    else
        return 0;
    return 1;
}

bool Grid::puzzleOutPut() {
    std::ofstream output("ofile.txt");
    output << "题解：" << std::endl;
    for(int i=0;i<9;i++)
        for (int j = 0; j < 9; j++) {
            output<< char(mPuzzle[i][j] + '0');
            if (j == 8) output << std::endl;
        }
    std::thread task([&]{system("notepad.exe ofile.txt");});
    task.detach();
    output.close();
    return 1;
}

void Grid::randomPuzzle(){
    mPuzzle=randompuzzle();
}
