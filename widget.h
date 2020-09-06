#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Player
{
public:
    int identity;//0表示黑牌，1表示红牌
    int state;//状态 0表示被驱逐 1表示游戏中
    double vote;//票数
    double voteNum;//得票数
    int eye;//是否睁眼 0不睁眼 1睁眼
    Player(){state=1, vote=1, voteNum=0, eye=0;}

};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    Player p[100];
    int n=1;
    int head;
    int black;
    int red;
    int currentNO;
    int right;
    int flag=0;
    int pShow=0;
    int chFlag=0;
    void SetCard();//发牌
    void Vote();//投票
    void Deal(int k);//放逐+判断游戏是否结束
    void Shift();//白/夜切换
    int NOShift(int k);

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void on_start_clicked();

    void on_submit_clicked();

    void on_voteSubmit_clicked();

private:
    Ui::Widget *ui;

};

#endif // WIDGET_H
