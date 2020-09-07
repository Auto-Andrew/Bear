#include "widget.h"
#include "ui_widget.h"
#include <QTime>
#include <QWidget>
#include <QDebug>
#include <QTextEdit>
#include <QLineEdit>
#include <QString>
#include <QPixmap>
#include <QLabel>
#include <math.h>
#include <QPainter>
#include <QLabel>
#include <QFont>
#include <QTextBrowser>
#include <QKeyEvent>
#include <QComboBox>
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QRegExp regx("[1-9][0-9]+$");
    QValidator *validator = new QRegExpValidator(regx, ui->NUM);
    ui->NUM->setValidator(validator);

    ui->warning->hide();
    ui->voteBox->hide();
    ui->prompt->hide();
    ui->textBrowser->hide();
    ui->textEdit->hide();
    ui->submit->hide();
    ui->voteSubmit->hide();
    ui->eyeOpen->hide();

    ui->prompt->setFont(QFont("Microsoft YaHei", 17, -1, false));
    ui->prompt->setStyleSheet("color:#ff00ff");
    ui->prompt->setAlignment(Qt::AlignHCenter);


}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *)//绘制
{

    if(flag==0)return;

    if(pShow==1)
    ui->prompt->setText(QString::number(currentNO)+"号玩家开始发言");

    int px=350;
    int py=320;
    int R=240;
    int r=45;
    double angle=360.0/n;

    QPainter painter;
    painter.begin(this);
    painter.setFont(QFont("Microsoft YaHei", 18, QFont::Bold, false));

    for(int i=1;i<=n;i++)
    {

        if(p[i].state==0)
        {
            painter.setBrush(Qt::darkGray);
        }
        else
        {
            painter.setBrush(QColor(141,9,129));
            if(pShow==1&&i==currentNO)painter.setBrush(QColor(164,224,225));
        }

        painter.drawEllipse(QPoint(int(px+sin(angle*(i-1)/180*3.14)*R),int(py-cos(angle*(i-1)/180*3.14)*R)), r, r);
        painter.drawText(QRectF(int(px+sin(angle*(i-1)/180*3.14)*R-r/2),int(py-cos(angle*(i-1)/180*3.14)*R-r/2),r,r),QString::number(i),QTextOption(Qt::AlignCenter));

    }
    painter.end();
}

void Widget::SetCard()//发牌
{
    int a[100];
    for(int i=1;i<=n;i++)a[i]=i;
    for(int i=n;i>=1;i--)
    {
        srand((unsigned)time(NULL));
        //swap(a[i], a[rand()%i+1]);
        int t=rand()%i+1;
        int tt=a[i];
        a[i]=a[t];
        a[t]=tt;
    }
    for(int i=1;i<=n;i++)p[i].identity=a[i]%2;
}

void Widget::Vote()//投票
{
    ui->voteBox->clear();
    ui->voteBox->show();

    if(chFlag==1)//决定方向
    {
        ui->voteBox->addItem("Left");
        ui->voteBox->addItem("Right");
        return;
    }
    for(int i=1; i<=n; i++)//投出 移交
    {
        if(p[i].state==1)
        {
            ui->voteBox->addItem(QString::number(i));
        }
    }
    if(chFlag==0)ui->voteBox->addItem("0");//弃选项
}

void Widget::Deal(int k)//放逐+判断游戏是否结束
{
    ui->textBrowser->insertPlainText(QString::number(k)+"号玩家得到最多票数："+QString::number(p[k].voteNum)+"票 将被驱逐\n");
    ui->prompt->setText(QString::number(k)+"号玩家被驱逐！");
    ui->textBrowser->insertPlainText(QString::number(k)+"号玩家被驱逐！身份为：");
    //QMessageBox message(this);
    //message.setWindowTitle("PROMPT");
    //message.setText(QString::number(k)+"号玩家被驱逐！");
    //message.setGeometry(567,274,160,100);
    //about(this, "PROMPT", QString::number(k)+"号玩家被驱逐！");
    p[k].state=0;
    for(int i=1;i<=n;i++)
    {
        p[i].voteNum=0;
    }
    if(p[k].identity==0){//黑牌
        black--;
        ui->textBrowser->insertPlainText("黑牌！\n");
    }
    else{//红牌
        red--;
        ui->textBrowser->insertPlainText("红牌！\n");
    }
    if(red==0||black==0)//游戏结束
    {
#if 0
        QMessageBox GameEnd(this);
        GameEnd.setWindowTitle("GameOver");
        QString str="游戏结束\n";
        if(red==0)str+="黑方";
        else str+="红方";
        str+="获胜！！！";
        GameEnd.setText(str);
#endif
        qDebug()<<"gameover";
        this->close();
        return;
    }
    if(k!=head)
    {
        ui->voteBox->hide();
        ui->voteSubmit->hide();
        ui->submit->show();
        currentNO=NOShift(currentNO);
        chFlag=0;
        pShow=1;
        update();

    }else{
        ui->textBrowser->insertPlainText("被驱逐是村长！请村长移交下一任村长！\n");
        Vote();
        chFlag=2;
    }
}

void Widget::lastWords(int k)
{

}

int Widget::NOShift(int k)
{
    int i;
    if(right==1)
    {
        if(k==1)i=n;
        else i=k-1;
        while(p[i].state==0)
        {
            if(i==1)i=n;
            else i--;
        }
    }
    else
    {
        if(k==n)i=1;
        else i=k+1;
        while(p[i].state==0){
            if(i==n)i=1;
            else i++;
        }
    }
    return i;
}

