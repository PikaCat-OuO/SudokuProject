#include "random.h"
bool Random::set(int x, int y, int val)
{
    if (sudo[y][x] != 0)        //非空
        return false;
    int x0, y0;
    for (x0 = 0; x0 < 9; x0++)
    {
        if (sudo[y][x0] == val) //行冲突
            return false;
    }
    for (y0 = 0; y0 < 9; y0++)
    {
        if (sudo[y0][x] == val) //列冲突
            return false;
    }
    for (y0 = y / 3 * 3; y0 < y / 3 * 3 + 3; y0++)
    {
        for (x0 = x / 3 * 3; x0 < x / 3 * 3 + 3; x0++)
        {
            if (sudo[y0][x0] == val) //格冲突
                return false;
        }
    }
    sudo[y][x] = val;
    return true;
}
void Random::reset(int x, int y)
{
    sudo[y][x] = 0;
}
void Random::initXOrd(int* xOrd)    //0~9随机序列
{
    int i, k, tmp;
    for (i = 0; i < 9; i++)
    {
        xOrd[i] = i;
    }
    for (i = 0; i < 9; i++)
    {
        k = rand() % 9;
        tmp = xOrd[k];
        xOrd[k] = xOrd[i];
        xOrd[i] = tmp;
    }
}

bool Random::fillFrom(int y, int val)
{
    int xOrd[9];
    initXOrd(xOrd);     //生成当前行的扫描序列
    for (int i = 0; i < 9; i++)
    {
        int x = xOrd[i];
        if (set(x, y, val))
        {
            if (y == 8)                 //到了最后一行
            {
                //当前填9则结束, 否则从第一行填下一个数
                if (val == 9 || fillFrom(0, val + 1))
                    return true;
            }
            else
            {
                if (fillFrom(y + 1, val)) //下一行继续填当前数
                    return true;
            }
            reset(x, y);    //回溯
        }
    }
    return false;
}
void Random::dighole()
{
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 8; j++)
        {
            int tmpi = rand() % 9;
            sudo[i][tmpi] = 0;
        }
}
std::array<std::array<size_t, 9>, 9> randompuzzle ()
{
    Random a;
    srand((unsigned)time(NULL));
    while (!a.fillFrom(0, 1));
    a.dighole();
    std::array<std::array<size_t, 9>, 9> array1;
    for (int j = 0; j < 9; j++)
    {
        for (int i = 0; i < 9; i++)
        {
            array1[j][i] = a.sudo[j][i];
        }
    }
    return array1;
}
