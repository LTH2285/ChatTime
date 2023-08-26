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
};

#endif // UTLS_H
