#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    //安装标题栏按钮事件监控
    ui->CloseButton->installEventFilter(this);
    ui->MinButton->installEventFilter(this);
    //设置无边框窗口
    setWindowFlag(Qt::FramelessWindowHint);
    //设置窗口透明
    setAttribute(Qt::WA_TranslucentBackground);
    //设置窗口在屏幕正中央显示
    auto desk = QApplication::primaryScreen()->geometry();
    move((desk.width() - this->width()) / 2,
         (desk.height() - this->height()) / 2);
    //窗口启动动画
    QPropertyAnimation *Ani=new QPropertyAnimation(this,"windowOpacity");
    Ani->setDuration(600);
    Ani->setStartValue(0);
    Ani->setEndValue(0.94);
    Ani->setEasingCurve(QEasingCurve::InOutSine);
    Ani->start();
    //设置窗口阴影
    QGraphicsDropShadowEffect *Shadow = new QGraphicsDropShadowEffect;
    Shadow->setOffset(0,0);
    Shadow->setColor(QColor(0,0,0,50));
    Shadow->setBlurRadius(10);
    ui->frame->setGraphicsEffect(Shadow);
    for(size_t i=0;i<9;++i){
        for(size_t j=0;j<9;++j){
            ui->Grid->item(i,j)->setTextAlignment(Qt::AlignCenter);
        }
    }
    qRegisterMetaType<std::array<std::array<size_t,9>,9>>("std::array<std::array<size_t,9>,9>");
    qRegisterMetaType<size_t>("size_t");
    connect(solver,SIGNAL(changeValue(const size_t&,const size_t&)),this,SLOT(changeValue(const size_t&,const size_t&)));
    connect(solver,SIGNAL(changeAll(const std::array<std::array<size_t,9>,9>&)),this,SLOT(changeAll(const std::array<std::array<size_t,9>,9>&)));
}

Dialog::~Dialog()
{
    delete ui;
}

//-----------------------事件函数---------------------------//
//重写关闭事件实现窗口关闭动画
void Dialog::closeEvent(QCloseEvent *event)
{
    if(CloseCheck==false){
        CloseCheck=true;
        event->ignore();
        //窗口关闭动画
        QPropertyAnimation *Ani=new QPropertyAnimation(this,"windowOpacity");
        Ani->setDuration(600);
        Ani->setStartValue(0.94);
        Ani->setEndValue(0);
        Ani->setEasingCurve(QEasingCurve::InOutSine);
        Ani->start();
        connect(Ani,SIGNAL(finished()),this,SLOT(close()));
    }else{
        solver->stop();
        event->accept();
    }
}

//重写鼠标移动事件，实现点击窗口任意处移动窗口的功能
void Dialog::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        OnDialog=true;
        MouseStartPoint = event->globalPosition().toPoint();
        DialogStartPoint=this->frameGeometry().topLeft();
    }
}

void Dialog::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons()==Qt::LeftButton)&&OnDialog){
      QPoint MouseMovePoint =
          event->globalPosition().toPoint() - MouseStartPoint;
      this->move(DialogStartPoint + MouseMovePoint);
    }else{
        //单击按住按钮后离开按钮区域，按钮回复原样
        ColorClose=QColor(212,64,39,0);
        QString QssClose=QString("#CloseButton{border-image:url(:/Images/close.ico);border-radius:5px;background: rgba(212,64,39,0);}#CloseButton:pressed{border-image:url(:/Images/close_press.ico);}");
        ui->CloseButton->setStyleSheet(QssClose);
        ColorMin=QColor(38,169,218,0);
        QString QssMin=QString("#MinButton{border-image:url(:/Images/min.ico);border-radius:5px;background: rgba(38,169,218,0);}#MinButton:pressed{border-image:url(:/Images/min_press.bmp);}");
        ui->MinButton->setStyleSheet(QssMin);
    }
}

void Dialog::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        OnDialog=false;
    }
}

//实现按Esc键关闭窗口时也能播放动画
void Dialog::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Escape){
        this->close();
    }
}

