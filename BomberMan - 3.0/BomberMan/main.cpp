#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    w.setWindowTitle("BomberMan");
    //w.setFixedSize(1000, 600);
    return a.exec();
}
