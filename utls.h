#ifndef UTLS_H
#define UTLS_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Utls
{
public:
    Utls();
    QString getColumnNames(QSqlQueryModel *model);
    bool creati(QString tableName,QString values);
//    values用单引号引好各个值，用英文逗号隔开
    bool deletei(QString tableName,QString conditions);
    bool updatei(QString tableName,QString values,QString conditions);
    QSqlQuery researchi(QString tableName,QString conditions);
};

#endif // UTLS_H