//标题栏按钮动画
bool Dialog::eventFilter(QObject *watched, QEvent *event)
{
    //关闭按钮点燃效果
    if(watched==ui->CloseButton){
        if(event->type()==QEvent::Enter){
            QPropertyAnimation *Ani=new QPropertyAnimation(this,"ColorClose");
            Ani->setDuration(100);
            Ani->setStartValue(getColorClose());
            Ani->setEndValue(QColor(212,64,39,230));
            Ani->start();
            return true;
        }
        if(event->type()==QEvent::Leave){
            QPropertyAnimation *Ani=new QPropertyAnimation(this,"ColorClose");
            Ani->setDuration(100);
            Ani->setStartValue(getColorClose());
            Ani->setEndValue(QColor(212,64,39,0));
            Ani->start();
            return true;
        }
    }
    //最小化按钮点燃效果
    if(watched==ui->MinButton){
        if(event->type()==QEvent::Enter){
            QPropertyAnimation *Ani=new QPropertyAnimation(this,"ColorMin");
            Ani->setDuration(100);
            Ani->setStartValue(getColorMin());
            Ani->setEndValue(QColor(38,169,218,230));
            Ani->start();
            return true;
        }
        if(event->type()==QEvent::Leave){
            QPropertyAnimation *Ani=new QPropertyAnimation(this,"ColorMin");
            Ani->setDuration(100);
            Ani->setStartValue(getColorMin());
            Ani->setEndValue(QColor(38,169,218,0));
            Ani->start();
            return true;
        }
    }
    return false;
}
//----------------------事件函数结束------------------------//

