#ifndef USER_H
#define USER_H

#include <QObject>

class User
{
public:
    int id;
    QString username;
    bool isonline; //login or not
    QString ip;
    User();
};

#endif // USER_H
