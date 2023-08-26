#include "mainwindow.h"
#include "Init.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Init init;

    if (init.createDatabase()) {
            if (init.createUserTable()) {
                qDebug() << "Database and user table created successfully!";
            } else {
                qDebug() << "User table creation failed.";
            }
        } else {
            qDebug() << "Database connection failed.";
        }

    return a.exec();
}
