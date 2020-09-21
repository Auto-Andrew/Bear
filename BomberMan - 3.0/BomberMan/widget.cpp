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
#include <QTimer>
#include <QImage>

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

    /*使小怪物不断移动*/
    /*碰到炸弹也会返回噢*/
    /*有些怪物可以穿过砖块噢*/
    connect(timer, &QTimer::timeout,
            [=]()
            {
                for(int i=0;i<monsters;i++)
                {
                    if(mons[i].state==0)continue;
                    switch(mons[i].dir){
                        case 0:
                            if(map[(mons[i].y-mons[i].speed-oriY)/hh][(mons[i].x-oriX)/ww]==0&&map[(mons[i].y-mons[i].speed-oriY)/hh][(mons[i].x-oriX+mons[i].width)/ww]==0)
                            {
                                mons[i].img_x++;
                                mons[i].img_y=3;
                                if(mons[i].img_x>=4)mons[i].img_x=0;
                                mons[i].y-=mons[i].speed;
                            }else{
                                mons[i].dir=2;
                            }
                            break;
                        case 1:
                            if(map[(mons[i].y-oriY)/hh][(mons[i].x-mons[i].speed-oriX)/ww]==0&&map[(mons[i].y+mons[i].height-oriY)/hh][(mons[i].x-mons[i].speed-oriX)/ww]==0)
                            {
                                mons[i].img_x++;
                                mons[i].img_y=1;
                                if(mons[i].img_x>=4)mons[i].img_x=0;
                                mons[i].x-=mons[i].speed;
                            }else{
                                mons[i].dir=3;
                            }
                            break;
                        case 2:
                            if(map[(mons[i].y+mons[i].speed+mons[i].height-oriY)/hh][(mons[i].x-oriX)/ww]==0&&map[(mons[i].y+mons[i].speed+mons[i].height-oriY)/hh][(mons[i].x-oriX+mons[i].width)/ww]==0)
                            {
                                mons[i].img_x++;
                                mons[i].img_y=0;
                                if(mons[i].img_x>=4)mons[i].img_x=0;
                                mons[i].y+=mons[i].speed;
                            }else{
                                mons[i].dir=0;
                            }
                            break;
                        case 3:
                            if(map[(mons[i].y-oriY)/hh][(mons[i].x+mons[i].speed+mons[i].width-oriX)/ww]==0&&map[(mons[i].y+mons[i].height-oriY)/hh][(mons[i].x-oriX+mons[i].speed+mons[i].width)/ww]==0)
                            {
                                mons[i].img_x++;
                                mons[i].img_y=2;
                                if(mons[i].img_x>=4)mons[i].img_x=0;
                                mons[i].x+=mons[i].speed;
                            }else{
                                mons[i].dir=1;
                            }
                            break;
                        default:
                            break;
                    }

              }
              update();
        }
        );
    /*尝试全局每0.3秒刷新一下*/
    //connect(intervalTimer, &QTimer::timeout, [=](){update();});
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setMap()
{
    /*初始化 墙 砖块 怪物 主角 道具*/
    int cnt=0;
    int x, y;
    int tBProps=0, tMProps=0, tSProps=0, tFProps=0;
    while(cnt<walls){
        x=qrand()%(h-2)+1;
        y=qrand()%(w-2)+1;
        if(map[x][y]==0&&(x+y>4)){
            if(num==2&&x+y>=w+h-6)continue;
            map[x][y]=2;
            cnt++;

            /*道具的生成有问题 bug*/
            /*出口*/
            if(cnt==1){
                odor.x=x;
                odor.y=y;
                odor.state=1;
            }
            /*道具*/
            if(cnt>1&&tBProps<boom_props){
                bpro[tBProps].x=x;
                bpro[tBProps].y=y;
                bpro[tBProps].state=1;
                tBProps++;
            }
            if(cnt>(1+boom_props)&&tMProps<mult_props){
                mpro[tMProps].x=x;
                mpro[tMProps].y=y;
                mpro[tMProps].state=1;
                tMProps++;

            }
            if(cnt>(1+boom_props+mult_props)&&tFProps<fast_props){
                fpro[tFProps].x=x;
                fpro[tFProps].y=y;
                fpro[tFProps].state=1;
                tFProps++;
            }
            if(cnt>(1+boom_props+mult_props+fast_props)&&tSProps<stay_props){
                spro[tSProps].x=x;
                spro[tSProps].y=y;
                spro[tSProps].state=1;
                tSProps++;
            }
        }
    }   

    qDebug()<<mpro[0].x<<' '<<mpro[0].y;
    qDebug()<<bpro[0].x<<' '<<bpro[0].y;
    qDebug()<<bpro[1].x<<' '<<bpro[1].y;

    /*主角*/
    if(num>=1){
        p1.x=oriX+1*ww;
        p1.y=oriY+1*hh;
        p1.boom=0;
        p1.fast=0;
        p1.mult=1;
        p1.stay=0;
        p1.bomb=0;//已放置炸弹数
        p1.img_x=0;
        p1.img_y=0;
        p1.width=25;
        p1.height=38;
        p1.state=1;
        p1.character=0;
        p1.speed=6;
    }
    if(num==2){
        p2.x=oriX+(w-2)*ww;
        p2.y=oriY+(h-2)*hh;
        p2.boom=0;
        p2.fast=0;
        p2.mult=1;
        p2.stay=0;
        p2.bomb=0;
        p2.img_x=0;
        p2.img_y=0;
        p2.width=25;
        p2.height=38;
        p2.state=1;
        p2.character=0;
        p2.speed=6;
        //qDebug()<<p2.x<<' '<<p2.y;
    }

    /*小怪物*/
    cnt=0;
    while(cnt<monsters){
        x=qrand()%(h-2)+1;
        y=qrand()%(w-2)+1;
        if(map[x][y]!=0)continue;
        mons[cnt].x=y*ww+oriX;
        mons[cnt].y=x*hh+oriY;
        mons[cnt].kind=qrand()%4;
        mons[cnt].state=1;
        mons[cnt].speed=4;
        mons[cnt].img_x=0;
        mons[cnt].img_y=0;
        mons[cnt].width=40;
        mons[cnt].height=40;
        mons[cnt].dir=qrand()%4;
        cnt++;
    }

}

