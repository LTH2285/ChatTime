///////chat.cpp
#include "aiaudio.h"
#include "audio.h"
#include "speech.h"
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
#include "screenwidget.h"
#include <QScrollBar>

extern QString hostIp;
extern int hostPort;
extern User user;
extern User fri;

Chat::Chat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chat)
{

    ui->setupUi(this);
    this->setWindowTitle(QString("Chat with %1").arg(fri.username));
    timer = new QTimer();
    tcpSocket =new QTcpSocket();
    lastUpdateTime = "0:0:0";
    //时刻读取历史聊天记录达到实时刷新的效果
    connect(timer,SIGNAL(timeout()),this,SLOT(getChatHistory()));
    timer->start(500);
    qDebug()<<"timer is running";
}


Chat::~Chat()
{
    timer->stop();
    delete ui;
     tcpSocket->abort();
    delete this;
}

void Chat::closeEvent(QCloseEvent *event){
    timer->stop();
    this->close();
    tcpSocket->abort();
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
           connect(tcpSocket,&QTcpSocket::readyRead,[=](){
              Protocol ansInfo;
              ansInfo.recvStructData(tcpSocket,ansInfo);
         });

       }

        //ui->chatHistory->append( "I chat to " + m_contactname + " :" + message);


    }
    else
    {
         QMessageBox::warning(this, "Warning!", "消息不能为空", QMessageBox::Yes);
    }
//    QString message = ui->chatInput->text();
//    if(!message.trimmed().isEmpty()){
//        ui->chatHistory->append( "I chat to " + m_recvusername + " :" + message);
//    }
    tcpSocket->disconnect();
    this->getChatHistory();
}

void Chat::getChatHistory()
{


    tcpSocket->abort();//取消已有链接
    tcpSocket->connectToHost(hostIp, hostPort);//链接服务器

    if(!tcpSocket->waitForConnected(3000))
    {
        QMessageBox::warning(this, "Warning!", "Network Error", QMessageBox::Yes);
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
           qDebug()<<ansInfo.function_name<<ansInfo.userip;


            if(ansInfo.function_name == QString("chatHistory_ok")&&this->lastUpdateTime!=ansInfo.userip)
            {
                lastUpdateTime =ansInfo.userip;
                qDebug()<<"消息记录已刷新";
                ui->chatHistory->clear();
                //服务器说ok
                //将读取的信息按一定格式显示在TextEdit上
                //ansInfo.//ansInfo内要存储数据库查询sqlQyery的格式

                for(int i = 0;i<ansInfo.sql.size();i++){
                    ui->chatHistory->insertPlainText(ansInfo.sql[i]);
                }

                QScrollBar *scrollBar = ui->chatHistory->verticalScrollBar();
                scrollBar->setValue(scrollBar->maximum());

                ui->chatInput->clear();
                qDebug()<<ansInfo.sql;
            }
            else
            {
                qDebug()<<"暂无新消息";
            }
            tcpSocket->disconnect();
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
        Protocol sendinfo;
        sendinfo.function_name = "wantSendFile";
        sendinfo.userid = user.id;
        sendinfo.friendid = fri.id;

        // 获取当前时间的时间戳
        qint64 currentTimestamp = QDateTime::currentSecsSinceEpoch();
        // 将时间戳转换为 QDateTime 对象
        QDateTime currentDateTime;
        currentDateTime.setSecsSinceEpoch(currentTimestamp);

        // 将 QDateTime 对象转换为 QString
        QString strTimestamp = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");

        sendinfo.tim = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
        sendinfo.sendStructData(tcpSocket,sendinfo);
        tcpSocket->disconnect();
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

void Chat::on_screenshot_clicked()
{
//    this->close();
    ScreenWidget *s = new ScreenWidget(true);
    s->showFullScreen();
}

void Chat::on_saveHistoryBtn_clicked()
{
    QString textContent = ui->chatHistory->toPlainText();
    if(textContent!=""){
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Text Files (*.txt);;All Files (*)"));

        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly)) {
                QTextStream stream(&file);
                stream << textContent;
                file.close();
            } else {
                QMessageBox::critical(this, tr("Error"), tr("Could not save file!"));
            }
        }

    }
    else{
        QMessageBox::warning(this, "Warning!", "empty history", QMessageBox::Yes);
    }


}

void Chat::on_aiAudio_pressed()
{
    audio = new Audio;
    audio->startAudio("1.pcm");
}

void Chat::on_aiAudio_released()
{
    //停止录音
    audio->stopAudio();
    //开始识别
    speech m_speech;
    QString text = m_speech.speechIdentify("1.pcm");

    ui->chatInput->setText(text);
}
