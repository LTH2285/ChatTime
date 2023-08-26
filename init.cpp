#include "Init.h"
#include <QSqlQuery>
#include <QDebug>

Init::Init(QObject *parent) : QObject(parent)
{
    QSqlDatabase db = createDatabase();
    bool b = createTable();
    if(!b)
    {
        qDebug() << "create table error!";
    }
    db.close();
}

QSqlDatabase Init::createDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("SQL.db");

    if (!db.open())
    {
        qWarning() << "Failed to open database:";
        QSqlDatabase db;
        return db;
    }

    qDebug() << "Database connected!";
    return db;
}

bool Init::createTable()
{
    QSqlQuery query;

    bool flag = true;

//用户信息表
    flag = flag && query.exec("CREATE TABLE IF NOT EXISTS user_information_table (\
                        userID INTEGER PRIMARY KEY,\
                        userName TEXT NOT NULL,\
                        passWord TEXT NOT NULL,\
                        photo BLOB DEFAULT 'default')");
//    qDebug() << flag;

//好友关系表
    flag = flag && query.exec("CREATE TABLE IF NOT EXISTS friend_relationship_table (\
                        userIP TEXT,\
                        friendID TEXT,\
                        passed INTEGER)");//0表示还没通过，1表示已经通过
//    qDebug() << flag;

//群组信息表
    flag = flag && query.exec("CREATE TABLE IF NOT EXISTS group_info_table (\
                        groupID INTEGER PRIMARY KEY,\
                        groupName TEXT,\
                        managerID INTEGER)");
//    qDebug() << flag;

//群成员表
    flag = flag && query.exec("CREATE TABLE IF NOT EXISTS group_member_table (\
                        groupID INTEGER PRIMARY KEY,\
                        userID TEXT)");
//注：用户成员ID以逗号隔开，保存为文本
//    qDebug() << flag;

//群消息表
    flag = flag && query.exec("CREATE TABLE IF NOT EXISTS message_table (\
                        sendID INTEGER PRIMARY KEY,\
                        recvID INTEGER,\
                        message TEXT,\
                        sendTime TEXT)");
//    qDebug() << flag;

//用户登录信息表
    flag = flag && query.exec("CREATE TABLE IF NOT EXISTS user_login_table (\
                        userID INTEGER PRIMARY KEY,\
                        lastLoginIP TEXT,\
                        lastLoginPORT INTEGER,\
                        lastLogintime TEXT)");
    return flag;
}