void Widget::dealGame(int n)
{
    if(n==1||n==2){
        if(n==1){
            if(num==1){
                QMessageBox::about(this, "MES", "游戏失败");
            }else if(num==2){
                QMessageBox::about(this, "MES", "游戏结束\np2玩家胜利！");
            }
        }
        else if(n==2){
            QMessageBox::about(this, "MES", "游戏结束\np1玩家胜利！");
        }
        /*数据清空*/
        flag=0;
        ui->stack->setCurrentIndex(0);
        start=1;
        timer->stop();
    }
}

bool Widget::isBrick(int r, int c)
{
    if(map[r][c]==2)return true;
    return false;
}

/*是怪物 返回序号i 不是怪物 返回-1*/
int Widget::isMonster(int r, int c)
{
    for(int i=0;i<monsters;i++){
        if(mons[i].state==0)continue;
        int rr=(mons[i].y-oriY)/hh;
        int cc=(mons[i].x-oriX)/ww;
        if(rr==r&&cc==c)
            return i;
    }
    return -1;
}

/*是炸弹 返回序号i 不是炸弹 返回-1*/
int Widget::isBomb(int r, int c)
{
    for(int i=0;i<bombs;i++){
        if(bomb[i].state==0)continue;
        if(bomb[i].c==c&&bomb[i].r==r)
            return i;
    }
    return -1;
}

/*是玩家 返回玩家号 不是玩家 返回0*/
int Widget::isPlayer(int r, int c)
{
    int rr=(p1.y-oriY)/hh;
    int cc=(p1.x-oriX)/ww;
    if(rr==r&&cc==c)return 1;

    if(num==1)return 0;

    rr=(p2.y-oriY)/hh;
    cc=(p2.x-oriX)/ww;
    if(rr==r&&cc==c)return 2;

    return 0;
}

/*是空地 且没有炸弹放置*/
bool Widget::isEmpty(int r, int c)
{
    if(map[r][c]!=0)return false;
    for(int i=0;i<bombs;i++){
        if(bomb[i].state==0)continue;
        if(bomb[i].c==c&&bomb[i].r==r)
            return false;
    }
    return true;
}

bool Widget::NoneMonster()
{
    for(int i=0;i<monsters;i++){
        if(mons[i].state==1)
            return false;
    }
    return true;
}

