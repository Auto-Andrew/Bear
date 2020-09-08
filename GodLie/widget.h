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
    int head=1;//村长
    int black;//黑牌
    int red;//红牌
    int night=-1;//0红夜 1黑夜
    int currentNO=1;//当前玩家
    int right=0;//0左边 1右边
    int flag=0;//0什么也不做 1选村长 2游戏环节
    int pShow=0;//是否刷新重绘
    int chFlag=0;//关于村长的特殊投票
    void SetCard();//发牌
    void Vote();//投票
    void Deal(int k);//放逐+判断游戏是否结束
    void lastWords(int k);//遗言
    int NOShift(int k);//切换下一位玩家
    int Tie();//平票
    int NightShift();//进入黑/红夜

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void on_start_clicked();

    void on_submit_clicked();

    void on_voteSubmit_clicked();

    void on_eyeOpen_clicked();

    void on_skip_clicked();

private:
    Ui::Widget *ui;

};

#endif // WIDGET_H
