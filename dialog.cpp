#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    //��װ��������ť�¼����
    ui->CloseButton->installEventFilter(this);
    ui->MinButton->installEventFilter(this);
    //�����ޱ߿򴰿�
    setWindowFlag(Qt::FramelessWindowHint);
    //���ô���͸��
    setAttribute(Qt::WA_TranslucentBackground);
    //���ô�������Ļ��������ʾ
    auto desk = QApplication::primaryScreen()->geometry();
    move((desk.width() - this->width()) / 2,
         (desk.height() - this->height()) / 2);
    //������������
    QPropertyAnimation *Ani=new QPropertyAnimation(this,"windowOpacity");
    Ani->setDuration(600);
    Ani->setStartValue(0);
    Ani->setEndValue(0.94);
    Ani->setEasingCurve(QEasingCurve::InOutSine);
    Ani->start();
    //���ô�����Ӱ
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

//-----------------------�¼�����---------------------------//
//��д�ر��¼�ʵ�ִ��ڹرն���
void Dialog::closeEvent(QCloseEvent *event)
{
    if(CloseCheck==false){
        CloseCheck=true;
        event->ignore();
        //���ڹرն���
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

//��д����ƶ��¼���ʵ�ֵ���������⴦�ƶ����ڵĹ���
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
        //������ס��ť���뿪��ť���򣬰�ť�ظ�ԭ��
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

//ʵ�ְ�Esc���رմ���ʱҲ�ܲ��Ŷ���
void Dialog::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Escape){
        this->close();
    }
}

//��������ť����
bool Dialog::eventFilter(QObject *watched, QEvent *event)
{
    //�رհ�ť��ȼЧ��
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
    //��С����ť��ȼЧ��
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
//----------------------�¼���������------------------------//

//---------------------��������ť����------------------------//
//��ȡ�رհ�ť��ɫ
QColor Dialog::getColorClose() const
{
    return ColorClose;
}
//���ùرհ�ť��ɫ
void Dialog::setColorClose(const QColor color)
{
    ColorClose=color;
    QString qss=QString("#CloseButton{border-image:url(:/Images/close.ico);border-radius:5px;background: rgba(%1, %2, %3, %4);}#CloseButton:pressed{border-image:url(:/Images/close_press.ico);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    ui->CloseButton->setStyleSheet(qss);
}
//�����С����ť��ɫ
QColor Dialog::getColorMin() const
{
    return ColorMin;
}
//������С����ť��ɫ
void Dialog::setColorMin(const QColor color)
{
    ColorMin=color;
    QString qss=QString("#MinButton{border-image:url(:/Images/min.ico);border-radius:5px;background: rgba(%1, %2, %3, %4);}#MinButton:pressed{border-image:url(:/Images/min_press.bmp);}").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    ui->MinButton->setStyleSheet(qss);
}
//--------------------��������ť��������---------------------//

//----------------------�����������------------------------//
//���ڰ�ť�¼�
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
    ui->StateLable->setText(QString::fromLocal8Bit("״̬������������"));
    ui->StateLable->setStyleSheet(R"(font: 75 18pt "Microsoft YaHei UI";)");
    //��������ݶ�ȡ��array��
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
    //��ʼ���
    board->setPuzzle(puzzle);
    if(solver->check(board->getPuzzle())){
        solver->solve(board->getPuzzle());
    }else{
        ui->StateLable->setText(QString::fromLocal8Bit("״̬:�������޽�"));
        ui->StateLable->setStyleSheet(R"(font: 75 18pt "Microsoft YaHei UI";color:rgb(255, 0, 0);)");
        setButtonDisable(false);
        return;
    }
    for(size_t i=0;i<9;++i){
        for(size_t j=0;j<9;++j){
            ui->Grid->item(i,j)->setForeground(QBrush(Qt::darkGreen));
        }
    }
    ui->StateLable->setText(QString::fromLocal8Bit("״̬���������"));
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
    ui->StateLable->setText(QString::fromLocal8Bit("״̬���ȴ�����"));
    ui->StateLable->setStyleSheet(R"(font: 75 18pt "Microsoft YaHei UI";)");
}

void Dialog::on_FileIn_clicked()
{
    if(!board->puzzleInput()){
        ui->StateLable->setText(QString::fromLocal8Bit("״̬���ļ�����ʧ��"));
        ui->StateLable->setStyleSheet(R"(font: 75 18pt "Microsoft YaHei UI";color:rgb(255, 0, 0);)");
    }else{
        ui->StateLable->setText(QString::fromLocal8Bit("״̬���ļ�����ɹ�"));
        ui->StateLable->setStyleSheet(R"(font: 75 18pt "Microsoft YaHei UI";color:rgb(14, 135, 14);)");
        changeAll(board->getPuzzle());
    }
}

void Dialog::on_FileOut_clicked()
{
    if(!solver->check(board->getPuzzle())){
        ui->StateLable->setText(QString::fromLocal8Bit("״̬:�������޽�"));
        ui->StateLable->setStyleSheet(R"(font: 75 18pt "Microsoft YaHei UI";color:rgb(255, 0, 0);)");
        return;
    }
    if(!board->puzzleOutPut()){
        ui->StateLable->setText(QString::fromLocal8Bit("״̬���ļ����ʧ��"));
        ui->StateLable->setStyleSheet(R"(font: 75 18pt "Microsoft YaHei UI";color:rgb(255, 0, 0);)");
    }else{
        ui->StateLable->setText(QString::fromLocal8Bit("״̬���ļ�����ɹ�"));
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
//--------------------���������������-----------------------//

//------------------------�ۺ���---------------------------//
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
//-----------------------�ۺ�������-------------------------//
