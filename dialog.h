#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include<QPropertyAnimation>
#include<QGraphicsDropShadowEffect>
#include<QMessageBox>
#include<QKeyEvent>
#include<QCloseEvent>
#include<QMouseEvent>
#include <QStandardItem>
#include <array>
#include <QElapsedTimer>
#include "grid.h"
#include "sudokusolver.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QColor ColorClose READ getColorClose WRITE setColorClose)
    Q_PROPERTY(QColor ColorMin READ getColorMin WRITE setColorMin)

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

protected:
    void closeEvent(QCloseEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *watched,QEvent *event);

private slots:
    void changeValue(const size_t& now,const size_t& maxNum);
    void changeAll(const std::array<std::array<size_t,9>,9>&);
    void on_CloseButton_clicked();
    void on_MinButton_clicked();
    void on_ShowSteps_currentIndexChanged(int index);
    void on_ClearGrid_clicked();
    void on_Start_clicked();
    void on_FileIn_clicked();
    void on_FileOut_clicked();
    void on_Random_clicked();
    void on_Algorithm_currentIndexChanged(int index);
    void on_StepsSpeed_currentIndexChanged(int index);
private:
    void setButtonDisable(bool);
    Ui::Dialog *ui;
    Grid* board=new Grid(this);
    SudokuSolver* solver=new SudokuSolver(this);
    bool CloseCheck=false;
    bool OnDialog=false;
    QPoint MouseStartPoint;
    QPoint DialogStartPoint;
    QColor getColorClose() const;
    void setColorClose(const QColor color);
    QColor ColorClose=QColor(212,64,39,0);
    QColor getColorMin() const;
    void setColorMin(const QColor color);
    QColor ColorMin=QColor(38,169,218,0);
};
#endif // DIALOG_H
