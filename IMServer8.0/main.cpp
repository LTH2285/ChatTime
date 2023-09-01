#include "Init.h"
#include "imserver.h"
#include "utls.h"
#include "myserver.h"

#include <QApplication>

#include <QtSql/QSqlDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    myServer w;
    w.show();
    return a.exec();
}
