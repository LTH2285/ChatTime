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

    QSqlDatabase createDatabase();
    bool createTable();
};

#endif // INIT_H
