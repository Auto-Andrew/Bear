#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Player
{
public:
    int identity;
    int state;
    int position;
    int vote;
    int voteNum;
    Player(){state=0, vote=1, voteNum=0;}

};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    Player p[100];
    void SetPos();//布局
    void SetCard();//发牌
    void Vote();//投票
    void Deal();//放逐
    void Shift();//白/夜切换
    int n;
public slots:
    void Next();

private:
    Ui::Widget *ui;

};

#endif // WIDGET_H
