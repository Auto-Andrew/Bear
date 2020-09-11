#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QLabel>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QLabel label(this);
    label.setTextFormat(Qt::RichText);


}

Widget::~Widget()
{
    delete ui;
}
