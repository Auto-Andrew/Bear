#include "widget.h"
#include "ui_widget.h"
#include <QString>
#include <QMessageBox>
#include <QMovie>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTime>
#include <QPixmap>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    memset(map, 0, sizeof (map));

    qsrand(uint(QTime(0, 0, 0).secsTo(QTime::currentTime())));

    ui->startImage->setGeometry(0, 0, width(), height());
    ui->contImage->setGeometry(0, 0, width(), height());

    ui->stack->setCurrentIndex(0);

    ui->contImage->hide();
    ui->start->hide();
    ui->cont->hide();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::setMap()
{

    int cnt=0;
    int x, y;
    while(cnt<walls){
        x=qrand()%(h-2)+1;
        y=qrand()%(w-2)+1;
        if(map[x][y]==0&&(x+y>4)){
            if(num==2&&x+y>=w+h-6)continue;
            map[x][y]=2;
            if(cnt==0)
                map[x][y]=100;//outdoor
            if(cnt==1&&num==1)
                map[x][y]=11;//boom
            if(cnt==2&&num==1)
                map[x][y]=12;//mult
            if(cnt==3&&num==1)
                map[x][y]=13;//stay
            cnt++;
        }
    }   
    if(num==1)map[1][1]=-1;
    if(num==2)map[w-2][h-2]=-2;

    cnt=0;
    while(cnt<monsters){
        x=qrand()%(h-2)+1;
        y=qrand()%(w-2)+1;
        if(map[x][y]==0&&(x+y>4)){
            map[x][y]=qrand()%4+3;//4种怪物
            cnt++;
        }
    }

}

void Widget::paintEvent(QPaintEvent *)
{
    if(flag==0)return;

    QPainter painter(this);

    painter.setFont(QFont("华文新魏", 20));

    painter.drawText(275, 70, "TIME : "+QString::number(time));
    painter.drawText(600, 70, "SCORE : "+QString::number(score));

    int oriX=10;
    int oriY=130;

    int hh=32;
    int ww=32;

    painter.setPen(Qt::transparent);

    for(int i=0;i<h;i++){//初始化地图
        for(int j=0;j<w;j++){
            if(map[i][j]==1)
                painter.setBrush(Qt::gray);
            else if(map[i][j]==2||map[i][j]==100||(map[i][j]>=11&&map[i][j]<=13))
                painter.setBrush(Qt::yellow);
            else
                painter.setBrush(Qt::white);
            painter.drawRect(ww*j+oriX, hh*i+oriY, ww, hh);
        }
    }

    /*小怪物+主角*/

}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
        case Qt::Key_Return:
        {
            if(flag==0){
                if(start==1){
                    ui->stack->setCurrentIndex(1);

                    w=31;
                    h=13;

                    time=0;
                    score=0;

                    walls=int(w*h*0.2);
                    monsters=8;

                    /*计时器记录游戏时长*/

                    for(int i=0;i<h;i++){
                        for(int j=0;j<w;j++){
                            if(i%2==0&&j%2==0)
                                map[i][j]=1;
                            else if(i==0||j==0||i==12||j==30)
                                map[i][j]=1;
                            else
                                map[i][j]=0;
                        }
                    }

                }
                else if(start==0&&w!=0){
                    ui->stack->setCurrentIndex(2);
                    flag=1;
                    update();
                }
                break;
            }
            break;
        }
        case Qt::Key_Up:
        {
            if(flag==0){
                start=1-start;
                if(start==1){
                    ui->startImage->show();
                    ui->contImage->hide();

                }else{
                    ui->contImage->show();
                    ui->startImage->hide();
                }
                break;
            }

            break;
        }
        case Qt::Key_Down:
        {
            if(flag==0){
                start=1-start;
                if(start==1){
                    ui->startImage->show();
                    ui->contImage->hide();
                }else{
                    ui->contImage->show();
                    ui->startImage->hide();
                }
                break;
            }

            break;
        }
        case Qt::Key_Left:
        {

            break;
        }
        case Qt::Key_Right:
        {

            break;
        }
        case Qt::Key_W:
        {

            break;
        }
        case Qt::Key_A:
        {

            break;
        }
        case Qt::Key_S:
        {

            break;
        }
        case Qt::Key_D:
        {

            break;
        }

    }
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(flag==1&&event->button()==Qt::LeftButton){
        ui->stack->setCurrentIndex(0);
        flag=0;
        start=0;
        ui->startImage->hide();
        ui->contImage->show();
        update();
    }
}

void Widget::on_solo_clicked()
{
    ui->stack->setCurrentIndex(2);
    num=1;
    setMap();
    flag=1;
    update();
    for(int i=0;i<3;i++)p1.props[i]=0;
    p1.state=1;


}

void Widget::on_duet_clicked()
{
    ui->stack->setCurrentIndex(2);
    num=2;
    setMap();
    flag=1;
    update();
    for(int i=0;i<3;i++)p1.props[i]=1;
    for(int i=0;i<3;i++)p2.props[i]=1;
    p1.state=1;
    p2.state=1;


}
