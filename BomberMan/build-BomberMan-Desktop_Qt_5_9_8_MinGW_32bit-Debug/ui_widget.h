/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QStackedWidget *stack;
    QWidget *welcome;
    QLabel *startImage;
    QPushButton *start;
    QPushButton *cont;
    QLabel *contImage;
    QWidget *menu;
    QPushButton *solo;
    QPushButton *duet;
    QWidget *game;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(1013, 612);
        stack = new QStackedWidget(Widget);
        stack->setObjectName(QStringLiteral("stack"));
        stack->setGeometry(QRect(1, 1, 1011, 611));
        welcome = new QWidget();
        welcome->setObjectName(QStringLiteral("welcome"));
        startImage = new QLabel(welcome);
        startImage->setObjectName(QStringLiteral("startImage"));
        startImage->setGeometry(QRect(0, 1, 1011, 611));
        startImage->setPixmap(QPixmap(QString::fromUtf8("../background_start.png")));
        startImage->setScaledContents(true);
        start = new QPushButton(welcome);
        start->setObjectName(QStringLiteral("start"));
        start->setGeometry(QRect(400, 450, 171, 31));
        cont = new QPushButton(welcome);
        cont->setObjectName(QStringLiteral("cont"));
        cont->setGeometry(QRect(400, 490, 271, 31));
        contImage = new QLabel(welcome);
        contImage->setObjectName(QStringLiteral("contImage"));
        contImage->setGeometry(QRect(0, 0, 1011, 611));
        contImage->setPixmap(QPixmap(QString::fromUtf8("../background_continue.png")));
        contImage->setScaledContents(true);
        stack->addWidget(welcome);
        menu = new QWidget();
        menu->setObjectName(QStringLiteral("menu"));
        solo = new QPushButton(menu);
        solo->setObjectName(QStringLiteral("solo"));
        solo->setGeometry(QRect(340, 250, 151, 81));
        duet = new QPushButton(menu);
        duet->setObjectName(QStringLiteral("duet"));
        duet->setGeometry(QRect(520, 250, 151, 81));
        stack->addWidget(menu);
        game = new QWidget();
        game->setObjectName(QStringLiteral("game"));
        stack->addWidget(game);

        retranslateUi(Widget);

        stack->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", Q_NULLPTR));
        startImage->setText(QString());
        start->setText(QApplication::translate("Widget", "start", Q_NULLPTR));
        cont->setText(QApplication::translate("Widget", "continue", Q_NULLPTR));
        contImage->setText(QString());
        solo->setText(QApplication::translate("Widget", "\345\215\225\344\272\272\346\270\270\346\210\217", Q_NULLPTR));
        duet->setText(QApplication::translate("Widget", "\344\270\244\344\272\272PVP", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
