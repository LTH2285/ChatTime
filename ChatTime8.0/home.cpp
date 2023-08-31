#include "home.h"
#include "ui_home.h"
#include <QListWidget>
#include "QListWidgetItem"
#include "widget.h"
#include "QMessageBox"
#include "chat.h"
#include "aichat.h"
#include "uishadow.h"
#include <user.h>
#include <protocol.h>
#include <QInputDialog>
#include "aipaint.h"
#include "friendrequest.h"
#include "groupchat.h"
#include <QCheckBox>
#include <QImage>

extern User user;
extern User fri;

//QString hostIp = "127.0.0.1";
QString hostIp = "192.168.46.173";
int hostPort = 8888;

Home::Home(const int &userid, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Home)
{
    ui->setupUi(this);
    tcpSocket = new QTcpSocket();
//    m_userid = userid;
    user.id = userid;

    Shadow *sha = new Shadow;
    sha->applyToWidget(ui->friendList);
    ui->onlineNumber->setText(QString("欢迎id为:"+ QString::number(user.id))+"的用户");
}

// 整点薯条
Home::~Home()
{
    tcpSocket->disconnectFromHost();
    delete ui;
}

void Home::welcome(){
    qDebug() << "welcome!!";
    Home::createInfo();
}


// 整点薯条
void Home::StartChat(){
    tcpSocket = new QTcpSocket();
    tcpSocket->abort();//取消已有链接
    tcpSocket->connectToHost(hostIp,hostPort);
    if(!tcpSocket->waitForConnected(3000))
    {
        //连接失败
        QMessageBox::warning(this,"Warning","Network Error",QMessageBox::Yes);
        //连接失败退回登录界面
        this->close();

        user.isonline = false;
        Widget *wid = new Widget();
        wid->show();
        delete this;
    }
    else
    {
        //服务器连接成功

        if(ui->friendList->count() == 0){
            QMessageBox::warning(this,"Warning","没好友",QMessageBox::Yes);
            tcpSocket->disconnectFromHost();
        }
        else if(!ui->friendList->currentItem()){
            QMessageBox::warning(this,"Warning","没选中",QMessageBox::Yes);
            tcpSocket->disconnectFromHost();
        }
        else {
            QString str = ui->friendList->currentItem()->text();
            if(str.startsWith("[群聊]"))
            {
                 QList<QString>parts=str.split("]");
                 fri.id = parts[1].toInt();
                 fri.username = "[群聊]";
                 qDebug()<<parts;
            }
            else
            {
                //QString str = "xyz(id:yz)\t\t[在线]";
                QList<QString>parts1 = str.split("(");
                fri.username = parts1[0];
                QList<QString>parts2 = parts1[1].split(":");
                qDebug()<<parts2;
                QList<QString>parts3 = parts2[1].split(")");
                qDebug()<<parts3;
                fri.id = parts3[0].toInt();
            }
            qDebug()<<"friendInfo"<<fri.username<<fri.id;


            if(ui->friendList->count() == 0){
                  QMessageBox::warning(this, "Warning!", "没好友", QMessageBox::Yes);
            }
            else{

                //建立信息头,发送给socket
                Protocol info;
                info.function_name="createChat";
                info.userid = user.id;
                info.friendid=fri.id;
                info.sendStructData(tcpSocket,info);

                connect(tcpSocket,&QTcpSocket::readyRead,[=](){
                   Protocol ansInfo;//读取所有socket中的所有信息备用
                   ansInfo.recvStructData(tcpSocket,ansInfo);
                   QString ansType = ansInfo.function_name;

                   if(ansInfo.function_name == QString("createChat_ok"))
                       //
                   {
                       //服务器说ok
                       QListWidgetItem *selectedItem = ui->friendList->currentItem();
                       //打开对话框
                       if(selectedItem){
                           QString contactName = selectedItem->text();
                           Chat *chat = new Chat();
                           chat->show();}
                   }
                   else  if(ansInfo.function_name == QString("createChat_ok_group"))
                   {
                       QListWidgetItem *selectedItem = ui->friendList->currentItem();
                       //打开群聊对话框
                       if(selectedItem){
                           QString groupName = selectedItem->text();
                           GroupChat *groupchat = new GroupChat();
                           groupchat->show();}
                   }
                   else
                   {
                       //服务器说error
                         QMessageBox::warning(this, "Warning!", "服务器错误", QMessageBox::Yes);
                   }

                });
            }

        }


    }

//    QListWidgetItem *selectedItem = ui->friendList->currentItem();

//    if(selectedItem){
//        int userid = selectedItem->text().trimmed().toInt();
//        this->close();

//        Chat *chat = new Chat(m_userid, userid, "sendusername", "recvusername");
//        chat->show();


//    }
//    else{
//        QMessageBox::warning(this, "Warning", "Please select a contact",QMessageBox::Yes);
//    }
}

