#include "func.h"
#include "utls.h"
#include <QDebug>
#include <Init.h>
#include <QCryptographicHash>
#include <QFile>
#include <QList>
#include <QFileDialog>
#include <QDateTime>
#include <QBuffer>

Func::Func()
{
}

//查询用户图片
QList<QByteArray> Func::findAllUserImages(int userID)
{
    QList<QByteArray> images;

    Utls utls;
    QSqlQuery query;
    query.prepare("SELECT image FROM user_image_table WHERE userID = :userID");
    query.bindValue(":userID", userID);
    query.exec();

    while (query.next()) {
        QByteArray imageData = query.value(0).toByteArray();
        images.append(imageData);
    }

    return images;
}

//插入用户图片
bool Func::insertUserImage(int userID, const QByteArray &imageData)
{

    Utls utls;

    // 使用参数绑定方式插入 BLOB 数据
    QSqlQuery query;
    query.prepare("UPDATE user_image_table SET image = :photoData WHERE userID = :userID");
    query.bindValue(":photoData", imageData);
    query.bindValue(":userID", userID);

    if (query.exec()) {
        qDebug() << "Photo inserted successfully for userID:" << userID;
        return true;
    } else {
        qDebug() << "Error inserting photo for userID:" << userID;
        return false;
    }
//    Utls utls;

//    // 使用参数绑定方式插入图片数据
//    QSqlQuery query;
//    query.prepare("INSERT INTO user_image_table (userID) VALUES (:userID)");
//    query.bindValue(":userID", userID);

//    query.exec();

//    query.prepare("UPDATE user_image_table SET photo = :photoData WHERE userID = :userID");
//    query.bindValue(":photoData", imageData);
//    query.bindValue(":userID", userID);

//    if (query.exec()) {
//        qDebug() << "User image inserted successfully for userID:" << userID;
//        return true;
//    } else {
//        qDebug() << "Error inserting image for userID:" << userID;
//        return false;
//    }
}

bool Func::insertPhoto(int userID, const QByteArray &photoData)
{
    Utls utls;

    // 使用参数绑定方式插入 BLOB 数据
    QSqlQuery query;
    query.prepare("UPDATE user_information_table SET photo = :photoData WHERE userID = :userID");
    query.bindValue(":photoData", photoData);
    query.bindValue(":userID", userID);

    if (query.exec()) {
        qDebug() << "Photo inserted successfully for userID:" << userID;
        return true;
    } else {
        qDebug() << "Error inserting photo for userID:" << userID;
        return false;
    }
}

