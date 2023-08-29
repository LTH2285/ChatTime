#include "Init.h"
#include "func.h"
#include "myserver.h"
#include "ui_myserver.h"
#include "utls.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <protocol.h>
#include <QObject>


int currentsize = 0;

myServer::myServer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::myServer)
{
    ui->setupUi(this);
    tcpServer = new QTcpServer();

    ui->listWidget->clear();
    ui->listWidget->insertItem(0,tr("当前无在线用户"));
    for(int i = 0; i < M; i++)
    {
        tcpSocket[i]=new QTcpSocket();
    }
    tcpServer->listen(QHostAddress::Any,8888);
    QHostAddress serverAddress = tcpServer->serverAddress();
    QString serverIp = serverAddress.toString();
    qDebug() << "服务器IP地址：" << serverIp;

    Init init;
    Utls utls;
    QSqlDatabase db = init.createDatabase();

    connect(tcpServer,&QTcpServer::newConnection,this,&myServer::onMyConnect);
}

void myServer::onMyConnect(){

    tcpSocket[0] = tcpServer->nextPendingConnection();
    currentsize++;
    QString ip = tcpSocket[0]->peerAddress().toString().section(":",3,3);
    int port = tcpSocket[0]->peerPort();
    //预留currentsize以用作多用户同时连接所用
    QString str = QString("[%1:%2]").arg(ip).arg(port);
    qDebug() << str ;

    connect(tcpSocket[0],&QTcpSocket::readyRead,[=](){
        //读取缓冲区数据
        Protocol recvinfo;
        recvinfo.recvStructData(tcpSocket[0],recvinfo);
        qDebug() << "请求种类" << recvinfo.function_name << recvinfo.userid;

        if("login" == recvinfo.function_name)
            login(tcpSocket,recvinfo);
        else if ("register" == recvinfo.function_name)
            registeruser(tcpSocket,recvinfo);
        else if("addFriend" == recvinfo.function_name)
            addFriend(tcpSocket,recvinfo);
        else if("createChat" == recvinfo.function_name)
            createChat(tcpSocket,recvinfo);
        else if("sendMessage" == recvinfo.function_name)
            sendMessage(tcpSocket,recvinfo);
        else if("chatHistory" == recvinfo.function_name)
            chatHistory(tcpSocket,recvinfo);
        else if("viewFriends" == recvinfo.function_name)
            viewFriends(tcpSocket,recvinfo);
        else{
            qDebug() << "zen me shier?";
        }
    });
}

void myServer::login(QTcpSocket* tcpSocket[],Protocol recvinfo){
    Protocol sendinfo;
    //登陆

    Func func;
    bool loginstatus = func.loginFunction(recvinfo.userid, recvinfo.userpwd);

    if(!loginstatus){
        sendinfo.status = -1;
        sendinfo.sendStructData(tcpSocket[0],sendinfo);
    }
    else{
        qDebug() << "登陆成功!";
        sendinfo.status = 2;
        sendinfo.sendStructData(tcpSocket[0], sendinfo);

        Utls utls;
        utls.updatei("user_information_table","loginStatus = 1","userID=" + QString::number(recvinfo.userid));

        tcpSocket[0]->flush();

        //更新服务器界面
        ui->listWidget->clear();

        QSqlQuery sqlquery = utls.researchi("user_information_table", "loginStatus = 1");

        if(sqlquery.next())
        {
            QString userid = sqlquery.value(0).toString();
            QString username = sqlquery.value(1).toString();
//            QString userip = sqlquery.value(3).toString();
            //qDebug()<<userid;
            ui->listWidget->insertItem(0,"用户ID："+userid+",用户昵称:"+username);
            int rownum = 1;
            while (sqlquery.next())
            {
                QString userid = sqlquery.value(0).toString();
                QString username = sqlquery.value(1).toString();
                ui->listWidget->insertItem(rownum,"用户ID："+userid+",用户昵称:"+username);
                rownum++;
            }
        }
    }
}

void myServer::registeruser(QTcpSocket* tcpSocket[],Protocol recvinfo){
    //注册环节
    db.setDatabaseName("./people.db");
    db.open();
    QSqlQuery sqlquery;
    //注册用户的时候需要进行判重
    sqlquery.prepare("select * from people where name = :name");
    sqlquery.bindValue(":name",recvinfo.username);
    sqlquery.exec();
    if(!sqlquery.next())
    {
        //可以新建
        sqlquery.clear();
        Func func;
        int userid = func.account_register(recvinfo.username,recvinfo.userpwd);
//        sqlquery.prepare("insert into people values (null,:name,:password,null,0)");
//        sqlquery.bindValue(":name",recvinfo.username);
//        sqlquery.bindValue(":password",recvinfo.userpwd);
//        sqlquery.exec();

//        sqlquery.clear();
//        sqlquery.prepare("select * from people where name = :name");
//        sqlquery.bindValue(":name",recvinfo.username);
//        sqlquery.exec();//获得新建的用户的id
//        sqlquery.next();
//        int newid = sqlquery.value(0).toInt();
        sqlquery.exec("create table if not exists friend__" + QString::number(userid) +"(id INTEGER unique, name TEXT,sendmassage INTEGER,sendfile INTEGER)");

        Protocol sendinfo;
        sendinfo.msg = "register successed";
        sendinfo.status = 1;
        sendinfo.sendStructData(tcpSocket[0],sendinfo);

        db.close();
    }
    else
    {
        //有重名
        Protocol sendinfo;
        sendinfo.status = -1;
        sendinfo.msg = "register error!!same_name";
        sendinfo.sendStructData(tcpSocket[0],sendinfo);
        db.close();
    }
}

