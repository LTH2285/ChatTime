#include "groupchat.h"
#include "ui_groupchat.h"

GroupChat::GroupChat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupChat)
{
    ui->setupUi(this);
}

GroupChat::~GroupChat()
{
    delete ui;
}
