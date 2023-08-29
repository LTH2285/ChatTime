#include "imserver.h"
#include "ui_imserver.h"

IMServer::IMServer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::IMServer)
{
    ui->setupUi(this);
}

IMServer::~IMServer()
{
    delete ui;
}

