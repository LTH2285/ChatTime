#ifndef UTLS_H
#define UTLS_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>

class Utls
{
public:
    Utls();
    QString getColumnNames(QSqlQueryModel *model);
    int getRowNumbers(QString tableName);
    bool creati(QString tableName,QString values);
    bool deletei(QString tableName,QString conditions);
    bool updatei(QString tableName,QString values,QString conditions);
    QSqlQuery researchi(QString tableName,QString conditions);
};

#endif // UTLS_H
