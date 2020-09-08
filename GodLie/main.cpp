#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    w.setWindowTitle("GodLie");
    w.setFixedSize(1294, 648);
    return a.exec();
}
