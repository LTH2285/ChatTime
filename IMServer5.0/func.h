#ifndef FUNC_H
#define FUNC_H

#include <QString>
#include <QSqlQuery>

// 声明 FriendInfo 结构体
struct FriendInfo {
    int friendID;
    QString friendUsername;
    bool isOnline;
};

// 定义一个结构体来表示聊天消息
struct ChatMessage {
    int senderID;
    int recvID;
    QString message;
    QString sendTime;
};

class Func
{
public:
    Func();

    bool checkUserIDExist(int userID);
    int account_register(const QString &username, const QString &password, int question, const QString &answer);
    QString hashPassword(const QString &password);
    bool loginFunction(int userID, const QString password);
    QSqlQuery getUserInfo(int userID);
    QSqlQuery getOnlineFriends(int userID);
    int getOnlineFriendsCount(int userID);
    QSqlQuery getPendingFriendRequests(int userID);
    QSqlQuery getGroupChatMembers(int groupID);
    QSqlQuery getChatHistory(int userID1, int userID2);
    bool checkFriendship(int userID, int friendID);
    bool sendFriendRequest(int userID, int friendID);
    bool deleteFriendAndChat(int userID, int friendID);
    bool insertNewMessage(int sendID, int recvID, const QString &message, const QString &sendTime);
    int creategroupchat(int userID);
    bool addFriendToGroup(int groupID, int friendID);
    QList<FriendInfo> getUserFriendsWithInfo(int userID);
    QString getUserName(int userID);
    bool exportSingleChatHistoryToFile(int userID, int friendID);
    bool exportGroupChatHistoryToFile(int groupID, const QString &filePath);
    int getUserSecurityQuestion(int userID);
    bool recoverPassword(int userID, const QString &answer, const QString &newPassword);
    bool insertPhoto(int userID, const QByteArray &photoData);
    QByteArray getPhoto(int userID);
    QSqlQuery getChatHistory_Group(int groupID);

};

#endif // FUNC_H
