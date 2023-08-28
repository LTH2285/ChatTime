#include "home.h"
#include "ui_home.h"
#include <QListWidget>
#include "QListWidgetItem"
#include "widget.h"
#include "QMessageBox"
#include "chat.h"
#include "aichat.h"
#include "uishadow.h"

// 创建好友列表
QList<QString> contactList = {"fuyu", "wangzhihe", "luoyang"};

Home::Home(QWidget *parent) : QWidget(parent),
                              ui(new Ui::Home)
{
    ui->setupUi(this);

    // 设置阴影
    Shadow *sha = new Shadow;
    sha->applyToWidget(ui->friendList);

    // 创建好友列表
    Home::CreateFriendList();
    ui->onlineNumber->setText(QString("now %1 users active").arg(contactList.length()));
}

Home::~Home()
{
    delete ui;
}
// 开始聊天
void Home::StartChat()
{
    // 获取选中的好友名称
    QListWidgetItem *selectedItem = ui->friendList->currentItem();

    // 如果有选中的好友
    if (selectedItem)
    {
        QString contactName = selectedItem->text();
        // 关闭当前窗口
        this->close();
        // 创建聊天窗口
        Chat *chat = new Chat(contactName);
        chat->show();
    }
    // 如果没有选中的好友
    else
    {
        // 弹出警告框
        QMessageBox::warning(this, "Warning", "Please select a contact", QMessageBox::Yes);
    }
}
// 创建好友列表
void Home::CreateFriendList()
{
    // link server
    ui->friendList->addItems(contactList);
}

// 点击登出按钮
void Home::on_logoutBtn_clicked()
{
    // 关闭当前窗口
    this->close();
    // 创建登出窗口
    Widget *login = new Widget;
    login->show();
}

// 点击开始聊天按钮
void Home::on_startChatBtn_clicked()
{
    // 开始聊天
    StartChat();
}
// 点击开始AI聊天按钮
void Home::on_AiChatStartBtn_clicked()
{
    // 关闭当前窗口
    this->close();
    // 创建AI聊天窗口
    Aichat *w = new Aichat();
    w->show();

    void Home::on_AiChatStartBtn_clicked()
}
// 点击好友列表转发按钮
void Home::on_friendList_itemDoubleClicked(QListWidgetItem *item)
{
    this->close();
    Aichat *w = new Aichat();
    w->show();

    // 开始聊天
    StartChat();
}

void Home::on_friendList_itemDoubleClicked(QListWidgetItem *item)
{
    StartChat();
}