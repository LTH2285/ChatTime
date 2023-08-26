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

    if (!db.open())
    {
        qWarning() << "Failed to open database:";
        return false;
    }

    qDebug() << "Database connected!";
    return true;
}

bool Init::createTable()
{
    QSqlQuery query;

    bool flag = true;

    flag = flag && query.exec("CREATE TABLE IF NOT EXISTS user_information_table (\
                        userID INTEGER PRIMARY KEY,\
                        username TEXT NOT NULL,\
                        password TEXT NOT NULL,\
                        userIP TEXT NOT NULL,\
                        userPORT INTEGER NOT NULL,\
                        photo BLOB DEFAULT 'default')");
    qDebug() << flag;

    flag = flag && query.exec("CREATE TABLE IF NOT EXISTS friend_relationship_table (\
                        userIP TEXT,\
                        Friend_user_ID TEXT)");
    qDebug() << flag;

    flag = flag && query.exec("CREATE TABLE IF NOT EXISTS group_info_table (\
                        groupID INTEGER PRIMARY KEY,\
                        groupname TEXT,\
                        managerID INTEGER)");
    qDebug() << flag;

    flag = flag && query.exec("CREATE TABLE IF NOT EXISTS group_member_table (\
                        groupID INTEGER PRIMARY KEY,\
                        userID INTEGER)");
    qDebug() << flag;

    flag = flag && query.exec("CREATE TABLE IF NOT EXISTS message_table (\
                        sendID INTEGER PRIMARY KEY,\
                        recvID INTEGER,\
                        message TEXT,\
                        send_time TEXT)");
    qDebug() << flag;

    flag = flag && query.exec("CREATE TABLE IF NOT EXISTS user_login_table (\
                        userID INTEGER PRIMARY KEY,\
                        last_login_IP TEXT,\
                        last_login_time TEXT)");
    return flag;
}