void Widget::paintEvent(QPaintEvent *)
{
    if(flag==0)return;

    QPainter painter(this);

    //qDebug()<<p2.x<<' '<<p2.y;

    painter.setPen(Qt::transparent);

    /*初始化地图*/
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            if(map[i][j]==1)
                painter.setBrush(Qt::gray);
            else if(map[i][j]==2)
                painter.setBrush(Qt::yellow);
            else
                painter.setBrush(Qt::white);
            painter.drawRect(ww*j+oriX, hh*i+oriY, ww, hh);
        }
    }


    /*人物踩了道具之后的效果有问题 bug*/
    /*道具+出口*/
    for(int i=0;i<boom_props;i++){
        if(bpro[i].state==0)continue;
        int r=bpro[i].x;
        int c=bpro[i].y;
        if(map[r][c]==0)
            painter.drawPixmap(c*ww+oriX, r*hh+oriY, ww, hh, QPixmap("../boom.png"));
        int NO=isPlayer(r, c);
        if(NO==1){
            p1.boom++;
            bpro[i].state=0;
        }
        else if(NO==2){
            p2.boom++;
            bpro[i].state=0;
        }
    }

    for(int i=0;i<fast_props;i++){
        if(fpro[i].state==0)continue;
        int r=fpro[i].x;
        int c=fpro[i].y;
        if(map[r][c]==0)
            painter.drawPixmap(c*ww+oriX, r*hh+oriY, ww, hh, QPixmap("../fast.png"));
        int NO=isPlayer(r, c);
        if(NO==1){
            p1.speed++;
            fpro[i].state=0;
        }
        else if(NO==2){
            p2.speed++;
            fpro[i].state=0;
        }
    }

    for(int i=0;i<stay_props;i++){
        if(spro[i].state==0)continue;
        int r=spro[i].x;
        int c=spro[i].y;
        if(map[r][c]==0)
            painter.drawPixmap(spro[i].y*ww+oriX, spro[i].x*hh+oriY, ww, hh, QPixmap("../stay.png"));
        int NO=isPlayer(r ,c);
        if(NO==1){
            p1.stay=1;
            spro[i].state=0;
        }
        else if(NO==2){
            p2.stay=1;
            spro[i].state=0;
        }
    }

    for(int i=0;i<mult_props;i++){
        if(mpro[i].state==0)continue;
        int r=mpro[i].x;
        int c=mpro[i].y;
        if(map[r][c]==0)
            painter.drawPixmap(c*ww+oriX, r*hh+oriY, ww, hh, QPixmap("../mult.png"));
        int NO=isPlayer(r, c);
        if(NO==1){
            p1.mult++;
            mpro[i].state=0;
        }
        else if(NO==2){
            p2.mult++;
            mpro[i].state=0;
        }
    }

    if(map[odor.x][odor.y]==0){
        painter.drawPixmap(odor.y*ww+oriX, odor.x*hh+oriY, ww, hh, QPixmap("../outdoor.png"));
        if(NoneMonster()){
            int NO=isPlayer(odor.x, odor.y);
            if(NO==1){
                    QMessageBox::about(this, "MES", "游戏胜利！");

            }
        }

    }
    /*判断主角在门上的时候是否赢得游戏 + 道具是否被主角吃到*/

    /*小怪物*/
    for(int i=0;i<monsters;i++){
        if(mons[i].state==0)continue;
        painter.drawPixmap(mons[i].x, mons[i].y, QPixmap("../pkq.png"), mons[i].img_x*mons[i].width, mons[i].img_y*mons[i].height, mons[i].width, mons[i].height);
        //qDebug()<<mons[i].x<<' '<<mons[i].y;
    }

    /*主角*/
    if(num>=1){
        painter.drawPixmap(p1.x, p1.y, QPixmap("../duora.png"), p1.img_x*p1.width, p1.img_y*p1.height, p1.width, p1.height);
    }
    if(num==2){
        painter.drawPixmap(p2.x, p2.y, QPixmap("../DaXiong.png"), p2.img_x*p2.width, p2.img_y*p2.height, p2.width, p2.height);
    }

    /*炸弹*/
    for(int i=0;i<bombs;i++){
        if(bomb[i].state==0)continue;//炸弹膨胀
        painter.drawPixmap(bomb[i].c*ww+oriX, bomb[i].r*hh+oriY, ww, hh, QPixmap("../bomb.png"), bomb[i].img_x*bomb[i].width, bomb[i].img_y*bomb[i].height, bomb[i].width, bomb[i].height);
        int NO=0;
        if(bomb[i].explode==1){//炸弹爆炸
            int r=bomb[i].r;
            int c=bomb[i].c;
            int dr, dc;

            dr=1;//up
            while((map[r-dr][c]==0||isBrick(r-dr, c))&&dr<=bomb[i].scale){
                //qDebug()<<(r-dr)<<' '<<c<<map[r-dr][c];
                if(NO==0)NO=isPlayer(r-dr, c);
                while(isMonster(r-dr, c)!=-1){
                    mons[isMonster(r-dr, c)].state=0;
                }
                painter.drawPixmap(c*ww+oriX, (r-dr)*hh+oriY, ww, hh, QPixmap("../bomb.png"), 6*bomb[i].width, 2*bomb[i].height, bomb[i].width, bomb[i].height);
                if(isBrick(r-dr, c)){
                    map[r-dr][c]=0;
                    break;
                }
                dr++;
            }

            dr=1;//down
            while((map[r+dr][c]==0||isBrick(r+dr, c))&&dr<=bomb[i].scale){
                if(NO==0)NO=isPlayer(r+dr, c);
                while(isMonster(r+dr, c)!=-1){
                    mons[isMonster(r+dr, c)].state=0;
                }
                painter.drawPixmap(c*ww+oriX, (r+dr)*hh+oriY, ww, hh, QPixmap("../bomb.png"), 8*bomb[i].width, 2*bomb[i].height, bomb[i].width, bomb[i].height);
                if(isBrick(r+dr, c)){
                    map[r+dr][c]=0;
                    break;
                }
                dr++;
            }

            dc=1;//left
            while((map[r][c-dc]==0||isBrick(r, c-dc))&&dc<=bomb[i].scale){
                if(NO==0)NO=isPlayer(r, c-dc);
                while(isMonster(r, c-dc)!=-1){
                    mons[isMonster(r, c-dc)].state=0;
                }
                painter.drawPixmap((c-dc)*ww+oriX, r*hh+oriY, ww, hh, QPixmap("../bomb.png"), 2*bomb[i].width, 2*bomb[i].height, bomb[i].width, bomb[i].height);
                if(isBrick(r, c-dc)){
                    map[r][c-dc]=0;
                    break;
                }
                dc++;
            }

            dc=1;//right
            while((map[r][c+dc]==0||isBrick(r, c+dc))&&dc<=bomb[i].scale){
                if(NO==0)NO=isPlayer(r, c+dc);
                while(isMonster(r, c+dc)!=-1){
                    mons[isMonster(r, c+dc)].state=0;
                }
                painter.drawPixmap((c+dc)*ww+oriX, r*hh+oriY, ww, hh, QPixmap("../bomb.png"), 4*bomb[i].width, 2*bomb[i].height, bomb[i].width, bomb[i].height);
                if(isBrick(r, c+dc)){
                    map[r][c+dc]=0;
                    break;
                }
                dc++;
            }
            //qDebug()<<"yeah";
            //这里如果主角死掉之后会刷两次 bug
            QTimer::singleShot(300, this, [=](){bomb[i].state=0;bomb[i].explode=0;dealGame(NO);});

            //if(bomb[i].owner==1)p1.bomb--;
            //else if(bomb[i].owner==2)p2.bomb--;
            //for(int i=0;i<monsters;i++)
            //    qDebug()<<mons[i].state;
        }
    }

}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
        case Qt::Key_Return://确认
        {
            if(flag==0){//开始游戏状态
                if(start==1){
                    ui->stack->setCurrentIndex(1);

                    w=31;
                    h=13;

                    oriX=0;
                    oriY=130;

                    hh=47;
                    ww=47;

                    //time=0;
                    //score=0;

                    walls=int(w*h*0.2);
                    monsters=8;
                    boom_props=3;
                    mult_props=1;
                    stay_props=1;
                    fast_props=1;
                    outdoors=1;

                    /*计时器记录游戏时长*/

                    /*初始化地图数据*/
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
                    timer->start(300);
                    update();
                }
                break;
            }

            break;
        }
        case Qt::Key_Up://p2+选择
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
            else if(flag==1&&num==2){

            }

            break;
        }
        case Qt::Key_Down://p2+选择
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
            else if(flag==1&&num==2){

            }

            break;
        }
        case Qt::Key_Left://p2
        {
            if(flag==1&&num==2){

            }
            break;
        }
        case Qt::Key_Right://p2
        {
            if(flag==1&&num==2){

            }
            break;
        }
        case Qt::Key_W://p1
        {
            if(flag==1&&p1.state==1){

                p1.img_x++;
                p1.img_y=3;
                if(p1.img_x>=4)p1.img_x=0;

                if(map[(p1.y-p1.speed-oriY)/hh][(p1.x-oriX)/ww]==0&&map[(p1.y-p1.speed-oriY)/hh][(p1.x-oriX+p1.width)/ww]==0){
                    p1.y-=p1.speed;
                }
            }
            break;
        }
        case Qt::Key_A://p1
        {
            if(flag==1&&p1.state==1){
                p1.img_x++;
                p1.img_y=1;
                if(p1.img_x>=4)p1.img_x=0;

                if(map[(p1.y-oriY+4)/hh][(p1.x-p1.speed-oriX)/ww]==0&&map[(p1.y+p1.height-oriY)/hh][(p1.x-p1.speed-oriX)/ww]==0){
                    p1.x-=p1.speed;
                }
            }
            break;
        }
        case Qt::Key_S://p1
        {
            if(flag==1&&p1.state==1){
                p1.img_x++;
                p1.img_y=0;
                if(p1.img_x>=4)p1.img_x=0;

                if(map[(p1.y+p1.speed+p1.height-oriY)/hh][(p1.x-oriX)/ww]==0&&map[(p1.y+p1.speed+p1.height-oriY)/hh][(p1.x-oriX+p1.width)/ww]==0){
                    p1.y+=p1.speed;
                }
            }
            break;
        }
        case Qt::Key_D://p1
        {
            if(flag==1&&p1.state==1){
                p1.img_x++;
                p1.img_y=2;
                if(p1.img_x>=4)p1.img_x=0;

                if(map[(p1.y-oriY+4)/hh][(p1.x+p1.speed+p1.width-oriX)/ww]==0&&map[(p1.y+p1.height-oriY)/hh][(p1.x-oriX+p1.speed+p1.width)/ww]==0){
                    p1.x+=p1.speed;
                }
            }
            break;
        }
        case Qt::Key_K://放炸弹p1
        {
            if(flag==1&&p1.state==1){
                int r=(p1.y+p1.height/2-oriY)/hh;
                int c=(p1.x+p1.width/2-oriX)/ww;
                int t=bombs;
                if(p1.bomb<p1.mult&&isEmpty(r, c)){
                    p1.bomb++;
                    bomb[bombs].r=r;
                    bomb[bombs].c=c;
                    bomb[bombs].scale=1+p1.boom;
                    bomb[bombs].explode=0;
                    bomb[bombs].state=1;
                    //bomb[bombs].owner=1;
                    QTimer::singleShot(1000, this, [=](){bomb[t].img_x=1;update();});
                    QTimer::singleShot(2000, this, [=](){bomb[t].img_x=0;update();});
                    QTimer::singleShot(3000, this, [=](){bomb[t].img_x=0;bomb[t].img_y=2;bomb[t].explode=1;p1.bomb--;update();});
                    bombs++;
                }
            }
            break;
        }
        case Qt::Key_3://放炸弹p2
        {
            if(flag==1&&p2.state==1&&num==2){
                int r=(p2.y+p2.height/2-oriY)/hh;
                int c=(p2.x+p2.width/2-oriX)/ww;
                int t=bombs;
                if(p2.bomb<p2.mult&&isEmpty(r, c)){
                    p2.bomb++;
                    bomb[bombs].r=r;
                    bomb[bombs].c=c;
                    bomb[bombs].scale=1+p2.boom;
                    bomb[bombs].explode=1;
                    bomb[bombs].state=1;
                    //bomb[bombs].owner=2;
                    QTimer::singleShot(1000, this, [=](){bomb[t].img_x=1;update();});
                    QTimer::singleShot(2000, this, [=](){bomb[t].img_x=0;update();});
                    QTimer::singleShot(3000, this, [=](){bomb[t].img_x=0;bomb[t].img_y=2;bomb[t].explode=1;p2.bomb--;update();});
                    bombs++;
                }
            }
            //qDebug()<<"yeah";
            break;
        }
        default:
            break;

    }
    update();
    /*是否可以设置为每隔0.3秒刷新一下*/
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(flag==1&&event->button()==Qt::LeftButton){
        ui->stack->setCurrentIndex(0);
        flag=0;
        start=0;
        timer->stop();
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

    timer->start(300);

    update();

}

void Widget::on_duet_clicked()
{
    ui->stack->setCurrentIndex(2);
    num=2;
    outdoors=0;
    setMap();
    flag=1;

    timer->start(300);

    update();

}
