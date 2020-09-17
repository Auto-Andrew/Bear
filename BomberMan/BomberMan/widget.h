#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

struct player{
    int skills;
    int props[3];//道具
    int state;//存/亡

    player(){
        props[0]=0;
        props[1]=0;
        props[2]=0;
        state=1;
    }
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    player p1;
    player p2;

    int w=0;
    int h=0;

    int walls=0;
    int monsters=0;
    int time=0;
    int score=0;

    int start=1;
    int flag=0;
    int num=1;

    int boom=1;//强化炸弹
    int mult=1;//多个炸弹
    int stay=1;//呆在炸弹

    int outdoor=1;

    /*地图
     *-1/-2 主角
     *0 空地
     *1 不可摧毁墙体
     *2 可摧毁墙体
     *3/4/5.. 小怪物
     *11/12/13.. 道具
     *16/17/18.. 其他
     *100 出口
     */
    int map[100][100];

    void setMap();

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