void Home::createInfo(){
    ui->friendList->clear();

    if (tcpSocket) {
        tcpSocket->disconnectFromHost(); // 断开当前连接
        delete tcpSocket;
        tcpSocket = nullptr;
    }
    tcpSocket = new QTcpSocket();
    tcpSocket->connectToHost(hostIp, hostPort); // 链接服务器

    //ip:host
    QString ip = tcpSocket->peerAddress().toString().section(":",3,3);
    int port = tcpSocket->peerPort();
    QString str = QString("[%1:%2]").arg(ip).arg(port);
    qDebug() << str ;

    if(!tcpSocket->waitForConnected(3000))
       QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);

    else
    {
        if (tcpSocket->state() == QAbstractSocket::ConnectedState) {
            qDebug() << "开始view";
            //服务器连接成功
            Protocol sendinfo;
            sendinfo.function_name = "getinfo";
            sendinfo.userid= user.id;

            sendinfo.sendStructData(tcpSocket,sendinfo);

            connect(tcpSocket,&QTcpSocket::readyRead,[=](){
                qDebug() << "我的朋友呢？";

                Protocol recvinfo;
                recvinfo.recvStructData(tcpSocket,recvinfo);

                if(recvinfo.status==-1)
                {//未找到好友
                  //输出未找到
                    qDebug()<<"no friend found";
                }
                else
                {
                    QList<QString> nameList = recvinfo.sql;
                    QList<QByteArray> iconList = recvinfo.friendIcon;
                    //qDebug()<<"friend list"<<recvinfo.sql;
                    int size = nameList.size();
                    //int friendnum = recvinfo.status;
                    int iconNum=0;

                    for(int i=0;i<size;i++)
                    {
                          QListWidgetItem* item = new QListWidgetItem();
                        if(!nameList[i].startsWith("[群聊]"))
                        {
                            QImage loadedImage;
                            loadedImage.loadFromData(iconList[iconNum++], "JPEG");
                            QPixmap pixmap = QPixmap::fromImage(loadedImage).scaled(QSize(64, 64)); // 调整为32x32大小

                            QIcon icon(pixmap);
                            item->setIcon(icon);

                        }
                        QList<QString>parts = nameList[i].split("#%^&*@");
                        if(parts[2]=="在线状态:1")
                        {
                            item->setText(QString("%1(id:%2)\t\t[在线]").arg(parts[0]).arg(parts[1]));
                        }
                        else if(parts[2]=="在线状态:0")
                        {
                           QBrush greyBrush(Qt::gray);
                           item->setForeground(greyBrush);
                           item->setText(QString("%1(id:%2)\t\t[离线]").arg(parts[0]).arg(parts[1]));
                        }
                        else
                        {
                           item->setText(QString("[群聊]%1").arg(parts[1]));
                        }



                        //设置显示格式
                        ui->friendList->addItem(item);

                    }

                }

                QByteArray pic = recvinfo.ba;

                if(pic == ""){
                    qDebug()<<"empty";
                }
                else{
                    QImage loadedImage;
                    loadedImage.loadFromData(pic, "JPEG"); // 使用与保存时相同的格式
                    QPixmap pixmap = QPixmap::fromImage(loadedImage);
                    ui->label_2->setPixmap(pixmap);
                    ui->label_2->setScaledContents(true);
                }
                tcpSocket->disconnectFromHost();
            });
        }
        else {
             QMessageBox::warning(this, "Warning!", "无法连接到服务器", QMessageBox::Yes);
        }
    }
}

// 整点薯条
void Home::on_logoutBtn_clicked()
{
    if (tcpSocket) {
        tcpSocket->disconnectFromHost(); // 断开当前连接
        delete tcpSocket;
        tcpSocket = nullptr;
    }

    tcpSocket = new QTcpSocket();

    tcpSocket->connectToHost(hostIp, hostPort); // 链接服务器

    QString ip = tcpSocket->peerAddress().toString().section(":",3,3);
    int port = tcpSocket->peerPort();
    QString str = QString("[%1:%2]").arg(ip).arg(port);
    qDebug() << str ;
    if(!tcpSocket->waitForConnected(30000))
    {
        QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
    }
    else{
        Protocol sendinfo;
        sendinfo.function_name = "logout";
        sendinfo.userid=user.id;
        // sendinfo.userid = 1;
        qDebug() << "logout的userid" <<sendinfo.userid;

        sendinfo.sendStructData(tcpSocket, sendinfo);
        connect(tcpSocket, &QTcpSocket::readyRead,[=](){
            Protocol recvinfo;
            qDebug() << recvinfo.function_name << recvinfo.status;
            recvinfo.recvStructData(tcpSocket,recvinfo);
            qDebug() << recvinfo.function_name << recvinfo.status;
            if(recvinfo.status == 1){
                qDebug()<<"logout successfully";
                this->close();
                Widget *login = new Widget;
                login->show();
                delete this;
            }
            else{
                QMessageBox::warning(this, "Warning", "Logout error", QMessageBox::Yes);
            }
        });
    }
}

