#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QImage>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    int w=0;
    int h=0;
    int ww=0;
    int hh=0;
    int oriX=0;
    int oriY=0;

    int walls=0;
    int monsters=0;
    int bombs=0;
    int time=0;

    /*标记*/
    int start=1;
    int flag=0;
    int num=1;
    int choose=0;

    int boom_props=3;//强化炸弹
    int mult_props=2;//多个炸弹
    int stay_props=1;//呆在炸弹
    int fast_props=1;//快速移动
    int outdoors=1;//出口

    /*地图
     * -1/-2 主角
     * 0 空地
     * 1 墙体
     * 2 砖块
     * 100 出口
     */
    int map[50][50];
    QTimer *timer = new QTimer(this);
    QTimer *randomTimer = new QTimer(this);
    QTimer *intervalTimer = new QTimer(this);

    void setMap();
    void dealGame(int n);//判段输赢
    int isPlayer(int r, int c);
    int isMonster(int r, int c);
    int isBomb(int r, int c);
    bool isBrick(int r, int c);
    bool isEmpty(int r, int c);
    bool NoneMonster();

    struct player{
        int x, y;
        int img_x, img_y;
        int width, height;
        int state;
        int boom, mult, stay, fast;
        int speed;
        int bomb;
        int character;
    }p1, p2;

    struct monster{
        int speed;
        int x, y;
        int img_x, img_y;
        int width, height;
        int state;
        int kind;// 0 1 2 3
        int dir;// 0up 1left 2down 3right
    }mons[10];

    struct bombS{
        int r, c;
        int img_x, img_y;
        int width, height;
        int state;
        int explode;
        int scale;
        int owner;
        bombS(){img_x=2, img_y=0, width=24, height=24, state=0, explode=0, scale=0;}
    }bomb[100];

    /*以下的x y表示map的第x行第y列*/
    struct boomProp{
        int x, y;
        int state;
    }bpro[10];
    struct stayProp{
        int x, y;
        int state;
    }spro[10];
    struct fastProp{
        int x, y;
        int state;
    }fpro[10];
    struct multProp{
        int x, y;
        int state;
    }mpro[10];

    struct outdoor{
        int x, y;
        int state;
        outdoor(){x=0;y=0;state=0;}
    }odor;

protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);

private slots:

    void on_solo_clicked();

    void on_duet_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
