#include "widget.h"
#include "aiaudio.h"
#include <QDebug>
#include <QIcon>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/Icon.jpg"));
//    Aiaudio *w = new Aiaudio;
    Widget *w = new Widget;
    w->setWindowIcon(QIcon(":/images/Icon.jpg"));
    w->show();
    return a.exec();
}