void Home::on_startChatBtn_clicked()
{
    StartChat();
}

void Home::on_AiChatStartBtn_clicked()
{
    Aichat *w = new Aichat(user.id);
    w->show();
}

void Home::on_friendList_itemDoubleClicked(QListWidgetItem *item)
{
    StartChat();
}

void Home::on_addFriendBtn_clicked()
{
    FriendRequest *friReq = new FriendRequest(user.id);
    friReq->show();
}

void Home::on_aipaint_clicked()
{
    AiPaint *h = new AiPaint(user.id);
    h->show();
}

void Home::on_refreshBtn_clicked()
{

    Home *hom = new Home(user.id);
    hom->show();
    hom->welcome();
    //QMessageBox::information(this, "提示!", "刷新成功！", QMessageBox::Yes);
    delete this;
}
//创建群聊
void Home::on_groupChatBtn_clicked()
{
    tcpSocket = new QTcpSocket();
    tcpSocket->abort();//取消已有链接
    tcpSocket->connectToHost(hostIp,hostPort);//链接服务器
    if(!tcpSocket->waitForConnected(3000))
    {
       QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
    }
    else
    {
        //服务器连接成功
        Protocol sendinfo;
        sendinfo.function_name = "getinfo";
        sendinfo.userid= user.id;

        sendinfo.sendStructData(tcpSocket,sendinfo);
        connect(tcpSocket,&QTcpSocket::readyRead,[=](){

            Protocol recvinfo;
            recvinfo.recvStructData(tcpSocket,recvinfo);
            if(recvinfo.status == -1)
            {//未找到好友
              //输出未找到
                qDebug()<<"no friend found";

            }
            else
            {
                QList<QString> options;
                for (const QString& item : recvinfo.sql) {
                    if (!item.startsWith("[群聊]")) {
                        options.append(item);
                    }
                }

                //qDebug()<<"friend list"<<recvinfo.sql;


                QDialog dialog;
                   QVBoxLayout *layout = new QVBoxLayout(&dialog);

                   QList<QCheckBox*> checkBoxes;

                   for (const QString& option : options) {
                       QList<QString>parts = option.split("#%^&*@");
                       QString str=QString("%1 id:%2").arg(parts[0]).arg(parts[1]);
                       QCheckBox *checkBox = new QCheckBox(str);
                       layout->addWidget(checkBox);
                       checkBoxes.append(checkBox);
                   }

                   QPushButton *okButton = new QPushButton("确定");
                   layout->addWidget(okButton);

                   // 点击确定按钮时触发槽函数
                   QObject::connect(okButton, &QPushButton::clicked, [&]() {
                       QStringList selectedOptions;
                       for (QCheckBox *checkBox : checkBoxes) {
                           if (checkBox->isChecked()) {
                               selectedOptions.append(checkBox->text());
                           }
                       }
                       qDebug() << "Selected options:" << selectedOptions;

                       //将选中的信息发送给服务器
                       this->createGroup(selectedOptions);

                       // 关闭对话框
                       dialog.accept();
                   });

                   dialog.exec();

            }
            tcpSocket->disconnectFromHost();
        });
    }
}

void Home::createGroup(QList<QString> selectedOptions){
    tcpSocket = new QTcpSocket();
    tcpSocket->abort();//取消已有链接
    tcpSocket->connectToHost(hostIp,hostPort);//链接服务器
    if(!tcpSocket->waitForConnected(3000))
    {
       QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
    }
    else
    {
        Protocol reqInfo;
        reqInfo.function_name = "createGroup";
        reqInfo.userid = user.id;
        reqInfo.sql = selectedOptions;
        reqInfo.sendStructData(tcpSocket,reqInfo);

        connect(tcpSocket,&QTcpSocket::readyRead,[=](){
           Protocol ansInfo;//读取所有socket中的所有信息备用
           ansInfo.recvStructData(tcpSocket,ansInfo);
           if(ansInfo.function_name=="createGroup_null")
           {
               QMessageBox::warning(this,"Warning!","不能创建只有自己的群聊哦",QMessageBox::Yes);
           }
           else if(ansInfo.function_name=="createGroup_ok")
           {
               QMessageBox::information(this,"Infomation","群聊创建成功！",QMessageBox::Yes);
           }
           else
           {
               QMessageBox::warning(this,"Warning!","群聊创建失败！",QMessageBox::Yes);
           }
         });
    }
}