//---------------------标题栏按钮函数------------------------//
//获取关闭按钮颜色
QColor Dialog::getColorClose() const
{
    return ColorClose;
}
//设置关闭按钮颜色
void Dialog::setColorClose(const QColor color)
{
    ColorClose=color;
    QString qss=QString("#CloseButton{border-image:url(:/Images/close.ico);border-radius:5px;background: rgba(%1, %2, %3, %4);}#CloseButton:pressed{border-image:url(:/Images/close_press.ico);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    ui->CloseButton->setStyleSheet(qss);
}
//获得最小化按钮颜色
QColor Dialog::getColorMin() const
{
    return ColorMin;
}
//设置最小化按钮颜色
void Dialog::setColorMin(const QColor color)
{
    ColorMin=color;
    QString qss=QString("#MinButton{border-image:url(:/Images/min.ico);border-radius:5px;background: rgba(%1, %2, %3, %4);}#MinButton:pressed{border-image:url(:/Images/min_press.bmp);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    ui->MinButton->setStyleSheet(qss);
}
//--------------------标题栏按钮函数结束---------------------//

//----------------------窗口组件函数------------------------//
//窗口按钮事件
void Dialog::on_CloseButton_clicked()
{
    this->close();
}

void Dialog::on_MinButton_clicked()
{
    this->showMinimized();
}

void Dialog::on_Start_clicked()
{
    setButtonDisable(true);
    ui->StateLable->setText(QString::fromLocal8Bit("状态：正在运算中"));
    ui->StateLable->setStyleSheet(R"(font: 75 18pt "Microsoft YaHei UI";)");
    //将表格内容读取到array中
    std::array<std::array<size_t,9>,9> puzzle{};
    for(size_t i=0;i<9;++i){
        for(size_t j=0;j<9;++j){
            QString content=ui->Grid->item(i,j)->text();
            if(content!=""&&content.toUInt()>0&&content.toUInt()<10){
                puzzle[i][j]=ui->Grid->item(i,j)->text().toUInt();
            }else{
                ui->Grid->item(i,j)->setText("");
            }
            ui->Grid->item(i,j)->setForeground(QBrush(Qt::black));
        }
    }
    //开始解答
    board->setPuzzle(puzzle);
    if(solver->check(board->getPuzzle())){
        solver->solve(board->getPuzzle());
    }else{
        ui->StateLable->setText(QString::fromLocal8Bit("状态:该数独无解"));
        ui->StateLable->setStyleSheet(R"(font: 75 18pt "Microsoft YaHei UI";color:rgb(255, 0, 0);)");
        setButtonDisable(false);
        return;
    }
    for(size_t i=0;i<9;++i){
        for(size_t j=0;j<9;++j){
            ui->Grid->item(i,j)->setForeground(QBrush(Qt::darkGreen));
        }
    }
    ui->StateLable->setText(QString::fromLocal8Bit("状态：运算完成"));
    ui->StateLable->setStyleSheet(R"(font: 75 18pt "Microsoft YaHei UI";color:rgb(14, 135, 14);)");
    setButtonDisable(false);
}

void Dialog::on_ShowSteps_currentIndexChanged(int index)
{
    solver->setAnimation(!index);
}


void Dialog::on_ClearGrid_clicked()
{
    for(size_t i=0;i<9;++i){
        for(size_t j=0;j<9;++j){
            ui->Grid->item(i,j)->setText("");
            ui->Grid->item(i,j)->setForeground(Qt::black);
        }
    }
    ui->StateLable->setText(QString::fromLocal8Bit("状态：等待计算"));
    ui->StateLable->setStyleSheet(R"(font: 75 18pt "Microsoft YaHei UI";)");
}

void Dialog::on_FileIn_clicked()
{
    if(!board->puzzleInput()){
        ui->StateLable->setText(QString::fromLocal8Bit("状态：文件读入失败"));
        ui->StateLable->setStyleSheet(R"(font: 75 18pt "Microsoft YaHei UI";color:rgb(255, 0, 0);)");
    }else{
        ui->StateLable->setText(QString::fromLocal8Bit("状态：文件读入成功"));
        ui->StateLable->setStyleSheet(R"(font: 75 18pt "Microsoft YaHei UI";color:rgb(14, 135, 14);)");
        changeAll(board->getPuzzle());
    }
}

void Dialog::on_FileOut_clicked()
{
    if(!solver->check(board->getPuzzle())){
        ui->StateLable->setText(QString::fromLocal8Bit("状态:该数独无解"));
        ui->StateLable->setStyleSheet(R"(font: 75 18pt "Microsoft YaHei UI";color:rgb(255, 0, 0);)");
        return;
    }
    if(!board->puzzleOutPut()){
        ui->StateLable->setText(QString::fromLocal8Bit("状态：文件输出失败"));
        ui->StateLable->setStyleSheet(R"(font: 75 18pt "Microsoft YaHei UI";color:rgb(255, 0, 0);)");
    }else{
        ui->StateLable->setText(QString::fromLocal8Bit("状态：文件输出成功"));
        ui->StateLable->setStyleSheet(R"(font: 75 18pt "Microsoft YaHei UI";color:rgb(14, 135, 14);)");
    }
}

void Dialog::on_Random_clicked()
{
    on_ClearGrid_clicked();
    board->randomPuzzle();
    changeAll(board->getPuzzle());
}

void Dialog::on_Algorithm_currentIndexChanged(int index)
{
    solver->setAlgo(!index);
}

void Dialog::on_StepsSpeed_currentIndexChanged(int index)
{
    solver->setSpeed(index);
}


void Dialog::setButtonDisable(bool flag){
    ui->ClearGrid->setDisabled(flag);
    ui->FileIn->setDisabled(flag);
    ui->FileOut->setDisabled(flag);
    ui->Random->setDisabled(flag);
    ui->Start->setDisabled(flag);
    if (flag) {
      ui->Grid->setEditTriggers(QAbstractItemView::NoEditTriggers);
    } else {
      ui->Grid->setEditTriggers(QAbstractItemView::AnyKeyPressed |
                                QAbstractItemView::DoubleClicked);
    }
}
//--------------------窗口组件函数结束-----------------------//

//------------------------槽函数---------------------------//
void Dialog::changeValue(const size_t &now, const size_t &maxNum){
    ui->Grid->item(now/9,now%9)->setText(QString::number(maxNum));
}

void Dialog::changeAll(const std::array<std::array<size_t,9>,9> &grid){
    for(size_t i=0;i<9;++i){
        for(size_t j=0;j<9;++j){
            if(grid[i][j]){
                ui->Grid->item(i,j)->setText(QString::number(grid[i][j]));
            }
            ui->Grid->item(i,j)->setForeground(QBrush(Qt::black));
        }
    }
}
//-----------------------槽函数结束-------------------------//
