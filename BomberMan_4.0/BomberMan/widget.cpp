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
#include <QIcon>
#include <QLabel>

/*基础功能及结构建立*//*2020.09.16~2020.09.19*/

/*剩余功能*//*2020.09.20*/
/*人物走动*//*已完成*/
/*摆怪物*//*已完成*/
/*怪物走动*//*已完成*/
/*摆炸弹*//*已完成*/

/*剩余功能*//*2020.09.21*/
/*连环炸*//*已完成*/
/*怪物碰到炸弹要返回*//*已完成*/
/*有些怪物可以穿过砖块*//*已完成*/
/*怪物弄死主角*//*已完成*/
/*p2copy*//*已完成*/

/*剩余功能*//*2020.09.22*/
/*怪物随机转向*//*已完成*/

/*待优化*/
/*走位精度太高*/
/*走位不流畅*//*已优化*/
/*怪物及主角死亡动画*/
/*炸弹爆炸连续性特效*//*已优化*/
/*UI + 音效*/
/*游戏结束时的数据及界面处理*//*已优化*/
/*怪物离家太近，出生便没了*//*已优化*/
/*炸弹炸怪物和主角的时候 + 以及怪物吃人的时候 + 主角吃道具 + 四角都要考虑*//*吃道具待优化*/
/*按键同时按下并响应*/
/*两个主角一起死怎么办*/
/*从暂停到重新游戏 数据重置问题*//*已优化*/
/*AI怪物偏向于往主角方向走*/

