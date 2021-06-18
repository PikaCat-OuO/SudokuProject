#ifndef GRID_H
#define GRID_H

#include <QObject>
#include <thread>
#include <fstream>
#include "random.h"

class Grid : public QObject
{
    Q_OBJECT
public:
    explicit Grid(QObject *parent = nullptr);
    void setPuzzle(std::array<std::array<size_t,9>,9>&);
    std::array<std::array<size_t,9>,9>& getPuzzle();
    void randomPuzzle();
    bool puzzleInput();
    bool puzzleOutPut();

private:
    std::array<std::array<size_t,9>,9> mPuzzle{};
};

#endif // GRID_H
