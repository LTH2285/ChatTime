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
  // 创建一个查询模型
  QSqlQueryModel model;
  // 查询表中的所有列
  model.setQuery("SELECT * FROM " + tableName);
  // 获取模型中的列名
  QString cols = getColumnNames(&model);
  // 构建SQL语句
//  QString sql = "INSERT INTO " + tableName + " (" + cols + ") VALUES(" + values + ")";
  QString sql = QString("INSERT INTO %1 (%2) VALUES(%3)").arg(tableName,cols,values);

  // 创建一个查询对象
  QSqlQuery query;
  // 打印SQL语句
  qDebug() << "Executing SQL:" << sql;
  // 打印查询错误信息
  qDebug() << "Query error:" << query.lastError().text();
  // 执行SQL语句
  return query.exec(sql);
}

bool Utls::updatei(QString tableName, QString values, QString conditions)
{
  // 创建一个查询模型
  QSqlQueryModel model;
  // 查询表中的所有列
  model.setQuery("SELECT * FROM " + tableName);
  // 获取模型中的列名
  QString cols = getColumnNames(&model);
  // 构建查询语句
//  QString sql = "UPDATE " + tableName + " SET " + values + " WHERE " + conditions;
  QString sql = QString("UPDATE %1 SET %2 WHERE %3").arg(tableName,values,conditions);

  // 创建查询对象
  QSqlQuery query;

  // 执行查询语句
  return query.exec(sql);
}

bool Utls::deletei(QString tableName, QString conditions)
{
  // 创建一个查询模型
  QSqlQueryModel model;
  // 查询表中的所有数据
  model.setQuery("SELECT * FROM " + tableName);
  // 获取模型中的列名
  QString cols = getColumnNames(&model);
  // 构建查询语句
//  QString sql = "DELETE FROM " + tableName + " WHERE " + conditions;
  QString sql = QString("DELETE FROM %1 WHERE %2").arg(tableName,conditions);
  // 创建查询对象
  QSqlQuery query;
  // 执行查询
  return query.exec(sql);
}

QSqlQuery Utls::researchi(QString tableName, QString conditions)
{
  // 创建查询模型
  QSqlQueryModel model;
  // 查询表中的所有数据
  model.setQuery("SELECT * FROM " + tableName);
  // 获取查询模型中的列名
  QString cols = getColumnNames(&model);
  // 构建查询语句
//  QString sql = "SELECT * FROM " + tableName + " WHERE " + conditions;
  QString sql = QString("SELECT * FROM %1 WHERE %2").arg(tableName,conditions);
  // 创建查询对象
  QSqlQuery query;
  // 执行查询语句
  query.exec(sql);
  // 返回查询对象
  return query;
}

int Utls::getRowNumbers(QString tableName)
{
  // 创建一个查询对象
  QSqlQuery query;
  // 构建查询语句
  QString sql = QString("SELECT COUNT(*) FROM %1").arg(tableName);

  // 执行查询语句
  if (query.exec(sql))
  {
    // 如果查询成功，则获取第一条记录的值
    if (query.next())
    {
      return query.value(0).toInt();
    }
  }
  else
  {
    // 如果查询失败，则打印错误信息
    qDebug() << "Error getting row count:" << query.lastError().text();
  }

  // 返回-1表示查询失败
  return -1;
}

