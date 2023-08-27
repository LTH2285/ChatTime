#ifndef FUNC_H
#define FUNC_H

#include <QString>
#include <QSqlQuery>

class Func
{
public:
    Func();
    bool loginFunction(const int userID, const QString &password);
    bool checkUserIDExist(int userID);
    int account_register(const QString username, const QString password);
    bool loginFunction(int userID, const QString password);
    QSqlQuery getUserInfo(int userID);
    QSqlQuery getOnlineFriends(int userID);
    int getOnlineFriendsCount(int userID);
    QSqlQuery getPendingFriendRequests(int userID);
    QSqlQuery getGroupChatMembers(int groupID);
    QSqlQuery getChatHistory(int userID1, int userID2);
};

#endif // FUNC_H
