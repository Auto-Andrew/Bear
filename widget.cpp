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

    ui->prompt->setFont(QFont("Microsoft YaHei", 17, -1, false));
    ui->prompt->setStyleSheet("color:#ff00ff");
    ui->prompt->setAlignment(Qt::AlignHCenter);

    black=n/2;
    red=n-n/2;

    //q.setTextBackgroundColor(QColor(285,33,67));
    //ui->textBrowser->hide();

    //connect(ui->start, &QPushButton::pressed, this, &Widget::on_start_clicked);


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

    //qDebug()<<n;

    for(int i=1;i<=n;i++)
    {

        if(p[i].state==0)
        {
            painter.setBrush(Qt::darkGray);
        }
        else
        {
            painter.setBrush(QColor(141,9,129));
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

    if(chFlag==1)
    {
        ui->voteBox->addItem("Left");
        ui->voteBox->addItem("Right");
        return;
    }

    for(int i=1; i<=n; i++)
    {
        if(p[i].state==1)
        {
            ui->voteBox->addItem(QString::number(i));
        }
    }
}

void Widget::Deal(int k)//放逐+判断游戏是否结束
{

}

void Widget::Shift()//白/夜切换
{

    srand((unsigned)time(NULL));
    if(rand()%2)//黑夜
    {

    }
    else//红夜
    {

    }

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
    if(chFlag==1)
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
        ui->textBrowser->insertPlainText("天亮了！\n");
        ui->voteBox->hide();
        ui->voteSubmit->hide();
        ui->submit->show();
        flag=2;
        chFlag=0;
        pShow=1;
        update();
        return;

    }
    if(flag==1)
    {
        ui->textBrowser->insertPlainText(QString::number(currentNO)+"号玩家投给："+ui->voteBox->currentText()+"号\n");
        currentNO++;
        int t=ui->voteBox->currentText().toInt();
        p[t].voteNum++;
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
            ui->prompt->setText("请村长选择发言顺序");
            ui->textBrowser->insertPlainText(QString::number(maxIndex)+"号玩家得到最多票数："+QString::number(maxVote)+"票 投票时拥有"+
                                             QString::number(n%2==1?0.5:1.5)+"票\n");
            chFlag=1;
            for(i=0;i<=n;i++)p[i].voteNum=0;
            Vote();
        }
    }
    else if(flag==2)
    {
        ui->textBrowser->insertPlainText(QString::number(currentNO)+"号玩家投给："+ui->voteBox->currentText()+"号\n");
        int t=ui->voteBox->currentText().toInt();
        p[t].voteNum+=p[currentNO].vote;
        currentNO=NOShift(currentNO);
        Vote();
        if(NOShift(currentNO)==head)
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
