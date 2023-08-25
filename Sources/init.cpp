#include "Init.h"
#include <QSqlQuery>
#include <QDebug>

Init::Init(QObject *parent) : QObject(parent)
{
}

bool Init::createDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("SQL.db");

    if (!db.open()) {
        qWarning() << "Failed to open database:";
        return false;
    }

    qDebug() << "Database connected!";
    return true;
}

bool Init::createUserTable()
{
    QSqlQuery query;

    // 创建用户信息表
    if (!query.exec("CREATE TABLE IF NOT EXISTS User information table ("
                    "UserIP TEXT PRIMARY KEY,"
                    "name TEXT NOT NULL,"
                    "password TEXT NOT NULL)")) {
        qWarning() << "Table creation error:" << query.lastError().text();
        return false;
    }

    qDebug() << "User table created!";
    return true;
}
