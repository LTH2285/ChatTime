#include "utls.h"

Utls::Utls()
{
}

QString Utls::getColumnNames(QSqlQueryModel *model)
{
  QString columnNames;
  for (int i = 0; i < model->columnCount(); i++)
  {
    if (i > 0)
    {
      columnNames.append(", ");
    }
    QString headerText = model->headerData(i, Qt::Horizontal).toString();
    columnNames.append(headerText);
  }
  return columnNames;
}

bool Utls::creati(QString tableName, QString values)
{
    QSqlQueryModel model;
    model.setQuery("SELECT * FROM "+ tableName);
    QString cols = getColumnNames(&model);
    QString sql = "INSERT INTO " + tableName + " (" + cols + ") VALUES(" + values + ")";
    QSqlQuery query;
    return query.exec(sql);
}

bool Utls::updatei(QString tableName, QString values, QString conditions)
{
    QSqlQueryModel model;
    model.setQuery("SELECT * FROM "+ tableName);
    QString cols = getColumnNames(&model);
    QString sql = "UPDATE " + tableName + " SET " + values + " WHERE " + conditions;
    QSqlQuery query;
    return query.exec(sql);
}

bool Utls::deletei(QString tableName, QString conditions)
{
    QSqlQueryModel model;
    model.setQuery("SELECT * FROM "+ tableName);
    QString cols = getColumnNames(&model);
    QString sql = "DELETE FROM " + tableName + " WHERE " + conditions;
    QSqlQuery query;
    return query.exec(sql);
}

QSqlQuery Utls::researchi(QString tableName, QString conditions)
{
    QSqlQueryModel model;
    model.setQuery("SELECT * FROM "+ tableName);
    QString cols = getColumnNames(&model);
    QString sql = "SELECT * FROM " + tableName + " WHERE " + conditions;
    QSqlQuery query;
    query.exec(sql);
    return query;
}
