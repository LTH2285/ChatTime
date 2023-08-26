#ifndef INIT_H
#define INIT_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>

class Init : public QObject
{
    Q_OBJECT
public:
    explicit Init(QObject *parent = nullptr);

    bool createDatabase();
    bool createUserTable();
};

#endif // INIT_H