void myServer::chatHistory(QTcpSocket* tcpSocket[],Protocol recvinfo){
    int userId = recvinfo.userid;
    int friendId = recvinfo.friendid;
    //int friendId = 3;//测试用的后门！
    // 在数据库中查找聊天记录
    Protocol ansInfo;
    Func func;
    QSqlQuery queryRes = func.getChatHistory(userId,friendId);
    //
    if(queryRes.first()){
        ansInfo.function_name="chatHistory_ok";
        //查找的结果不为空,把查找结果存起来
        while(queryRes.next()){
            QString value1 = queryRes.value(0).toString(); // 获取第一列的值，并转换为 QString 类型 发送者
            QString value2 = queryRes.value(1).toString(); // 获取第二列的值，并转换为 QString 类型 接收者
            QString value3 = queryRes.value(2).toString(); // 获取第三列的值，并转换为 QString 类型 消息
            QString value4 = queryRes.value(3).toString(); // 获取第四列的值，并转换为 QString 类型 时间戳
            QString msg = QString("%1 \n %2 to %3: %4\n").arg(value4).arg(value1).arg(value2).arg(value3);
            ansInfo.sql.append(msg); // 将第一列的值添加到 QList 中

        }

    }
    else{
        ansInfo.function_name="chatHistory_empty";
        //查找的结果为空
    }
    ansInfo.sendStructData(tcpSocket[0],ansInfo);
}

void myServer::addFriend(QTcpSocket* tcpSocket[],Protocol recvinfo)
{
    int userId = recvinfo.userid;
    int friendId = recvinfo.friendid;
    qDebug()<<"user id"<<recvinfo.userid;
    qDebug()<<"friend id"<<recvinfo.friendid;
    Protocol ansInfo;
    //在用户表中查询friendId
    Func func;
    if(!func.checkUserIDExist(friendId))
    {
    //不存在该用户
        ansInfo.function_name="addFriend_error";

    }
    //在好友关系表中查询是否有好友关系
    else if(func.checkFriendship(userId,friendId))
    {
    //已经是好友
        ansInfo.function_name="addFriend_already";
    }
    else
    {
        ansInfo.function_name="addFriend_ok";
        //向数据库中添加这条好友请求
        func.sendFriendRequest(userId,friendId);
    }
    qDebug()<<"返回给数据库的信息类型"<<ansInfo.function_name;
    ansInfo.sendStructData(tcpSocket[0],ansInfo);

}

void myServer::createChat(QTcpSocket* tcpSocket[],Protocol recvinfo){
    Protocol ansInfo;

    int userId = recvinfo.userid;
    Func func;
    int friendId = recvinfo.friendid;
    qDebug()<<"userId"<<recvinfo.userid<<"friendId"<<recvinfo.friendid;

    //在好友表中查找friendId

    if(func.checkFriendship(userId,friendId))
    {
        //找到了
        ansInfo.function_name="createChat_ok";
        ansInfo.status = 1;
    }
    else
    {
        //不是好友(按道理说不会出现这种情况，因为发送这个请求是friendId一定是好友的)
        ansInfo.function_name="createChat_error";
        ansInfo.status = -1;
    }
         ansInfo.sendStructData(tcpSocket[0],ansInfo);
}

void myServer::sendMessage(QTcpSocket* tcpSocket[],Protocol recvinfo){
    //接收：发送名、接受名、消息、时间戳

    int sendId = recvinfo.userid;
    int recvId = recvinfo.friendid;
    //recvId = 3;
    QString tim = recvinfo.tim;
    QString message = recvinfo.msg;

    //将这条消息储存到库中即可
    Func func;
    func.insertNewMessage(sendId,recvId,message,tim);
}

void myServer::viewFriends(QTcpSocket* tcpSocket[],Protocol recvinfo)
{
    Func func;
    QList<FriendInfo>friendList = func.getUserFriendsWithInfo(recvinfo.userid);
    Protocol sendinfo;
    if(!friendList.empty())
    {


        for (const FriendInfo& friendInfo : friendList) {
            // 将 FriendInfo 结构体转换为字符串格式
            QString friendInfoString =  friendInfo.friendUsername+ "#" +
                                       QString::number(friendInfo.friendID)+ "# 在线状态:" +
                                       QString::number(friendInfo.isOnline);
            sendinfo.sql.append(friendInfoString);
        }

        //qDebug()<<userid;

        //回传,status内存好友个数
        sendinfo.status=friendList.size();
        sendinfo.function_name="viewFriends_ok";
           }
    else
    {
        //未找到好友
        sendinfo.status=-1;
        sendinfo.function_name="viewFriends_null";
    }
    sendinfo.sendStructData(tcpSocket[0],sendinfo);


}

myServer::~myServer()
{
    tcpServer->close();
    tcpServer->deleteLater();
    delete ui;
}
