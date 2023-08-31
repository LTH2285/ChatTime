#include "func.h"
#include "utls.h"
#include <QDebug>
#include <Init.h>

Func::Func()
{
}

// 检查给定的用户ID是否在user_information_table中存在
//
// 参数:
// - userID:要检查存在性的用户ID
//
// 返回值:
// - 如果用户ID存在,则返回true,否则返回false
bool Func::checkUserIDExist(int userID)
{
    Utls utls;
    QSqlQuery query = utls.researchi("user_information_table", "userID=" + QString::number(userID));

    if (query.first())
    {
        qDebug() << "UserID exists:" << userID;
        return true;
    }
    else
    {
        qDebug() << "UserID does not exist:" << userID;
        return false;
    }
}

// 用给定的用户名和密码注册新账户
//
// 参数:
// - username: 新账户的用户名
// - password: 新账户的密码
//
// 返回值:
// - 新注册账户的ID,如果有错误则返回-1
int Func::account_register(const QString username, const QString password)
{
    Utls utls;
    int existingrows = utls.getRowNumbers("user_information_table");
    if (existingrows < 0)
    {
        qDebug() << "Error fetching existing row numbers.";
        return -1; // Error code
    }

    int id = existingrows + 1;
    QString values = QString("%1, '%2', '%3', NULL").arg(id).arg(username).arg(password);
    bool status = utls.creati("user_information_table", values);
    if (!status)
    {
        qDebug() << "Error registering the account.";
        return -1; // Error code
    }
    else
    {
        qDebug() << "Successfully registered the account with ID:" << id;
        return id; // Return the ID of the newly registered account
    }
}

// 检查给定用户ID的登录凭证是否匹配
//
// 参数:
// - userID: 尝试登录的用户ID
// - password: 用户输入的密码
//
// 返回值:
// - 如果凭证匹配并登录成功,则返回true,否则返回false
bool Func::loginFunction(int userID, const QString password)
{
    Utls utls;
    QSqlQuery query = utls.researchi("user_information_table", "userID=" + QString::number(userID) + " AND passWord='" + password + "'");

    if (query.first())
    {
        qDebug() << "Login successful for userID:" << userID;

        // Update login status
        QString updateSql = "UPDATE user_information_table SET loginStatus = 1 WHERE userID = :userID";
        QSqlQuery updateQuery;
        updateQuery.prepare(updateSql);
        updateQuery.bindValue(":userID", userID);
        if (!updateQuery.exec())
        {
            qDebug() << "Error updating login status for userID:" << userID;
        }

        return true;
    }
    else
    {
        qDebug() << "Login failed for userID:" << userID;
        return false;
    }
}

// 获取给定用户的个人信息(头像、用户ID、用户名)
//
// 参数:
// - userID: 要获取信息的用户ID
//
// 返回值:
// - 包含用户头像、ID和用户名的SQL查询结果
QSqlQuery Func::getUserInfo(int userID)
{
    //    Init init;
    QSqlQuery query;

    QString sql = "SELECT photo, userName FROM user_information_table WHERE userID = :userID";
    query.prepare(sql);

    query.bindValue(":userID", userID);
    query.exec();
    //    if (query.exec() && query.next())
    //    {
    //        QSqlQuery userInfoQuery;
    //        userInfoQuery.bindValue(":photo", query.value(0));
    //        userInfoQuery.bindValue(":userID", userID);
    //        userInfoQuery.bindValue(":userName", query.value(1));

    //       return userInfoQuery;
    //    }
    return query;
}

// 获取给定用户的在线好友
//
// 参数:
// - userID: 要获取在线好友的用户ID
//
// 返回值:
// - 包含所有在线好友ID的SQL查询结果
QSqlQuery Func::getOnlineFriends(int userID)
{
    //    Init init;
    //    QSqlDatabase db = init.createDatabase();
    QSqlQuery query;

    QString sql = "SELECT DISTINCT friendID FROM friend_relationship_table "
                  "WHERE userIP = :userID AND passed = 1 "
                  "AND friendID IN (SELECT userID FROM user_information_table WHERE loginStatus = 1)";
    query.prepare(sql);
    query.bindValue(":userID", userID);

    if (query.exec())
    {
        //        db.close();
        return query;
    }

    //    db.close();
    return QSqlQuery(); // Return an empty query if no online friends found;
}

// 获取给定用户的在线好友数量
//
// 参数:
// - userID: 要获取在线好友数量的用户ID
//
// 返回值:
// - 在线好友数量
int Func::getOnlineFriendsCount(int userID)
{
    //    Init init;
    //    QSqlDatabase db = init.createDatabase();
    QSqlQuery query;

    QString sql = "SELECT COUNT(DISTINCT friendID) FROM friend_relationship_table "
                  "WHERE userIP = :userID AND passed = 1 "
                  "AND friendID IN (SELECT userID FROM user_information_table WHERE loginStatus = 1)";
    query.prepare(sql);
    query.bindValue(":userID", userID);

    if (query.exec() && query.next())
    {
        int count = query.value(0).toInt();
        //        db.close();
        return count;
    }

    //    db.close();
    return 0; // Return 0 if no online friends found;
}

// 获取给定用户的未通过好友请求
//
// 参数:
// - userID: 要获取未通过请求的用户ID
//
// 返回值:
// - 包含未通过好友请求的SQL查询结果
QSqlQuery Func::getPendingFriendRequests(int userID)
{
    Utls utls;
    QSqlQuery query = utls.researchi("friend_relationship_table", "userIP=" + QString::number(userID) + " AND passed=0");

    return query;
}

// 获取给定群聊的成员
//
// 参数:
// - groupID: 要获取成员的群聊ID
//
// 返回值:
// - 包含群聊成员的SQL查询结果
QSqlQuery Func::getGroupChatMembers(int groupID)
{
    Utls utls;
    QSqlQuery query = utls.researchi("user_information_table", "loginStatus = 1");

    if (query.first())
    {
        QString sql = "SELECT userID FROM group_member_table "
                      "WHERE groupID = :groupID AND userID IN (SELECT userID FROM user_information_table WHERE loginStatus = 1)";
        QSqlQuery memberQuery;
        memberQuery.prepare(sql);
        memberQuery.bindValue(":groupID", groupID);
        if (memberQuery.exec())
        {
            return memberQuery;
        }
    }

    return QSqlQuery(); // Return an empty query if no members found;
}

// 获取两用户间的聊天历史记录（单聊）
//
// 参数:
// - userID1: 第一个用户的ID
// - userID2: 第二个用户的ID
//
// 返回值:
// - 包含两用户间聊天记录的SQL查询结果
QSqlQuery Func::getChatHistory(int userID1, int userID2)
{
    Utls utls;
    QString conditions = "sendID=" + QString::number(userID1) + " AND recvID=" + QString::number(userID2) + " OR sendID=" + QString::number(userID2) + " AND recvID=" + QString::number(userID1);
    QSqlQuery query = utls.researchi("message_table", conditions);

    return query;
}
