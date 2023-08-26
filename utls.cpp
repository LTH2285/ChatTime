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

