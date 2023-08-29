#ifndef IMSERVER_H
#define IMSERVER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class IMServer; }
QT_END_NAMESPACE

class IMServer : public QWidget
{
    Q_OBJECT

public:
    IMServer(QWidget *parent = nullptr);
    ~IMServer();

private:
    Ui::IMServer *ui;
};
#endif // IMSERVER_H
