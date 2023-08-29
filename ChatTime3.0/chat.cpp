#include "chat.h"
#include "ui_chat.h"
#include "home.h"
#include <QApplication>
#include <QListWidget>
#include "fileoperation.h"
#include "facecharacter.h"
#include <QMessageBox>
#include "user.h"
#include "widget.h"
#include "protocol.h"
#include "receivefiledialog.h"

extern QString hostIp;
extern int hostPort;
extern User user;
extern User fri;

Chat::Chat(const int sendid, const int recvid, const QString sendusername, const QString recvusername, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chat),
    m_sendid(sendid),
    m_recvid(recvid),
    m_sendusername(sendusername),
    m_recvusername(recvusername)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Chat with %1").arg(m_recvusername));
    timer = new QTimer();
    //时刻读取历史聊天记录达到实时刷新的效果
    connect(timer,SIGNAL(timeout()),this,SLOT(getChatHistory()));
    timer->start(500);
    qDebug()<<"timer is running111";
}

Chat::Chat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Chat with %1").arg(m_recvusername));
    timer = new QTimer();
    tcpSocket =new QTcpSocket();
    //时刻读取历史聊天记录达到实时刷新的效果
    connect(timer,SIGNAL(timeout()),this,SLOT(getChatHistory()));
    timer->start(500);
    qDebug()<<"timer is running";
}


Chat::~Chat()
{
//    timer->stop();
    delete ui;
    delete this;
}

void Chat::on_pushButton_clicked()
{
    timer->stop();
    this->close();
    tcpSocket->abort();
}

//sendMessage
void Chat::on_chatSendBtn_clicked()
{
    //发送消息
    QString message = ui->chatInput->text();
    if(!message.trimmed().isEmpty()){
            //输入框不为空

       tcpSocket =new QTcpSocket();
       tcpSocket->abort();
       tcpSocket->connectToHost(hostIp,hostPort);
       //打印服务器的ip和端口信息
//           QString ip = tcpSocket->peerAddress().toString().section(":",3,3);
//           int port = tcpSocket->peerPort();
//       QString str = QString("[%1:%2]").arg(ip).arg(port);
//       qDebug()<<str;
       if(!tcpSocket->waitForConnected(3000))
       {
           //连接超时
           QMessageBox::warning(this,"Warning!","Network Error!",QMessageBox::Yes);
           this->close();
           user.isonline = false;
           Widget *wid = new Widget();
           wid->show();
       }
       else
       {
            //创建信息头:发送名、接受名、消息、时间戳
//           QString messageInfo = QString("sendMessage|%1|%2|%3").arg(user.id).arg(fri.id).arg(message);
//           tcpSocket->write(messageInfo.toUtf8());
//           tcpSocket->flush();
           Protocol reqInfo;
           reqInfo.function_name = "sendMessage";
           reqInfo.userid = user.id;
           reqInfo.friendid = fri.id;
           // 获取当前时间的时间戳
           qint64 currentTimestamp = QDateTime::currentSecsSinceEpoch();

           // 将时间戳转换为 QDateTime 对象
           QDateTime currentDateTime;
           currentDateTime.setSecsSinceEpoch(currentTimestamp);

           // 将 QDateTime 对象转换为 QString
           QString strTimestamp = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");

           reqInfo.tim = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
           reqInfo.msg = message;
           reqInfo.sendStructData(tcpSocket,reqInfo);

       }

        //ui->chatHistory->append( "I chat to " + m_contactname + " :" + message);
        ui->chatInput->clear();
    }
    else
    {
         QMessageBox::warning(this, "Warning!", "消息不能为空", QMessageBox::Yes);
    }
//    QString message = ui->chatInput->text();
//    if(!message.trimmed().isEmpty()){
//        ui->chatHistory->append( "I chat to " + m_recvusername + " :" + message);
//    }
    this->getChatHistory();
}

void Chat::getChatHistory()
{
    qDebug()<<"消息记录已刷新";
    tcpSocket->abort();//取消已有链接
    tcpSocket->connectToHost(hostIp, hostPort);//链接服务器

    if(!tcpSocket->waitForConnected(3000))
    {
        QMessageBox::warning(this, "Warning!", "Networ Error", QMessageBox::Yes);
        this->close();
    }
    else
    {//服务器连接成功

        // 发送给服务器的信息头
        Protocol reqInfo;
        reqInfo.function_name = "chatHistory";
        reqInfo.userid = user.id;
        reqInfo.friendid = fri.id;
        reqInfo.sendStructData(tcpSocket,reqInfo);

        connect(tcpSocket,&QTcpSocket::readyRead,[=](){
           Protocol ansInfo;
           ansInfo.recvStructData(tcpSocket,ansInfo);
            if(ansInfo.function_name == QString("chatHistory_ok"))
            {
                ui->chatHistory->clear();
                //服务器说ok
                //将读取的信息按一定格式显示在TextEdit上
                //ansInfo.//ansInfo内要存储数据库查询sqlQyery的格式

                for(int i = 0;i<ansInfo.sql.size();i++){
                    ui->chatHistory->insertPlainText(ansInfo.sql[i]);
                }
                qDebug()<<ansInfo.sql;
            }
            else if(ansInfo.function_name == QString("chatHistory_empty"))
            {
                ui->chatHistory->setText("无消息记录");
            }
        });
    }
}

void Chat::on_emojiBtn_clicked()
{
    FaceCharacter *w = new FaceCharacter(this);
    connect(w, &FaceCharacter::emojiSelected,this,&Chat::inserEmojiIntoLineEdit);
    w->show();
}

void Chat::inserEmojiIntoLineEdit(QString emoji){
    QString currentText = ui->chatInput->text();
    ui->chatInput->setText(currentText + emoji);
}

void Chat::on_sendfile_clicked()
{
    tcpSocket = new QTcpSocket();
    if (tcpSocket) {
        tcpSocket->disconnectFromHost(); // 断开当前连接
        delete tcpSocket;
        tcpSocket = nullptr;
    }

    tcpSocket = new QTcpSocket();

    tcpSocket->connectToHost(hostIp, hostPort); // 链接服务器

    if(!tcpSocket->waitForConnected(30000))
    {
        QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
        this->close();

    }
    else
    {//服务器连接成功
        QString message = QString("want_send_file##%1##%2").arg("1001").arg("hihi");
        tcpSocket->write(message.toUtf8());
        tcpSocket->flush();
    }

    fileOperation *w = new fileOperation;
    w->show();
    //    sendfiledialog *sf = new sendfiledialog();
    //    sf->show();
}

void Chat::on_recvfile_clicked()
{
    receivefiledialog *rf = new receivefiledialog();
    rf->show();
}
