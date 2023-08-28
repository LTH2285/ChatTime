#include "func.h"
#include "utls.h"
#include <QDebug>
#include <Init.h>

Func::Func()
{
}

//查询用户IP是否存在
bool Func::checkUserIDExist(int userID)
{
    Utls utls;
    QSqlQuery query = utls.researchi("user_information_table", "userID=" + QString::number(userID));

    if (query.first()) {
        qDebug() << "UserID exists:" << userID;
        return true;
    } else {
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
    if (existingrows < 0) {
        qDebug() << "Error fetching existing row numbers.";
        return -1; // Error code
    }

    int id = existingrows + 1;
    QString values = QString("%1, '%2', '%3', NULL").arg(id).arg(username).arg(password);
    bool status = utls.creati("user_information_table", values);
    if (!status) {
        qDebug() << "Error registering the account.";
        return -1; // Error code
    } else {
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

    if (query.first()) {
        qDebug() << "Login successful for userID:" << userID;

        // Update login status
        QString updateSql = "UPDATE user_information_table SET loginStatus = 1 WHERE userID = :userID";
        QSqlQuery updateQuery;
        updateQuery.prepare(updateSql);
        updateQuery.bindValue(":userID", userID);
        if (!updateQuery.exec()) {
            qDebug() << "Error updating login status for userID:" << userID;
        }

        return true;
    } else {
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

    if (query.first()) {
        QString sql = "SELECT userID FROM group_member_table "
                      "WHERE groupID = :groupID AND userID IN (SELECT userID FROM user_information_table WHERE loginStatus = 1)";
        QSqlQuery memberQuery;
        memberQuery.prepare(sql);
        memberQuery.bindValue(":groupID", groupID);
        if (memberQuery.exec()) {
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
    QString conditions = "sendID=" + QString::number(userID1) + " AND recvID=" + QString::number(userID2)
                         + " OR sendID=" + QString::number(userID2) + " AND recvID=" + QString::number(userID1);
    QSqlQuery query = utls.researchi("message_table", conditions);

    return query;
}

//罗洋需求功能新加，有一些功能已经在上面给出，如果没在下面找到请去上面看看~

//判断好友关系是否存在
bool Func::checkFriendship(int userID, int friendID)
{
    Utls utls;

    // 构建查询条件以检查好友关系是否存在
    QString conditions = "userIP = " + QString::number(userID) + " AND friendID = " + QString::number(friendID);
    QSqlQuery query = utls.researchi("friend_relationship_table", conditions);

    return query.first(); // 如果好友关系存在，返回 true；否则返回 false
}

//查询好友列表，包括好友ID、好友用户名和好友在线状态
QList<FriendInfo> Func::getUserFriendsWithInfo(int userID)
{
    Utls utls;
    QList<FriendInfo> friendList;

    // 查询userID作为userID时的好友关系
    QString conditions1 = "userID = " + QString::number(userID) + " AND passed = 1";
    QSqlQuery query1 = utls.researchi("friend_relationship_table", conditions1);

    while (query1.next()) {
        int friendID = query1.value("friendID").toInt();

        // 检查是否已经存在相同friendID的好友信息
        bool exists = false;
        for (const FriendInfo &existingFriend : friendList) {
            if (existingFriend.friendID == friendID) {
                exists = true;
                break;
            }
        }

        if (!exists) {
            FriendInfo friendInfo;
            friendInfo.friendID = friendID;

            QSqlQuery userInfoQuery = utls.researchi("user_information_table", "userID = " + QString::number(friendID));
            if (userInfoQuery.next()) {
                friendInfo.friendUsername = userInfoQuery.value("userName").toString();
                friendInfo.isOnline = userInfoQuery.value("loginStatus").toInt() == 1;
            }

            friendList.append(friendInfo);
        }
    }

    // 查询userID作为friendID时的好友关系
    QString conditions2 = "friendID = " + QString::number(userID) + " AND passed = 1";
    QSqlQuery query2 = utls.researchi("friend_relationship_table", conditions2);

    while (query2.next()) {
        int friendID = query2.value("userID").toInt();

        // 检查是否已经存在相同friendID的好友信息
        bool exists = false;
        for (const FriendInfo &existingFriend : friendList) {
            if (existingFriend.friendID == friendID) {
                exists = true;
                break;
            }
        }

        if (!exists) {
            FriendInfo friendInfo;
            friendInfo.friendID = friendID;

            QSqlQuery userInfoQuery = utls.researchi("user_information_table", "userID = " + QString::number(friendID));
            if (userInfoQuery.next()) {
                friendInfo.friendUsername = userInfoQuery.value("userName").toString();
                friendInfo.isOnline = userInfoQuery.value("loginStatus").toInt() == 1;
            }

            friendList.append(friendInfo);
        }
    }

    return friendList;
}



//插入好友申请信息
bool Func::sendFriendRequest(int userID, int friendID)
{
    Utls utls;

    // 构建插入数据的值
    QString values = QString("'%1', '%2', %3").arg(userID).arg(friendID).arg(0); // 初始状态为未通过

    // 插入数据
    bool status = utls.creati("friend_relationship_table", values);

    return status;
}

//删除好友关系信息(同时删除二者间的聊天记录)
bool Func::deleteFriendAndChat(int userID, int friendID)
{
    Utls utls;

    QSqlQueryModel model;
    model.setQuery("SELECT * FROM friend_relationship_table");
    QString cols = utls.getColumnNames(&model);

    // 删除好友关系信息
    QString deleteFriendQuery = "DELETE FROM friend_relationship_table WHERE (userID = :userID AND friendID = :friendID) OR (userID = :friendID AND friendID = :userID)";
    QSqlQuery deleteFriendQueryObject;
    deleteFriendQueryObject.prepare(deleteFriendQuery);
    deleteFriendQueryObject.bindValue(":userID", userID);
    deleteFriendQueryObject.bindValue(":friendID", friendID);
    if (!deleteFriendQueryObject.exec()) {
        qDebug() << "Error deleting friend relationship:" << deleteFriendQueryObject.lastError().text();
        return false;
    }

    // 删除聊天记录
    QString deleteChatQuery = "DELETE FROM message_table WHERE (sendID = :userID AND recvID = :friendID) OR (sendID = :friendID AND recvID = :userID)";
    QSqlQuery deleteChatQueryObject;
    deleteChatQueryObject.prepare(deleteChatQuery);
    deleteChatQueryObject.bindValue(":userID", userID);
    deleteChatQueryObject.bindValue(":friendID", friendID);
    if (!deleteChatQueryObject.exec()) {
        qDebug() << "Error deleting chat history:" << deleteChatQueryObject.lastError().text();
        return false;
    }

    qDebug() << "Friendship and chat history deleted for userID:" << userID << "and friendID:" << friendID;
    return true;
}

//插入新消息记录
bool Func::insertNewMessage(int sendID, int recvID, const QString &message, const QString &sendTime)
{
    Utls utls;
    QString values = QString("%1, %2, '%3', '%4'").arg(sendID).arg(recvID).arg(message).arg(sendTime);
    return utls.creati("message_table", values);
}

//创建群聊
int Func::creategroupchat(int userID)
{
    // 创建一个 Utls 类的实例以操作数据库
    Utls utls;

    // 获取现有群聊数量，以确定新群聊的群ID
    int existingRows = utls.getRowNumbers("group_info_table");
    if (existingRows == -1) {
        qDebug() << "Error fetching existing row numbers.";
        return -1; // 返回错误代码
    }

    int groupID = 9001 + existingRows; // 计算新群ID

    // 将新群聊信息插入 group_info_table 表
    QString values = QString("%1, NULL, %2").arg(groupID).arg(userID);
    bool status = utls.creati("group_info_table", values);
    if (!status) {
        qDebug() << "Error creating group chat.";
        return -1; // 返回错误代码
    } else {
        qDebug() << "Successfully created group chat, Group ID:" << groupID;
        return groupID; // 返回新创建的群ID
    }
}

//邀请好友入群
bool Func::addFriendToGroup(int groupID, int friendID)
{
    Utls utls;

    // 首先检查群聊中是否已经存在该好友
    QSqlQuery query = utls.researchi("group_member_table", "groupID=" + QString::number(groupID) + " AND userID=" + QString::number(friendID));
    if (query.next()) {
        qDebug() << "Friend already in the group chat.";
        return false; // 好友已存在，操作失败
    }

    // 将好友加入群聊
    QString values = QString("%1, %2").arg(groupID).arg(friendID);
    if (utls.creati("group_member_table", values)) {
        qDebug() << "Successfully added friend to group chat.";
        return true; // 成功将好友加入群聊
    } else {
        qDebug() << "Error adding friend to group chat.";
        return false; // 操作失败
    }
}