/*bug*/
/*人躲在炸弹中央炸不死*//*已干掉*//*在出现放完炸弹之后卡在炸弹处时不稳定*/
/*结束界面多次刷新并提示*//*已干掉*/
/*爆炸炸多了砖块*//*bug赢了 干不掉它了*/
/*怪物走在炸弹中间也炸不死了*//*已干掉*/
/*p2人物生成地点错误*//*已干掉*/
/*怪物走着走着突然消失*//*已干掉*/
/*道具生成地点重复*//*已干掉*/

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    memset(map, 0, sizeof (map));

    //qDebug()<<width()<<' '<<height();

    qsrand(uint(QTime(0, 0, 0).secsTo(QTime::currentTime())));

    ui->startImage->setGeometry(0, 0, width(), height());
    ui->contImage->setGeometry(0, 0, width(), height());

    ui->stack->setCurrentIndex(0);

    ui->contImage->hide();

    QMovie *movie1 = new QMovie("../bulbasaur.gif");
    QMovie *movie2 = new QMovie("../charmander.gif");
    QMovie *movie3 = new QMovie("../squirtle.gif");

    ui->label_17->setMovie(movie1);
    ui->label_18->setMovie(movie2);
    ui->label_19->setMovie(movie3);

    movie1->start();
    movie2->start();
    movie3->start();

    /*导入素材*/
    for(int i=0;i<3;i++)
        image[i].load("../ghost.png");
    for(int i=3;i<5;i++)
        image[i].load("../shilaimu.png");
    for(int i=5;i<7;i++)
        image[i].load("../abo.png");
    image[7].load("../badahu.png");

    /*使小怪物不断移动*/
    /*碰到炸弹也会返回噢*/
    /*有些怪物可以穿过砖块噢*/
    connect(timer, &QTimer::timeout,
            [=]()
            {
                for(int i=0;i<monsters;i++)
                {
                    if(mons[i].state==0)continue;

                    if(mons[i].transC!=0&&mons[i].transR!=0){//找到了转折点
                        switch(mons[i].dir){
                        case 0:
                            if(mons[i].y>=mons[i].transR*hh+oriY&&mons[i].y+mons[i].height<=mons[i].transR*hh+oriY+hh)
                            {
                                mons[i].transC=0;
                                mons[i].transR=0;
                                mons[i].dir=(qrand()%2)==1?1:3;
                            }
                            break;
                        case 1:
                            if(mons[i].x>=mons[i].transC*ww+oriX&&mons[i].x+mons[i].width<=mons[i].transC*ww+oriX+ww)
                            {
                                mons[i].transC=0;
                                mons[i].transR=0;
                                mons[i].dir=(qrand()%2)==1?2:0;
                            }
                            break;
                        case 2:
                            if(mons[i].y>=mons[i].transR*hh+oriY&&mons[i].y+mons[i].height<=mons[i].transR*hh+oriY+hh)
                            {
                                mons[i].transC=0;
                                mons[i].transR=0;
                                mons[i].dir=(qrand()%2)==1?1:3;
                            }
                            break;
                        case 3:
                            if(mons[i].x>=mons[i].transC*ww+oriX&&mons[i].x+mons[i].width<=mons[i].transC*ww+oriX+ww)
                            {
                                mons[i].transC=0;
                                mons[i].transR=0;
                                mons[i].dir=(qrand()%2)==1?2:0;
                            }
                            break;
                        default:
                            break;
                        }

                    }

                    switch(mons[i].dir){
                        case 0:
                            if(mons[i].kind!=0){
                                if(isEmpty((mons[i].y-mons[i].speed-oriY)/hh, (mons[i].x-oriX)/ww)&&isEmpty((mons[i].y-mons[i].speed-oriY)/hh, (mons[i].x-oriX+mons[i].width)/ww))
                                {
                                    mons[i].img_x++;
                                    mons[i].img_y=3;
                                    if(mons[i].img_x>=4)mons[i].img_x=0;
                                    mons[i].y-=mons[i].speed;
                                }else{
                                    mons[i].dir=2;
                                }
                            }
                            else if(mons[i].kind==0){
                                if((isEmpty((mons[i].y-mons[i].speed-oriY)/hh, (mons[i].x-oriX)/ww)&&isEmpty((mons[i].y-mons[i].speed-oriY)/hh, (mons[i].x-oriX+mons[i].width)/ww))||
                                        (map[(mons[i].y-mons[i].speed-oriY)/hh][(mons[i].x-oriX)/ww]==2&&map[(mons[i].y-mons[i].speed-oriY)/hh][(mons[i].x-oriX+mons[i].width)/ww]==2))
                                {
                                    mons[i].img_x++;
                                    mons[i].img_y=3;
                                    if(mons[i].img_x>=4)mons[i].img_x=0;
                                    mons[i].y-=mons[i].speed;
                                }else{
                                    mons[i].dir=2;
                                }
                            }

                            break;
                        case 1:
                            if(mons[i].kind!=0){
                                if(isEmpty((mons[i].y-oriY)/hh, (mons[i].x-mons[i].speed-oriX)/ww)&&isEmpty((mons[i].y+mons[i].height-oriY)/hh, (mons[i].x-mons[i].speed-oriX)/ww))
                                {
                                    mons[i].img_x++;
                                    mons[i].img_y=1;
                                    if(mons[i].img_x>=4)mons[i].img_x=0;
                                    mons[i].x-=mons[i].speed;
                                }else{
                                    mons[i].dir=3;
                                }

                            }
                            else if(mons[i].kind==0){
                                if((isEmpty((mons[i].y-oriY)/hh, (mons[i].x-mons[i].speed-oriX)/ww)&&isEmpty((mons[i].y+mons[i].height-oriY)/hh, (mons[i].x-mons[i].speed-oriX)/ww))||
                                        (map[(mons[i].y-oriY)/hh][(mons[i].x-mons[i].speed-oriX)/ww]==2&&map[(mons[i].y+mons[i].height-oriY)/hh][(mons[i].x-mons[i].speed-oriX)/ww]==2))
                                {
                                    mons[i].img_x++;
                                    mons[i].img_y=1;
                                    if(mons[i].img_x>=4)mons[i].img_x=0;
                                    mons[i].x-=mons[i].speed;
                                }else{
                                    mons[i].dir=3;
                                }
                            }

                            break;
                        case 2:
                            if(mons[i].kind!=0){
                                if(isEmpty((mons[i].y+mons[i].speed+mons[i].height-oriY)/hh, (mons[i].x-oriX)/ww)&&isEmpty((mons[i].y+mons[i].speed+mons[i].height-oriY)/hh, (mons[i].x-oriX+mons[i].width)/ww))
                                {
                                    mons[i].img_x++;
                                    mons[i].img_y=0;
                                    if(mons[i].img_x>=4)mons[i].img_x=0;
                                    mons[i].y+=mons[i].speed;
                                }else{
                                    mons[i].dir=0;
                                }

                            }
                            else if(mons[i].kind==0){
                                if((isEmpty((mons[i].y+mons[i].speed+mons[i].height-oriY)/hh, (mons[i].x-oriX)/ww)&&isEmpty((mons[i].y+mons[i].speed+mons[i].height-oriY)/hh, (mons[i].x-oriX+mons[i].width)/ww))||
                                        (map[(mons[i].y+mons[i].speed+mons[i].height-oriY)/hh][(mons[i].x-oriX)/ww]==2&&map[(mons[i].y+mons[i].speed+mons[i].height-oriY)/hh][(mons[i].x-oriX+mons[i].width)/ww]==2))
                                {
                                    mons[i].img_x++;
                                    mons[i].img_y=0;
                                    if(mons[i].img_x>=4)mons[i].img_x=0;
                                    mons[i].y+=mons[i].speed;
                                }else{
                                    mons[i].dir=0;
                                }
                            }

                            break;
                        case 3:
                            if(mons[i].kind!=0){
                                if(isEmpty((mons[i].y-oriY)/hh, (mons[i].x+mons[i].speed+mons[i].width-oriX)/ww)&&isEmpty((mons[i].y+mons[i].height-oriY)/hh, (mons[i].x-oriX+mons[i].speed+mons[i].width)/ww))
                                {
                                    mons[i].img_x++;
                                    mons[i].img_y=2;
                                    if(mons[i].img_x>=4)mons[i].img_x=0;
                                    mons[i].x+=mons[i].speed;
                                }else{
                                    mons[i].dir=1;
                                }
                            }
                            else if(mons[i].kind==0){
                                if((isEmpty((mons[i].y-oriY)/hh, (mons[i].x+mons[i].speed+mons[i].width-oriX)/ww)&&isEmpty((mons[i].y+mons[i].height-oriY)/hh, (mons[i].x-oriX+mons[i].speed+mons[i].width)/ww))||
                                        (map[(mons[i].y-oriY)/hh][(mons[i].x+mons[i].speed+mons[i].width-oriX)/ww]==2&&map[(mons[i].y+mons[i].height-oriY)/hh][(mons[i].x-oriX+mons[i].speed+mons[i].width)/ww]==2))
                                {
                                    mons[i].img_x++;
                                    mons[i].img_y=2;
                                    if(mons[i].img_x>=4)mons[i].img_x=0;
                                    mons[i].x+=mons[i].speed;
                                }else{
                                    mons[i].dir=1;
                                }
                            }

                            break;
                        default:
                            break;
                    }

              }
              dealGame(NO);
              NO=0;
              update();
        }
        );

    connect(intervalTimer, &QTimer::timeout, [=](){intervalTimer->stop();intervalTimer->start(qrand()%1000+300);dirTrans();});

    connect(updateTimer, &QTimer::timeout, [=](){
        if(pressedKeys.isEmpty())
        {
            updateTimer->stop();
            return ;
        }
        for(int key:pressedKeys)
        {
            switch(key){
                case Qt::Key_Return://确认
                {
                    if(flag==0&&ui->stack->currentIndex()==0){//开始游戏状态
                        if(start==1){
                            if(pause==1){
                                p1.clear();
                                p2.clear();
                                if(mons!=nullptr)
                                delete []mons;
                                if(bomb!=nullptr)
                                delete []bomb;
                                if(fpro!=nullptr)
                                delete []fpro;
                                if(bpro!=nullptr)
                                delete []bpro;
                                if(spro!=nullptr)
                                delete []spro;
                                if(mpro!=nullptr)
                                delete []mpro;
                                pause=0;
                                p1choose=-1;
                                p2choose=-1;
                                pressedKeys.clear();
                            }

                            ui->stack->setCurrentIndex(1);

                            w=31;
                            h=13;

                            oriX=0;
                            oriY=200;

                            circle1x=78;
                            circle1y=348;
                            circle2x=78;
                            circle2y=597;

                            ui->circleP1->move(circle1x, circle1y);
                            ui->circleP2->move(circle2x, circle2y);

                            hh=47;
                            ww=47;

                            walls=int(w*h*0.2);
                            monsters=8;
                            boom_props=3;
                            mult_props=1;
                            stay_props=1;
                            fast_props=1;
                            outdoors=1;

                            bombs=0;
                            NO=0;

                            mons = new monster[10];
                            bomb = new bombS[60];
                            fpro = new fastProp[10];
                            spro = new stayProp[10];
                            bpro = new boomProp[10];
                            mpro = new multProp[10];

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
                            intervalTimer->start(8000);
                            update();
                        }
                        break;
                    }
                    else if(flag==0&&ui->stack->currentIndex()==1)//选角色状态
                    {
                        if(p2choose==-1&&p1choose!=-1)//单人
                        {
                            ui->stack->setCurrentIndex(2);
                            num=1;
                            outdoors=1;
                            setMap();
                            flag=1;

                            timer->start(300);
                            intervalTimer->start(8000);

                            update();
                        }else if(p1choose!=-1&&p2choose!=-1)//双人
                        {
                            ui->stack->setCurrentIndex(2);
                            num=2;
                            outdoors=0;
                            setMap();

                            flag=1;

                            timer->start(300);
                            intervalTimer->start(8000);
                            update();
                        }
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
                        p2.img_x++;
                        p2.img_y=3;
                        if(p2.img_x>=4)p2.img_x=0;

                        if(p2.stay==1){
                            if(map[(p2.y-p2.speed-oriY)/hh][(p2.x-oriX)/ww]==0&&map[(p2.y-p2.speed-oriY)/hh][(p2.x-oriX+p2.width)/ww]==0){
                                p2.y-=p2.speed;
                            }
                        }else if(p2.stay==0){
                            if((isEmpty((p2.y-p2.speed-oriY)/hh, (p2.x-oriX)/ww)&&isEmpty((p2.y-p2.speed-oriY)/hh, (p2.x-oriX+p2.width)/ww))||inBomb(p2.x, p2.y)){
                                p2.y-=p2.speed;
                            }
                        }
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
                        p2.img_x++;
                        p2.img_y=0;
                        if(p2.img_x>=4)p2.img_x=0;

                        if(p2.stay==1){
                            if(map[(p2.y+p2.speed+p2.height-oriY)/hh][(p2.x-oriX)/ww]==0&&map[(p2.y+p2.speed+p2.height-oriY)/hh][(p2.x-oriX+p2.width)/ww]==0){
                                p2.y+=p2.speed;
                            }
                        }else if(p2.stay==0){
                            if((isEmpty((p2.y+p2.speed+p2.height-oriY)/hh, (p2.x-oriX)/ww)&&isEmpty((p2.y+p2.speed+p2.height-oriY)/hh, (p2.x-oriX+p2.width)/ww))||inBomb(p2.x, p2.y)){
                                p2.y+=p2.speed;
                            }
                        }
                    }

                    break;
                }
                case Qt::Key_Left://p2
                {

                    if(flag==1&&num==2){
                        p2.img_x++;
                        p2.img_y=1;
                        if(p2.img_x>=4)p2.img_x=0;

                        if(p2.stay==1){
                            if(map[(p2.y-oriY+4)/hh][(p2.x-p2.speed-oriX)/ww]==0&&map[(p2.y+p2.height-oriY)/hh][(p2.x-p2.speed-oriX)/ww]==0){
                                p2.x-=p2.speed;
                            }
                        }else if(p2.stay==0){
                            if((isEmpty((p2.y-oriY+4)/hh, (p2.x-p2.speed-oriX)/ww)&&isEmpty((p2.y+p2.height-oriY)/hh, (p2.x-p2.speed-oriX)/ww))||inBomb(p2.x, p2.y)){
                                p2.x-=p2.speed;
                            }
                        }
                    }
                    else if(flag==0&&ui->stack->currentIndex()==1){
                        if(p2choose>-1){
                            p2choose--;
                            circle2x-=230;
                            ui->circleP2->move(circle2x, circle2y);
                        }
                    }
                    break;
                }
                case Qt::Key_Right://p2
                {
                    if(flag==1&&num==2){
                        p2.img_x++;
                        p2.img_y=2;
                        if(p2.img_x>=4)p2.img_x=0;

                        if(p2.stay==1){
                            if(map[(p2.y-oriY+4)/hh][(p2.x+p2.speed+p2.width-oriX)/ww]==0&&map[(p2.y+p2.height-oriY)/hh][(p2.x-oriX+p2.speed+p2.width)/ww]==0){
                                p2.x+=p2.speed;
                            }
                        }else if(p2.stay==0){
                            if((isEmpty((p2.y-oriY+4)/hh, (p2.x+p2.speed+p2.width-oriX)/ww)&&isEmpty((p2.y+p2.height-oriY)/hh, (p2.x-oriX+p2.speed+p2.width)/ww))||inBomb(p2.x, p2.y)){
                                p2.x+=p2.speed;
                            }
                        }
                    }
                    else if(flag==0&&ui->stack->currentIndex()==1){
                        if(p2choose<4){
                            p2choose++;
                            circle2x+=230;
                            ui->circleP2->move(circle2x, circle2y);
                        }
                    }
                    break;
                }
                case Qt::Key_W://p1
                {
                    if(flag==1&&p1.state==1){

                        p1.img_x++;
                        p1.img_y=3;
                        if(p1.img_x>=4)p1.img_x=0;

                        if(p1.stay==1){
                            if(map[(p1.y-p1.speed-oriY)/hh][(p1.x-oriX)/ww]==0&&map[(p1.y-p1.speed-oriY)/hh][(p1.x-oriX+p1.width)/ww]==0){
                                p1.y-=p1.speed;
                            }
                        }else if(p1.stay==0){
                            if((isEmpty((p1.y-p1.speed-oriY)/hh, (p1.x-oriX)/ww)&&isEmpty((p1.y-p1.speed-oriY)/hh, (p1.x-oriX+p1.width)/ww))||inBomb(p1.x, p1.y)){
                                p1.y-=p1.speed;
                            }
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

                        if(p1.stay==1){
                            if(map[(p1.y-oriY+4)/hh][(p1.x-p1.speed-oriX)/ww]==0&&map[(p1.y+p1.height-oriY)/hh][(p1.x-p1.speed-oriX)/ww]==0){
                                p1.x-=p1.speed;
                            }
                        }else if(p1.stay==0){
                            if((isEmpty((p1.y-oriY+4)/hh, (p1.x-p1.speed-oriX)/ww)&&isEmpty((p1.y+p1.height-oriY)/hh, (p1.x-p1.speed-oriX)/ww))||inBomb(p1.x, p1.y)){
                                p1.x-=p1.speed;
                            }
                        }
                    }
                    else if(flag==0&&ui->stack->currentIndex()==1){
                        if(p1choose>-1){
                            p1choose--;
                            circle1x-=230;
                            ui->circleP1->move(circle1x, circle1y);
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

                        if(p1.stay==1){
                            if(map[(p1.y+p1.speed+p1.height-oriY)/hh][(p1.x-oriX)/ww]==0&&map[(p1.y+p1.speed+p1.height-oriY)/hh][(p1.x-oriX+p1.width)/ww]==0){
                                p1.y+=p1.speed;
                            }
                        }else if(p1.stay==0){
                            if((isEmpty((p1.y+p1.speed+p1.height-oriY)/hh, (p1.x-oriX)/ww)&&isEmpty((p1.y+p1.speed+p1.height-oriY)/hh, (p1.x-oriX+p1.width)/ww))||inBomb(p1.x, p1.y)){
                                p1.y+=p1.speed;
                            }
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

                        if(p1.stay==1){
                            if(map[(p1.y-oriY+4)/hh][(p1.x+p1.speed+p1.width-oriX)/ww]==0&&map[(p1.y+p1.height-oriY)/hh][(p1.x-oriX+p1.speed+p1.width)/ww]==0){
                                p1.x+=p1.speed;
                            }
                        }else if(p1.stay==0){
                            if((isEmpty((p1.y-oriY+4)/hh, (p1.x+p1.speed+p1.width-oriX)/ww)&&isEmpty((p1.y+p1.height-oriY)/hh, (p1.x-oriX+p1.speed+p1.width)/ww))||inBomb(p1.x, p1.y)){
                                p1.x+=p1.speed;
                            }
                        }

                    }
                    else if(flag==0&&ui->stack->currentIndex()==1){
                        if(p1choose<4){
                            p1choose++;
                            circle1x+=230;
                            ui->circleP1->move(circle1x, circle1y);
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

                            //qDebug()<<bomb[bombs].r<<' '<<bomb[bombs].c;
                            //qDebug()<<(bomb[bombs].c*ww+oriX)<<' '<<(bomb[bombs].c*ww+oriX+ww);
                            //qDebug()<<(bomb[bombs].r*hh+oriY)<<' '<<(bomb[bombs].r*hh+oriY+hh);
                            //qDebug()<<p1.x<<' '<<p1.y;

                            //bomb[bombs].owner=1;
                            QTimer::singleShot(1000, this, [=](){bomb[t].img_x=1;if(bomb[t].explode==0)update();});
                            QTimer::singleShot(2000, this, [=](){bomb[t].img_x=0;if(bomb[t].explode==0)update();});
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
                            bomb[bombs].explode=0;
                            bomb[bombs].state=1;
                            //bomb[bombs].owner=2;
                            QTimer::singleShot(1000, this, [=](){bomb[t].img_x=1;if(bomb[t].explode==0)update();});
                            QTimer::singleShot(2000, this, [=](){bomb[t].img_x=0;if(bomb[t].explode==0)update();});
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
        }
        update();
    });
}

Widget::~Widget()
{
    delete ui;
}

/*初始化地图*/
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

            /*道具的生成有问题 bug*//*已修复*/
            /*出口*/
            if(cnt==1&&outdoors==1){
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

    //qDebug()<<mpro[0].x<<' '<<mpro[0].y;
    //qDebug()<<bpro[0].x<<' '<<bpro[0].y;
    //qDebug()<<spro[0].x<<' '<<spro[0].y;
    //if(num==1)qDebug()<<odor.x<<odor.y;

    /*主角*/
    if(num>=1){
        p1.x=oriX+1*ww;
        p1.y=oriY+1*hh;
        p1.boom=0;//爆炸范围增量
        p1.fast=0;//加速
        p1.mult=1;//可放置炸弹数
        p1.stay=0;//可穿过炸弹
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
        if(map[x][y]!=0||x+y<7||x+y>w+h-7)continue;
        mons[cnt].x=y*ww+oriX;
        mons[cnt].y=x*hh+oriY;
        mons[cnt].transC=0;
        mons[cnt].transR=0;
        if(cnt>=0&&cnt<2)
            mons[cnt].kind=0;//技能
        else
            mons[cnt].kind=1;
        mons[cnt].state=1;
        if(cnt==7)
            mons[cnt].speed=5;
        else if(cnt==5||cnt==6)
            mons[cnt].speed=3;
        else mons[cnt].speed=4;
        mons[cnt].img_x=0;
        mons[cnt].img_y=0;
        mons[cnt].width=40;
        mons[cnt].height=40;
        mons[cnt].dir=qrand()%4;//行走方向
        cnt++;
    }

}

/*结束了吗*/
void Widget::dealGame(int n)
{
    if(n==1||n==2){
        QMessageBox mes(this);
        mes.setTextFormat(Qt::RichText);
        if(n==1)
        {
            if(num==1){
                mes.about(this, "MES", "<b>游戏<font color='red'>失败</font>!</b>");
            }else if(num==2){
                mes.about(this, "MES", "<b><p>游戏结束</p><font color='red'>p2</font>玩家<font color='red'>胜利</font>！</b>");
            }
        }
        else if(n==2)
        {
            mes.about(this, "MES", "<b><p>游戏结束</p><font color='red'>p1</font>玩家<font color='red'>胜利</font>！</b>");
        }
        /*数据清空*/
        w=0;
        h=0;
        flag=0;
        p1choose=-1;
        p2choose=-1;
        ui->stack->setCurrentIndex(0);
        start=1;
        p1.clear();
        p2.clear();
        delete []mons;
        delete []bomb;
        delete []fpro;
        delete []bpro;
        delete []spro;
        delete []mpro;
        timer->stop();
        pressedKeys.clear();
    }
    else if(n==-1){
        QMessageBox mes(this);
        mes.setTextFormat(Qt::RichText);
        mes.about(this, "MES", "<b>游戏<font color='red'>胜利</font>！</b>");
        w=0;
        h=0;
        flag=0;
        p1choose=-1;
        p2choose=-1;
        ui->stack->setCurrentIndex(0);
        start=1;
        p1.clear();
        p2.clear();
        delete []mons;
        delete []bomb;
        delete []fpro;
        delete []bpro;
        delete []spro;
        delete []mpro;
        timer->stop();
        pressedKeys.clear();
    }
}

/*使怪物随机转向*/
void Widget::dirTrans()
{
    for(int i=0;i<monsters;i++)
    {
        if(mons[i].state==0)continue;
        int r=(mons[i].y-oriY)/hh;
        int c=(mons[i].x-oriX)/ww;
        if(mons[i].kind==0){//能穿墙
            if(mons[i].dir==0)
            {
                r--;
                while(r>0&&(isEmpty(r, c)||map[r][c]==2)){
                    if(isEmpty(r, c-1)||isEmpty(r, c+1)||map[r][c-1]==2||map[r][c+1]==2){
                        mons[i].transC=c;
                        mons[i].transR=r;
                        break;
                    }
                    r--;
                }
            }else if(mons[i].dir==1)
            {
                c--;
                while(c>0&&(isEmpty(r, c)||map[r][c]==2)){
                    if(isEmpty(r-1, c)||isEmpty(r+1, c)||map[r-1][c]==2||map[r+1][c]==2){
                        mons[i].transC=c;
                        mons[i].transR=r;
                        break;
                    }
                    c--;
                }
            }else if(mons[i].dir==2)
            {
                r++;
                while(r<h&&(isEmpty(r, c)||map[r][c]==2)){
                    if(isEmpty(r, c-1)||isEmpty(r, c+1)||map[r][c-1]==2||map[r][c+1]==2){
                        mons[i].transC=c;
                        mons[i].transR=r;
                        break;
                    }
                    r++;
                }
            }else
            {
                c++;
                while(c<w&&(isEmpty(r, c)||map[r][c]==2)){
                    if(isEmpty(r-1, c)||isEmpty(r+1, c)||map[r-1][c]==2||map[r+1][c]==2){
                        mons[i].transC=c;
                        mons[i].transR=r;
                        break;
                    }
                    c++;
                }
            }
        }
        else if(mons[i].kind!=0)
        {//不能穿墙
            if(mons[i].dir==0)
            {
                r--;
                while(r>0&&isEmpty(r, c)){
                    if(isEmpty(r, c-1)||isEmpty(r, c+1)){
                        mons[i].transC=c;
                        mons[i].transR=r;
                        break;
                    }
                    r--;
                }
            }else if(mons[i].dir==1)
            {
                c--;
                while(c>0&&isEmpty(r, c)){
                    if(isEmpty(r-1, c)||isEmpty(r+1, c)){
                        mons[i].transC=c;
                        mons[i].transR=r;
                        break;
                    }
                    c--;
                }
            }else if(mons[i].dir==2)
            {
                r++;
                while(r<h&&isEmpty(r, c)){
                    if(isEmpty(r, c-1)||isEmpty(r, c+1)){
                        mons[i].transC=c;
                        mons[i].transR=r;
                        break;
                    }
                    r++;
                }
            }else{
                c++;
                while(c<w&&isEmpty(r, c)){
                    if(isEmpty(r-1, c)||isEmpty(r+1, c)){
                        mons[i].transC=c;
                        mons[i].transR=r;
                        break;
                    }
                    c++;
                }
            }
        }
    }
}

/*被怪物吃掉了吗，被哪个怪物吃掉了*/
int Widget::MonsEat(int n)
{
    if(num>=1)
    {
        bool b1=(p1.x>mons[n].x&&p1.x<mons[n].x+mons[n].width);
        bool b2=(p1.y>mons[n].y&&p1.y<mons[n].y+mons[n].height);
        if(b1&&b2)
            return 1;
    }
    if(num==2)
    {
        bool b1=(p2.x>mons[n].x&&p2.x<mons[n].x+mons[n].width);
        bool b2=(p2.y>mons[n].y&&p2.y<mons[n].y+mons[n].height);
        if(b1&&b2)
            return 2;
    }
    return 0;
}

/*是砖块吗*/
bool Widget::isBrick(int r, int c)
{
    if(map[r][c]==2)return true;
    return false;
}

/*是怪物 返回序号i 不是怪物 返回-1*/
int Widget::isMonster(int r, int c)
{
    for(int i=0;i<monsters;i++)
    {
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
    for(int i=0;i<bombs;i++)
    {
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

/*怪物都杀掉了吗*/
bool Widget::NoneMonster()
{
    for(int i=0;i<monsters;i++){
        if(mons[i].state==1)
            return false;
    }
    return true;
}

/*在炸弹里吗*/
bool Widget::inBomb(int x, int y)
{
    bool b1;
    bool b2;
    for(int i=0;i<bombs;i++){
        if(bomb[i].state==0)continue;
        b1=(x>=bomb[i].c*ww+oriX-4&&x<=bomb[i].c*ww+oriX+ww+4);
        b2=(y>=bomb[i].r*hh+oriY-4&&y<=bomb[i].r*hh+oriY+hh+4);
        if(b1&&b2)
            return true;
    }
    return false;
}

/*绘图 + 爆炸标记 + 人物存活状态标记 + 游戏状态标记*/
void Widget::paintEvent(QPaintEvent *)
{
    if(flag==0)return;

    QPainter painter(this);

    painter.drawPixmap(0, 0, width(), height(), QPixmap("../background_game.jpg"));
    painter.drawPixmap(950, 15, 360, 170, QPixmap("../button.png"));

    //painter.drawPixmap(0, 0, QPixmap::fromImage(image[0]));

    //qDebug()<<p2.x<<' '<<p2.y;

    painter.setPen(Qt::transparent);

    /*初始化地图*/
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++)
        {
            if(map[i][j]==1)
            {
                painter.drawPixmap(ww*j+oriX, hh*i+oriY, ww, hh, QPixmap("../wall.png"));
            }
                //painter.setBrush(Qt::gray);
            else if(map[i][j]==2)
            {
                painter.drawPixmap(ww*j+oriX, hh*i+oriY, ww, hh, QPixmap("../brick.png"));
            }
                //painter.setBrush(Qt::yellow);
            else
            {
                painter.drawPixmap(ww*j+oriX, hh*i+oriY, ww, hh, QPixmap("../floor.png"));
                //painter.setBrush(Qt::white);
                //painter.drawRect(ww*j+oriX, hh*i+oriY, ww, hh);
            }
                //painter.setBrush(Qt::white);
            //painter.drawRect(ww*j+oriX, hh*i+oriY, ww, hh);
            //painter.drawText(ww*j+oriX, hh*i+oriY,QString::number(i)+","+QString::number(j));
        }
    }

    /*人物踩了道具之后的效果有问题 bug*//*已修复*/
    /*道具+出口*/
    for(int i=0;i<boom_props;i++)
    {
        if(bpro[i].state==0)continue;
        int r=bpro[i].x;
        int c=bpro[i].y;
        if(map[r][c]!=0)continue;
        painter.drawPixmap(c*ww+oriX, r*hh+oriY, ww, hh, QPixmap("../boom.png"));
        int p=isPlayer(r, c);
        //if(p==0)p=isPlayer(r, c-1);
        //if(p==0)p=isPlayer(r-1, c);
        if(p==1){
            p1.boom++;
            bpro[i].state=0;
        }
        else if(p==2){
            p2.boom++;
            bpro[i].state=0;
        }
    }

    for(int i=0;i<fast_props;i++)
    {
        if(fpro[i].state==0)continue;
        int r=fpro[i].x;
        int c=fpro[i].y;
        if(map[r][c]!=0)continue;
        painter.drawPixmap(c*ww+oriX, r*hh+oriY, ww, hh, QPixmap("../fast.png"));
        int p=isPlayer(r, c);
        //if(p==0)p=isPlayer(r, c-1);
        //if(p==0)p=isPlayer(r-1, c);
        if(p==1){
            p1.speed++;
            fpro[i].state=0;
        }
        else if(p==2){
            p2.speed++;
            fpro[i].state=0;
        }
    }

    for(int i=0;i<stay_props;i++)
    {
        if(spro[i].state==0)continue;
        int r=spro[i].x;
        int c=spro[i].y;
        if(map[r][c]!=0)continue;
        painter.drawPixmap(spro[i].y*ww+oriX, spro[i].x*hh+oriY, ww, hh, QPixmap("../stay.png"));
        int p=isPlayer(r ,c);
        //if(p==0)p=isPlayer(r, c-1);
        //if(p==0)p=isPlayer(r-1, c);
        if(p==1){
            p1.stay=1;
            spro[i].state=0;
        }
        else if(p==2){
            p2.stay=1;
            spro[i].state=0;
        }
    }

    for(int i=0;i<mult_props;i++)
    {
        if(mpro[i].state==0)continue;
        int r=mpro[i].x;
        int c=mpro[i].y;
        if(map[r][c]!=0)continue;
        painter.drawPixmap(c*ww+oriX, r*hh+oriY, ww, hh, QPixmap("../mult.png"));
        int p=isPlayer(r, c);
        //if(p==0)p=isPlayer(r, c-1);
        //if(p==0)p=isPlayer(r-1, c);
        if(p==1){
            p1.mult++;
            mpro[i].state=0;
        }
        else if(p==2){
            p2.mult++;
            mpro[i].state=0;
        }
    }

    if(map[odor.x][odor.y]==0&&num==1)
    {
        painter.drawPixmap(odor.y*ww+oriX, odor.x*hh+oriY, ww, hh, QPixmap("../outdoor.png"));
        if(NoneMonster()){
            int p=isPlayer(odor.x, odor.y);
            if(p==1){
                    NO=-1;
            }
        }
    }
    /*判断主角在门上的时候是否赢得游戏 + 道具是否被主角吃到*/

    /*小怪物*/
    for(int i=0;i<monsters;i++)
    {
        if(mons[i].state==0)continue;

        painter.drawPixmap(mons[i].x, mons[i].y, ww, hh, QPixmap::fromImage(image[i]), mons[i].img_x*mons[i].width, mons[i].img_y*mons[i].height, mons[i].width, mons[i].height);

        if(NO==0)NO=MonsEat(i);
        //if(NO==1){
        //    qDebug()<<"yeah";
        //}
        //qDebug()<<mons[i].x<<' '<<mons[i].y;
    }

    /*for(int i=0;i<monsters;i++){
        if(mons[i].state==0)continue;
        //qDebug()<<mons[i].transR<<' '<<mons[i].transC;
        painter.setBrush(Qt::red);
        painter.drawRect(ww*mons[i].transC+oriX, hh*mons[i].transR+oriY, ww, hh);
        //qDebug()<<endl;
    }*/

    /*主角*/
    if(num>=1)
    {
        if(p1choose==0)
            painter.drawPixmap(p1.x, p1.y, QPixmap("../duora.png"), p1.img_x*p1.width, p1.img_y*p1.height, p1.width, p1.height);
        else if(p1choose==1)
            painter.drawPixmap(p1.x, p1.y, QPixmap("../DaXiong.png"), p1.img_x*p1.width, p1.img_y*p1.height, p1.width, p1.height);
        else if(p1choose==2)
            painter.drawPixmap(p1.x, p1.y, QPixmap("../jingxiang.png"), p1.img_x*p1.width, p1.img_y*p1.height, p1.width, p1.height);
        else if(p1choose==3)
            painter.drawPixmap(p1.x, p1.y, QPixmap("../xiaofu.png"), p1.img_x*p1.width, p1.img_y*p1.height, p1.width, p1.height);
        else if(p1choose==4)
            painter.drawPixmap(p1.x, p1.y, QPixmap("../panghu.png"), p1.img_x*p1.width, p1.img_y*p1.height, p1.width, p1.height);

    }
    if(num==2)
    {
        if(p2choose==0)
            painter.drawPixmap(p2.x, p2.y, QPixmap("../duora.png"), p2.img_x*p2.width, p2.img_y*p2.height, p2.width, p2.height);
        else if(p2choose==1)
            painter.drawPixmap(p2.x, p2.y, QPixmap("../DaXiong.png"), p2.img_x*p2.width, p2.img_y*p2.height, p2.width, p2.height);
        else if(p2choose==2)
            painter.drawPixmap(p2.x, p2.y, QPixmap("../jingxiang.png"), p2.img_x*p2.width, p2.img_y*p2.height, p2.width, p2.height);
        else if(p2choose==3)
            painter.drawPixmap(p2.x, p2.y, QPixmap("../xiaofu.png"), p2.img_x*p2.width, p2.img_y*p2.height, p2.width, p2.height);
        else if(p2choose==4)
            painter.drawPixmap(p2.x, p2.y, QPixmap("../panghu.png"), p2.img_x*p2.width, p2.img_y*p2.height, p2.width, p2.height);

    }

    //if(NO==1){
    //    qDebug()<<"yeah";
    //}
    //奇妙的bug自己修复了，纪念一下~

    /*炸弹*/
    /*人躲在炸弹里面炸不死 bug*//*已修复*/
    for(int i=0;i<bombs;i++)
    {
        if(bomb[i].state==0)continue;//炸弹膨胀
        //if(NO==1){
        //    qDebug()<<"yeah";
        //}
        if(bomb[i].explode==0)painter.drawPixmap(bomb[i].c*ww+oriX, bomb[i].r*hh+oriY, ww, hh, QPixmap("../bomb.png"), bomb[i].img_x*bomb[i].width, bomb[i].img_y*bomb[i].height, bomb[i].width, bomb[i].height);
        if(bomb[i].explode==1){//炸弹爆炸

            painter.drawPixmap(bomb[i].c*ww+oriX, bomb[i].r*hh+oriY, ww, hh, QPixmap("../bomb.png"), 0*bomb[i].width, 2*bomb[i].height, bomb[i].width, bomb[i].height);

            int r=bomb[i].r;
            int c=bomb[i].c;
            int dr, dc;

            if(NO==0)NO=isPlayer(r, c);
            //if(num>=1)NO=inBomb(p1.x, p1.y);
            //if(NO==0&&num==2)NO=inBomb(p2.x, p2.y);

            while(isMonster(r, c)!=-1)
                mons[isMonster(r, c)].state=0;

            dr=1;//up
            while((map[r-dr][c]==0||isBrick(r-dr, c))&&dr<=bomb[i].scale){
                //qDebug()<<(r-dr)<<' '<<c<<map[r-dr][c];
                if(NO==0)NO=isPlayer(r-dr, c);
                while(isMonster(r-dr, c)!=-1){
                    mons[isMonster(r-dr, c)].state=0;
                }
                if(dr<bomb[i].scale)
                    painter.drawPixmap(c*ww+oriX, (r-dr)*hh+oriY, ww, hh, QPixmap("../bomb.png"), 5*bomb[i].width, 2*bomb[i].height, bomb[i].width, bomb[i].height);
                else
                    painter.drawPixmap(c*ww+oriX, (r-dr)*hh+oriY, ww, hh, QPixmap("../bomb.png"), 6*bomb[i].width, 2*bomb[i].height, bomb[i].width, bomb[i].height);
                int t=isBomb(r-dr, c);
                if(t!=-1){
                    bomb[t].explode=1;
                }
                if(isBrick(r-dr, c)){
                    map[r-dr][c]=0;
                    //qDebug()<<"yeah";
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
                if(dr<bomb[i].scale)
                    painter.drawPixmap(c*ww+oriX, (r+dr)*hh+oriY, ww, hh, QPixmap("../bomb.png"), 5*bomb[i].width, 2*bomb[i].height, bomb[i].width, bomb[i].height);
                else
                    painter.drawPixmap(c*ww+oriX, (r+dr)*hh+oriY, ww, hh, QPixmap("../bomb.png"), 8*bomb[i].width, 2*bomb[i].height, bomb[i].width, bomb[i].height);
                int t=isBomb(r+dr, c);
                if(t!=-1){
                    bomb[t].explode=1;
                }
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
                if(dc<bomb[i].scale)
                    painter.drawPixmap((c-dc)*ww+oriX, r*hh+oriY, ww, hh, QPixmap("../bomb.png"), 1*bomb[i].width, 2*bomb[i].height, bomb[i].width, bomb[i].height);
                else
                    painter.drawPixmap((c-dc)*ww+oriX, r*hh+oriY, ww, hh, QPixmap("../bomb.png"), 2*bomb[i].width, 2*bomb[i].height, bomb[i].width, bomb[i].height);
                int t=isBomb(r, c-dc);
                if(t!=-1){
                    bomb[t].explode=1;
                }
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
                if(dc<bomb[i].scale)
                    painter.drawPixmap((c+dc)*ww+oriX, r*hh+oriY, ww, hh, QPixmap("../bomb.png"), 1*bomb[i].width, 2*bomb[i].height, bomb[i].width, bomb[i].height);
                else
                    painter.drawPixmap((c+dc)*ww+oriX, r*hh+oriY, ww, hh, QPixmap("../bomb.png"), 4*bomb[i].width, 2*bomb[i].height, bomb[i].width, bomb[i].height);
                int t=isBomb(r, c+dc);
                if(t!=-1){
                    bomb[t].explode=1;
                }
                if(isBrick(r, c+dc)){
                    map[r][c+dc]=0;
                    break;
                }
                dc++;
            }
            //qDebug()<<"yeah";
            /*这里如果主角死掉之后会刷两次 bug*//*已修复*/
            QTimer::singleShot(300, this, [=](){bomb[i].state=0;bomb[i].explode=0;});

            //if(bomb[i].owner==1)p1.bomb--;
            //else if(bomb[i].owner==2)p2.bomb--;
            //for(int i=0;i<monsters;i++)
            //    qDebug()<<mons[i].state;
        }
    }
    //if(NO==1){
    //    qDebug()<<"yeah";
    //}
}

/*按压事件*/
void Widget::keyPressEvent(QKeyEvent *event)
{
    if(!event->isAutoRepeat())
        pressedKeys.insert(event->key());
    if(!updateTimer->isActive())
        updateTimer->start(100);
    //update();
    //dealGame(NO);
    //NO=0;
    /*是否可以设置为每隔0.3秒刷新一下*/
}

/*松开事件*/
void Widget::keyReleaseEvent(QKeyEvent *event)
{
    if(!event->isAutoRepeat()&&(!pressedKeys.isEmpty()))
        pressedKeys.remove(event->key());
    if(pressedKeys.isEmpty())
        updateTimer->stop();
}

/*暂停游戏*/
void Widget::mousePressEvent(QMouseEvent *event)
{
    if(flag==1&&event->button()==Qt::LeftButton){
        ui->stack->setCurrentIndex(0);
        flag=0;
        start=0;
        pause=1;
        timer->stop();
        intervalTimer->stop();
        ui->startImage->hide();
        ui->contImage->show();
        update();
    }
}
