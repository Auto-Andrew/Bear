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
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QStackedWidget *stack;
    QWidget *welcome;
    QLabel *startImage;
    QLabel *contImage;
    QWidget *menu;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *circleP1;
    QLabel *circleP2;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QWidget *game;
    QLabel *label_17;
    QLabel *label_18;
    QLabel *label_19;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(1452, 905);
        stack = new QStackedWidget(Widget);
        stack->setObjectName(QStringLiteral("stack"));
        stack->setGeometry(QRect(1, 1, 1451, 911));
        welcome = new QWidget();
        welcome->setObjectName(QStringLiteral("welcome"));
        startImage = new QLabel(welcome);
        startImage->setObjectName(QStringLiteral("startImage"));
        startImage->setGeometry(QRect(0, 1, 1451, 911));
        startImage->setPixmap(QPixmap(QString::fromUtf8("../background_start.png")));
        startImage->setScaledContents(true);
        contImage = new QLabel(welcome);
        contImage->setObjectName(QStringLiteral("contImage"));
        contImage->setGeometry(QRect(0, 0, 1451, 911));
        contImage->setPixmap(QPixmap(QString::fromUtf8("../background_continue.png")));
        contImage->setScaledContents(true);
        stack->addWidget(welcome);
        menu = new QWidget();
        menu->setObjectName(QStringLiteral("menu"));
        label = new QLabel(menu);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(310, 350, 120, 120));
        label->setPixmap(QPixmap(QString::fromUtf8("../\345\223\206\345\225\246a\346\242\246.png")));
        label->setScaledContents(true);
        label_2 = new QLabel(menu);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(540, 350, 120, 120));
        label_2->setPixmap(QPixmap(QString::fromUtf8("../\345\244\247\351\233\204.png")));
        label_2->setScaledContents(true);
        label_3 = new QLabel(menu);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(770, 350, 120, 120));
        label_3->setPixmap(QPixmap(QString::fromUtf8("../\351\235\231\351\246\231.png")));
        label_3->setScaledContents(true);
        label_4 = new QLabel(menu);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(1000, 350, 120, 120));
        label_4->setPixmap(QPixmap(QString::fromUtf8("../\345\260\217\345\244\253.png")));
        label_4->setScaledContents(true);
        label_5 = new QLabel(menu);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(1230, 350, 120, 120));
        label_5->setPixmap(QPixmap(QString::fromUtf8("../\350\203\226\350\231\216.png")));
        label_5->setScaledContents(true);
        label_6 = new QLabel(menu);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(310, 600, 120, 120));
        label_6->setPixmap(QPixmap(QString::fromUtf8("../\345\223\206\345\225\246a\346\242\246.png")));
        label_6->setScaledContents(true);
        label_7 = new QLabel(menu);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(540, 600, 120, 120));
        label_7->setPixmap(QPixmap(QString::fromUtf8("../\345\244\247\351\233\204.png")));
        label_7->setScaledContents(true);
        label_8 = new QLabel(menu);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(770, 600, 120, 120));
        label_8->setPixmap(QPixmap(QString::fromUtf8("../\351\235\231\351\246\231.png")));
        label_8->setScaledContents(true);
        label_9 = new QLabel(menu);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(1000, 600, 120, 120));
        label_9->setPixmap(QPixmap(QString::fromUtf8("../\345\260\217\345\244\253.png")));
        label_9->setScaledContents(true);
        label_10 = new QLabel(menu);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(1230, 600, 120, 120));
        label_10->setPixmap(QPixmap(QString::fromUtf8("../\350\203\226\350\231\216.png")));
        label_10->setScaledContents(true);
        circleP1 = new QLabel(menu);
        circleP1->setObjectName(QStringLiteral("circleP1"));
        circleP1->setGeometry(QRect(78, 347, 124, 124));
        circleP1->setPixmap(QPixmap(QString::fromUtf8("../\345\234\206 1.png")));
        circleP1->setScaledContents(true);
        circleP2 = new QLabel(menu);
        circleP2->setObjectName(QStringLiteral("circleP2"));
        circleP2->setGeometry(QRect(78, 597, 124, 124));
        circleP2->setPixmap(QPixmap(QString::fromUtf8("../\345\234\206 2.png")));
        circleP2->setScaledContents(true);
        label_11 = new QLabel(menu);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(80, 350, 120, 120));
        label_11->setPixmap(QPixmap(QString::fromUtf8("../p1.png")));
        label_11->setScaledContents(true);
        label_12 = new QLabel(menu);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(80, 600, 120, 120));
        label_12->setPixmap(QPixmap(QString::fromUtf8("../p2.png")));
        label_12->setScaledContents(true);
        label_13 = new QLabel(menu);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(10, 10, 1441, 901));
        label_13->setPixmap(QPixmap(QString::fromUtf8("../background_menu.png")));
        label_13->setScaledContents(true);
        label_14 = new QLabel(menu);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(90, 50, 291, 81));
        label_14->setPixmap(QPixmap(QString::fromUtf8("../doraemonLogo.png")));
        label_14->setScaledContents(true);
        label_15 = new QLabel(menu);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(470, 0, 291, 181));
        label_15->setPixmap(QPixmap(QString::fromUtf8("../bomberManLogo.PNG")));
        label_15->setScaledContents(true);
        label_16 = new QLabel(menu);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(420, 80, 31, 31));
        label_16->setPixmap(QPixmap(QString::fromUtf8("../cha.png")));
        label_16->setScaledContents(true);
        stack->addWidget(menu);
        label_13->raise();
        label_2->raise();
        label_3->raise();
        label_4->raise();
        label_5->raise();
        label_6->raise();
        label_7->raise();
        label_8->raise();
        label_9->raise();
        label_10->raise();
        label_11->raise();
        label_12->raise();
        label->raise();
        circleP1->raise();
        circleP2->raise();
        label_14->raise();
        label_15->raise();
        label_16->raise();
        game = new QWidget();
        game->setObjectName(QStringLiteral("game"));
        label_17 = new QLabel(game);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(180, 110, 81, 91));
        label_17->setScaledContents(true);
        label_18 = new QLabel(game);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(400, 110, 81, 91));
        label_18->setScaledContents(true);
        label_19 = new QLabel(game);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(650, 110, 81, 91));
        label_19->setScaledContents(true);
        stack->addWidget(game);

        retranslateUi(Widget);

        stack->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", Q_NULLPTR));
        startImage->setText(QString());
        contImage->setText(QString());
        label->setText(QString());
        label_2->setText(QString());
        label_3->setText(QString());
        label_4->setText(QString());
        label_5->setText(QString());
        label_6->setText(QString());
        label_7->setText(QString());
        label_8->setText(QString());
        label_9->setText(QString());
        label_10->setText(QString());
        circleP1->setText(QString());
        circleP2->setText(QString());
        label_11->setText(QString());
        label_12->setText(QString());
        label_13->setText(QString());
        label_14->setText(QString());
        label_15->setText(QString());
        label_16->setText(QString());
        label_17->setText(QApplication::translate("Widget", "TextLabel", Q_NULLPTR));
        label_18->setText(QApplication::translate("Widget", "TextLabel", Q_NULLPTR));
        label_19->setText(QApplication::translate("Widget", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
