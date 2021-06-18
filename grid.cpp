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
    std::thread task([]{system("notepad.exe ifile.txt");});//�ü��±��������ļ�
    task.detach();
    std::ifstream input("ifile.txt");
    std::ofstream output("ofile.txt", std::ofstream::trunc); //ÿ������ļ���¼һ����Ŀ�����Ӧ�𰸣���¼����Ŀʱ����ļ�
    int row = 0, col = 0;
    std::string t, tmp;
    output << "��Ŀ��" << std::endl; //��¼��Ŀ
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++) {
            mPuzzle[i][j] = 0;
        } //��ʼ��
    if (input&&output) {
        while (!input.eof()) {
            input >> t;
            tmp = tmp + t;
        }
        for (auto i : tmp) {
            if (row == 81) break; //����81�����ӵĲ��ֽض�
            if (isdigit(i)) { //ȷ��ֻ�������������
                mPuzzle[(row++) / 9][(col++) % 9] = i - '0';
                output << i;
                if (row % 9 == 0)
                    output << std::endl; //��������ÿһ�����һ�����з�
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
    output << "��⣺" << std::endl;
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
