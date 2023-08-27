#include "mainwindow.h"
#include "Init.h"
#include "utls.h"
#include <QApplication>
#include <QDebug>
#include <QSqlQueryModel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //    MainWindow w;
    //    w.show();
    Init init;
    Utls utls;
    QSqlDatabase db = init.createDatabase();

    QSqlQueryModel model;
    model.setQuery("SELECT * FROM user_information_table");

    QString columnNames = utls.getColumnNames(&model);
    qDebug() << columnNames;
    return a.exec();

    //    if (init.createDatabase()) {
    //            if (init.createUserTable()) {
    //                qDebug() << "Database and user table created successfully!";
    //            } else {
    //                qDebug() << "User table creation failed.";
    //            }
    //        } else {
    //            qDebug() << "Database connection failed.";
    //        }
}