//查询用户头像
QByteArray Func::getPhoto(int userID)
{
    Utls utls;

    // 查询用户的照片数据
    QSqlQuery query = utls.researchi("user_information_table", "userID=" + QString::number(userID));
    if (query.next()) {
        QByteArray photoData = query.value("photo").toByteArray();
        return photoData;
    } else {
        qDebug() << "User not found with userID:" << userID;
        return QByteArray(); // Return an empty QByteArray if user not found
    }
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
int Func::account_register(const QString &username, const QString &password, int question, const QString &answer)
{
    Utls utls;
    int existingrows = utls.getRowNumbers("user_information_table");
    if (existingrows < 0) {
        qDebug() << "Error fetching existing row numbers.";
        return -1; // Error code
    }

    int id = existingrows + 1;

    // 使用哈希函数对密码进行加密
    QString hashedPassword = hashPassword(password);

    QString values = QString("%1, '%2', '%3',0,NULL, %4, '%5'")
                        .arg(id).arg(username).arg(hashedPassword)
                        .arg(question).arg(answer);

    bool status = utls.creati("user_information_table", values);
    if (!status) {
        qDebug() << "Error registering the account.";
        return -1; // Error code
    } else {
        qDebug() << "Successfully registered the account with ID:" << id;
        return id; // Return the ID of the newly registered account
    }
}

QString Func::hashPassword(const QString &password)
{
    // 使用 QCryptographicHash 进行哈希加密
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(password.toUtf8()); // 将密码转换为字节流并哈希
    QString hashedPassword = QString(hash.result().toHex()); // 获取哈希结果，并转换为十六进制字符串
    return hashedPassword;
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

    // 对输入的密码进行哈希加密
    QString hashedPassword = hashPassword(password);

    // 构建查询条件，查询用户ID和加密后的密码是否匹配
    QString conditions = "userID=" + QString::number(userID) + " AND passWord='" + hashedPassword + "'";
    QSqlQuery query = utls.researchi("user_information_table", conditions);

    if (query.first()) {
        qDebug() << "Login successful for userID:" << userID;

        // 更新登录状态
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
    QSqlQuery query = utls.researchi("friend_relationship_table", "friendID =" + QString::number(userID) + " AND passed = 0");
    qDebug()<<"查询出的好友申请"<<query.value(0).toString();
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

    if (query.exec()) {
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

QSqlQuery Func::getChatHistory_Group(int groupID)
{
    Utls utls;
    QString conditions =  "recvID=" + QString::number(groupID);

    QSqlQuery query = utls.researchi("message_table", conditions);

    return query;
}

//罗洋需求功能新加，有一些功能已经在上面给出，如果没在下面找到请去上面看看~

//判断好友关系是否存在
bool Func::checkFriendship(int userID, int friendID)
{
    Utls utls;

    // 构建查询条件以检查好友关系是否存在
    QString conditions = "(userID = " + QString::number(userID) + " AND friendID = " + QString::number(friendID) + " AND passed = 1)  OR "+"(userID = " + QString::number(friendID) + " AND friendID = " + QString::number(userID) + " AND passed = 1)";
    //qDebug()<<conditions;
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
    // 创建 Utls 实例，用于数据库操作
    Utls utls;
    QString values = QString("%1, %2").arg(groupID).arg(friendID);
    QString values2 = QString("%1, %2, %3").arg(groupID).arg(friendID).arg(1);  //视为已经通过的好友
       if (utls.creati("friend_relationship_table", values2) ) {
           utls.creati("group_member_table", values);
           qDebug() << "Successfully invited friend to group chat.";
            // 返回成功标志
       } else {
           qDebug() << "Error inviting friend to group chat.";
            // 返回错误码
       }

}

//获取用户姓名
QString Func::getUserName(int userID)
{
    Utls utls;
    QSqlQuery query = utls.researchi("user_information_table", "userID = " + QString::number(userID));

    if (query.next()) {
        return query.value("userName").toString();
    } else {
        return "Unknown User";
    }
}

//导出聊天记录到本地
bool Func::exportSingleChatHistoryToFile(int userID, int friendID)
{
    Utls utls;

    // 获取聊天记录
    QSqlQuery query = getChatHistory(userID, friendID);

    QList<ChatMessage> chatHistory;

    while (query.next()) {
        ChatMessage message;
        message.senderID = query.value("sendID").toInt();
        message.recvID = query.value("recvID").toInt();
        message.message = query.value("message").toString();
        message.sendTime = query.value("sendTime").toString();
        chatHistory.append(message);
    }

    // 构建聊天记录字符串
    QString chatHistoryText;
    for (const ChatMessage &message : chatHistory) {
        QString senderName = getUserName(message.senderID);
        chatHistoryText += "[" + message.sendTime + "] " + senderName + ": " + message.message + "\n";
    }

    // 构建文件名（使用当前日期和时间）
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString fileName = QString("chat_history_%1.txt").arg(currentDateTime.toString("yyyyMMdd_hhmmss"));

    // 保存聊天记录到文件
    QString filePath = "../" + fileName; // 设置保存路径
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << chatHistoryText;
        file.close();
        qDebug() << "Chat history exported to:" << filePath;
        return true;
    } else {
        qDebug() << "Error exporting chat history.";
        return false;
    }
}


//导出群聊聊天记录到本地
//bool Func::exportGroupChatHistoryToFile(int groupID, const QString &filePath)
//传递用户选择的文件路径作为参数 filePath
//{
//    Utls utls;

//    // 查询群聊的聊天记录
//    QString conditions = "recvID = " + QString::number(groupID) + " ORDER BY sendTime ASC";
//    QSqlQuery chatQuery = utls.researchi("message_table", conditions);

//    // 检查是否有聊天记录
//    if (chatQuery.isActive() && chatQuery.first()) {
//        QFile file(filePath);
//        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
//            QTextStream stream(&file);
//            // 逐行写入聊天记录到文件
//            do {
//                QString sendTime = chatQuery.value("sendTime").toString();
//                QString message = chatQuery.value("message").toString();
//                QString senderName = chatQuery.value("senderName").toString();
//                QString line = sendTime + " " + senderName + ": " + message;
//                stream << line << endl;
//            } while (chatQuery.next());

//            file.close();
//            qDebug() << "Group chat history exported to file:" << filePath;
//            return true;
//        } else {
//            qDebug() << "Error opening file for writing:" << filePath;
//            return false;
//        }
//    } else {
//        qDebug() << "No chat history available for group:" << groupID;
//        return false;
//    }
//}

//查询用户的密保问题
int Func::getUserSecurityQuestion(int userID)
{
    Utls utls;
    QSqlQuery result = utls.researchi("user_information_table", QString("userID = %1").arg(userID)); // Pass an empty QString as the second argument

    if (result.next()) {
        int securityQuestion = result.value("question").toInt();
        return securityQuestion;
    } else {
        return -1; // Return an empty string if user not found or no question available
    }
}



//找回密码
bool Func::recoverPassword(int userID, const QString &answer, const QString &newPassword)
{
    Utls utls;

    // 查询用户的密保问题和答案
    QSqlQuery query = utls.researchi("user_information_table", "userID=" + QString::number(userID));
    if (query.next()) {
        //int storedQuestion = query.value("question").toInt();
        QString storedAnswer = query.value("answer").toString();

        // 比对密保答案
        if (answer == storedAnswer) {
            // 使用哈希函数对新密码进行加密
            QString hashedPassword = hashPassword(newPassword);

            // 更新数据库中的密码
            QString updateSql = "UPDATE user_information_table SET passWord = :hashedPassword WHERE userID = :userID";
            QSqlQuery updateQuery;
            updateQuery.prepare(updateSql);
            updateQuery.bindValue(":hashedPassword", hashedPassword);
            updateQuery.bindValue(":userID", userID);

            if (updateQuery.exec()) {
                qDebug() << "Password successfully updated for userID:" << userID;
                return true;
            } else {
                qDebug() << "Error updating password for userID:" << userID;
                return false;
            }
        } else {
            qDebug() << "Incorrect answer to security question.";
            return false;
        }
    } else {
        qDebug() << "User not found with userID:" << userID;
        return false;
    }
}

