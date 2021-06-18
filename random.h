#ifndef RANDOM_H
#define RANDOM_H

#include <time.h>
#include <stdlib.h>
#include<array>
class Random
{
public:
    int sudo[9][9] = {{0}};
    bool set(int x, int y, int val);
    void reset(int x, int y);
    void initXOrd(int* xOrd);
    bool fillFrom(int y, int val);
    void dighole();
};
std::array<std::array<size_t, 9>, 9>  randompuzzle();

#endif // RANDOM_H