void Widget::NightShift()
{
    ui->eyeOpen->show();
    srand((unsigned)time(NULL));
    if(rand()%2==0){
        ui->prompt->setText("进入黑夜");

    }else{
        ui->prompt->setText("进入红夜");

    }
}

void Widget::on_start_clicked()//开始游戏
{

    n = ui->NUM->text().toInt();
    if(n<4)
    {
        ui->warning->show();
        ui->NUM->clear();
        return ;
    }
    else
    {
        black=n/2;
        red=n-black;
        SetCard();
        ui->start->hide();
        ui->txt->hide();
        ui->NUM->hide();
        ui->warning->hide();

        flag=1;
        pShow=1;
        update();
        currentNO=1;

        ui->prompt->show();
        ui->textBrowser->show();
        ui->textEdit->show();
        ui->submit->show();

        ui->textBrowser->insertPlainText("开始游戏！！！\n从1号玩家开始轮流发言 选出村长！\n");
        return ;
    }

}

void Widget::on_submit_clicked()//提交发言
{
    ui->textBrowser->insertPlainText(QString::number(currentNO)+"号玩家："+"\n");
    ui->textBrowser->insertPlainText(ui->textEdit->toPlainText()+"\n");
    ui->textEdit->clear();
    if(flag==1)
    {
        currentNO++;
        if(currentNO==n+1)
        {
            currentNO=1;
            pShow=0;
            ui->prompt->setText("请开始投票选出村长！");
            ui->voteSubmit->show();
            ui->submit->hide();
            Vote();
        }
        else update();
    }
    else if(flag==2)
    {
        if(currentNO==head)
        {
            pShow=0;
            ui->prompt->setText("请开始投票进行驱逐！");
            ui->voteSubmit->show();
            ui->submit->hide();
            Vote();
            return;
        }
        currentNO=NOShift(currentNO);
        update();
    }

}

void Widget::on_voteSubmit_clicked()//提交选择
{
    if(chFlag==1)//村长选择发言方向
    {
        if(ui->voteBox->currentText()=="Left")
        {
            right=0;
            ui->textBrowser->insertPlainText("从村长左边开始发言!\n");
        }else
        {
            right=1;
            ui->textBrowser->insertPlainText("从村长右边开始发言!\n");
        }
        currentNO=NOShift(head);
        //天黑
        ui->voteBox->hide();
        ui->voteSubmit->hide();
        ui->submit->show();
        flag=2;
        chFlag=0;
        pShow=1;
        update();
        return;
    }else if(chFlag==2){//移交村长
        chFlag=1;
        head=ui->voteBox->currentText().toInt();
        double t=((black+red)%2==1?0.5:1.5);
        p[head].vote=t;
        ui->textBrowser->insertPlainText(QString::number(head)+"号玩家被移交为村长！ 投票时拥有"+QString::number(t)+"票\n");
        ui->prompt->setText("请村长决定发言顺序");
        Vote();
        return;
    }
    if(flag==1)
    {
        if(ui->voteBox->currentText()=="0")
        {
            ui->textBrowser->insertPlainText(QString::number(currentNO)+"号玩家：弃票\n");
        }else{
            ui->textBrowser->insertPlainText(QString::number(currentNO)+"号玩家投给："+ui->voteBox->currentText()+"号\n");
            int t=ui->voteBox->currentText().toInt();
            p[t].voteNum++;
        }
        currentNO++;
        Vote();
        if(currentNO==n+1)
        {
            int i=2;
            int maxVote=p[1].voteNum;
            int maxIndex=1;
            while(i<=n)
            {
                if(maxVote<p[i].voteNum)
                {
                    maxVote=p[i].voteNum;
                    maxIndex=i;
                }
                i++;
            }
            head=maxIndex;
            p[head].vote=(n%2==1?0.5:1.5);
            ui->textBrowser->insertPlainText(QString::number(maxIndex)+"号玩家得到最多票数："+QString::number(maxVote)+"票 成为村长 投票时拥有"+
                                             QString::number(n%2==1?0.5:1.5)+"票\n");
            ui->prompt->setText("请村长决定发言顺序");
            chFlag=1;
            for(i=0;i<=n;i++)p[i].voteNum=0;
            Vote();
        }
    }
    else if(flag==2)
    {
        if(ui->voteBox->currentText()=="0")
        {
            ui->textBrowser->insertPlainText(QString::number(currentNO)+"号玩家：弃票\n");
        }else{
            ui->textBrowser->insertPlainText(QString::number(currentNO)+"号玩家投给："+ui->voteBox->currentText()+"号\n");
            int t=ui->voteBox->currentText().toInt();
            p[t].voteNum+=p[currentNO].vote;
        }
        currentNO=NOShift(currentNO);
        Vote();
        if(currentNO==head)
        {
            int i=2;
            int maxVote=p[1].voteNum;
            int maxIndex=1;
            while(i<=n)
            {
                if(p[i].state==0)
                {
                    i++;
                    continue;
                }
                if(maxVote<p[i].voteNum)
                {
                    maxVote=p[i].voteNum;
                    maxIndex=i;
                }
                i++;
            }
            Deal(maxIndex);
        }
    }
}

void Widget::on_eyeOpen_clicked()
{
    p[currentNO].eye=1;

}
