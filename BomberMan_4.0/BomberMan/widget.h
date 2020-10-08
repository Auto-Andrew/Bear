#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QImage>
#include <QPainter>
#include <QSet>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    /*布局大小*/
    int w=0;
    int h=0;
    int ww=0;
    int hh=0;
    int oriX=0;
    int oriY=0;

    /*选择框*/
    int circle1x=0;
    int circle2x=0;
    int circle1y=0;
    int circle2y=0;

    /*元素个数*/
    int walls=0;
    int monsters=0;
    int bombs=0;
    int NO=0;

    /*标记*/
    int start=1;
    int flag=0;
    int num=1;
    int pause=0;
    int p1choose=-1;
    int p2choose=-1;

    /*道具*/
    int boom_props=3;//强化炸弹
    int mult_props=2;//多个炸弹
    int stay_props=1;//呆在炸弹
    int fast_props=1;//快速移动
    int outdoors=1;//出口

    /*地图
     * 0 空地
     * 1 墙体
     * 2 砖块
     */

    int map[50][50];
    QImage image[10];
    QTimer *timer = new QTimer(this);
    QTimer *intervalTimer = new QTimer(this);
    QTimer *updateTimer = new QTimer(this);

    void setMap();//设置地图
    void dealGame(int n);//判段输赢
    void dirTrans();//使怪物能转向
    int isPlayer(int r, int c);//判断指定位置是否为玩家
    int isMonster(int r, int c);//判断指定位置是否为怪物
    int isBomb(int r, int c);//判断指定位置是否为炸弹
    int MonsEat(int n);//判断某个怪物是否能吃掉玩家
    bool isBrick(int r, int c);//判断指定位置是否为砖块
    bool isEmpty(int r, int c);//判断指定位置是否为空地且没有放置炸弹
    bool NoneMonster();//判断场上的怪物是否都被消灭
    bool inBomb(int x, int y);//判断指定位置是否处于炸弹位

    struct player{
        int x, y;
        int img_x, img_y;
        int width, height;
        int state;
        int boom, mult, stay, fast;
        int speed;
        int bomb;
        int character;
        void clear(){x=0;y=0;img_x=0;img_y=0;width=0;height=0;state=0;boom=0;mult=1;stay=0;fast=0;speed=6;bomb=0;character=0;}
    }p1, p2;

    struct monster{
        int speed;
        int x, y;
        int transR, transC;
        int img_x, img_y;
        int width, height;
        int state;
        int kind;// 0 1 2 3
        int dir;// 0up 1left 2down 3right
        monster(){x=0;y=0;transR=0;transC=0;img_x=0;img_y=0;width=0;height=0;state=0;speed=5;kind=0;dir=0;}
    };
    monster *mons;

    struct bombS{
        int r, c;
        int img_x, img_y;
        int width, height;
        int state;
        int explode;
        int scale;
        int owner;
        bombS(){img_x=2;img_y=0;width=24;height=24;state=0;explode=0;scale=0;}
    };
    bombS *bomb;

    /*以下的x y表示map的第x行第y列*/
    struct boomProp{
        int x, y;
        int state;
    };
    boomProp *bpro = new boomProp[10];

    struct stayProp{
        int x, y;
        int state;
    };
    stayProp *spro = new stayProp[10];

    struct fastProp{
        int x, y;
        int state;
    };
    fastProp *fpro = new fastProp[10];

    struct multProp{
        int x, y;
        int state;
    };
    multProp *mpro = new multProp[10];

    struct outdoor{
        int x, y;
        int state;
        outdoor(){x=0;y=0;state=0;}
    }odor;

protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);

private slots:

private:
    Ui::Widget *ui;
    QSet <int> pressedKeys;
};

#endif // WIDGET_H
