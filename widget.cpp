#include "widget.h"
#include "ui_widget.h"
#include <QTime>
#include <QWidget>
#include <QTextStream>
#include <QDebug>
#include <QTextEdit>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::SetPos()
{

}

void Widget::SetCard()
{
    int black=n/2;
    int red=n-n/2;

}

void Widget::Vote()
{

}

void Widget::Deal()
{

}

void Widget::Shift()
{

}

void Widget::Next()
{

}

