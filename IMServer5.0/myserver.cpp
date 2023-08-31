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
        else if("logout" == recvinfo.function_name)
            logout(tcpSocket,recvinfo);
        else if("wantSendFile" == recvinfo.function_name)
            wantSendFile(tcpSocket,recvinfo);
        else if("reset1" == recvinfo.function_name)
            resetstep1(tcpSocket,recvinfo);
        else if("reset2" == recvinfo.function_name)
            resetstep2(tcpSocket,recvinfo);
        else if("writephoto" == recvinfo.function_name)
            writephoto(tcpSocket,recvinfo);
        else if("getinfo" == recvinfo.function_name)
            getinfo(tcpSocket,recvinfo);
        else if("viewFriApp" == recvinfo.function_name)
            viewFriApp(tcpSocket,recvinfo);
        else if("processFriApp" == recvinfo.function_name)
            processFriApp(tcpSocket,recvinfo);
        else if("createGroup" == recvinfo.function_name)
            createGroup(tcpSocket,recvinfo);
        else if("chatHistory_Group"== recvinfo.function_name)
            chatHistory_Group(tcpSocket,recvinfo);
        else if("addPic" == recvinfo.function_name)
            addPic(tcpSocket,recvinfo);
        else if("detect" == recvinfo.function_name)
            detect(tcpSocket,recvinfo);
        else
            qDebug() << "zen me shier?";
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
        int userid = func.account_register(recvinfo.username,recvinfo.userpwd,recvinfo.friendid,recvinfo.msg);
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
        sendinfo.userid = userid;
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

    // 在数据库中查找聊天记录
    Protocol ansInfo;
    Func func;
    QSqlQuery queryRes = func.getChatHistory(userId,friendId);
    //

    if(queryRes.size()){
        ansInfo.function_name="chatHistory_ok";
        //查找的结果不为空,把查找结果存起来

        while (queryRes.next()) {
            QString value1 = queryRes.value(0).toString(); // 获取第一列的值，并转换为 QString 类型 发送者
            QString value2 = queryRes.value(1).toString(); // 获取第二列的值，并转换为 QString 类型 接收者
            QString value3 = queryRes.value(2).toString(); // 获取第三列的值，并转换为 QString 类型 消息
            QString value4 = queryRes.value(3).toString(); // 获取第四列的值，并转换为 QString 类型 时间戳
            QString msg = QString("%1 \n %2 to %3: %4\n").arg(value4).arg(value1).arg(value2).arg(value3);
            ansInfo.sql.append(msg); // 将值添加到 QList 中

            ansInfo.userip = value4; // 用userid储存最后一条消息的时间戳

        }
    }
    else{
        ansInfo.function_name="chatHistory_empty";
        //查找的结果为空
    }
    qDebug()<<"最后的时间戳:"<<ansInfo.userip;
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
        if(friendId<9000){
            ansInfo.function_name="createChat_ok";
        }else{
            ansInfo.function_name="createChat_ok_group";
        }

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
//    recvId = 3;
    QString tim = recvinfo.tim;
    QString message = recvinfo.msg;

    //将这条消息储存到库中即可
    Func func;
    func.insertNewMessage(sendId,recvId,message,tim);
}

void myServer::logout(QTcpSocket* tcpSocket[],Protocol recvinfo)
{
    Protocol sendinfo;
    //sendinfo.islogin=0;
    sendinfo.status = 1;
    sendinfo.sendStructData(tcpSocket[0], sendinfo);


    Utls utls;
    qDebug()<<"wochualaile";
    utls.updatei("user_information_table","loginStatus = 0","userID=" + QString::number(recvinfo.userid));
    qDebug()<<"wozoule";
    //utls.updatei("user_information_table","loginStatus = 0","userID=" + QString::number(recvinfo.userid));


    tcpSocket[0]->flush();
    //更新服务器界面
    ui->listWidget->clear();
    QSqlQuery sqlquery = utls.researchi("user_information_table", "loginStatus = 1");
    if(sqlquery.next())
    {
        QString userid = sqlquery.value(0).toString();
        QString username = sqlquery.value(1).toString();
        //QString userip = sqlquery.value(3).toString();
        //qDebug()<<userid;
        ui->listWidget->insertItem(0,"用户ID："+userid+",用户昵称:"+username);
        int rownum = 1;
        while (sqlquery.next())
        {
            QString userid = sqlquery.value(0).toString();
            QString username = sqlquery.value(1).toString();
            //QString userip = sqlquery.value(3).toString();
            ui->listWidget->insertItem(rownum,"用户ID："+userid+",用户昵称:"+username);
            rownum++;
        }
    }
    else
    {
        ui->listWidget->clear();
        ui->listWidget->insertItem(0,tr("当前无在线用户"));
    }
}

void myServer::wantSendFile(QTcpSocket* tcpSocket[],Protocol recvinfo){
    //1给2发,1是id,2是name;在2的表中显示1的信息
    int sendId = recvinfo.userid;
    int recvId = recvinfo.friendid;
    qDebug() << sendId << recvId;
    db.setDatabaseName("./people.db");
    db.open();
    Func func;
    func.insertNewMessage(recvinfo.userid,recvinfo.friendid,QString("给爷接收文件！"),recvinfo.tim);
}

void myServer::resetstep1(QTcpSocket *tcpSocket[], Protocol recvinfo){

    Func func;
    int questionid = func.getUserSecurityQuestion(recvinfo.userid);
    qDebug() << "useid:" << recvinfo.userid;
    qDebug() << "question id:" << questionid;
    Protocol sendinfo;
    sendinfo.userid = recvinfo.userid;
    sendinfo.serverport = questionid;
    sendinfo.sendStructData(tcpSocket[0],sendinfo);
}

void myServer::resetstep2(QTcpSocket *tcpSocket[], Protocol recvinfo){
    Func func;
    bool isok = func.recoverPassword(recvinfo.userid,recvinfo.msg,recvinfo.userpwd);

    qDebug() << recvinfo.msg << recvinfo.userid;

    Protocol sendinfo;
    sendinfo.userid = recvinfo.userid;
    sendinfo.msg = recvinfo.msg;

    if(isok){
        if(recvinfo.userpwd == recvinfo.friendip){
            sendinfo.status = 1;
        }else{
            sendinfo.status = 0;
        }
    }
    else
        sendinfo.status = -1;

    sendinfo.sendStructData(tcpSocket[0],sendinfo);
}

void myServer::getinfo(QTcpSocket *tcpSocket[], Protocol recvinfo){
    Func func;
        QList<FriendInfo>friendList = func.getUserFriendsWithInfo(recvinfo.userid);
        Protocol sendinfo;
        if(!friendList.empty())
        {


            for (const FriendInfo& friendInfo : friendList) {
                // 将 FriendInfo 结构体转换为字符串格式
                if(friendInfo.friendID>9000){
                    QString friendInfoString =  "[群聊]#" +
                                               QString::number(friendInfo.friendID)+ "#";
                    sendinfo.sql.append(friendInfoString);

                }
                else{
                    QString friendInfoString =  friendInfo.friendUsername+ "#" +
                                               QString::number(friendInfo.friendID)+ "# 在线状态:" +
                                               QString::number(friendInfo.isOnline);

                    sendinfo.sql.append(friendInfoString);

                }

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

void myServer::writephoto(QTcpSocket* tcpSocket[],Protocol recvinfo){
    Protocol sendinfo;

    Func sql;

    qDebug()<<recvinfo.ba;

    bool status = sql.insertPhoto(recvinfo.userid, recvinfo.ba);

    if(status){
        sendinfo.status = 1;

    }
    else{
        sendinfo.status = -1;
    }

    sendinfo.sendStructData(tcpSocket[0],sendinfo);
}

void myServer::viewFriApp(QTcpSocket **tcpSocket, Protocol recvinfo)
{Func func;
    QSqlQuery friApp = func.getPendingFriendRequests(recvinfo.userid);

    Protocol ansInfo;
    //把QSqlQuery转为QList
    if(!friApp.exec()){
        //无好友请求
        ansInfo.status = -1;
        ansInfo.function_name = "nullFriApp";
    }
    else{
        ansInfo.status = 0;
        ansInfo.function_name = "existFriApp";
        while(friApp.next())
        {
            QString application = friApp.value(0).toString()+":想成为你的好友";
            ansInfo.sql.append(application);

            ansInfo.status++;
        }

    }
        ansInfo.sendStructData(tcpSocket[0],ansInfo);
}

void myServer::processFriApp(QTcpSocket **tcpSocket, Protocol recvinfo)
{
    Protocol sendinfo;
    int sendid = recvinfo.friendid;
    int recvid = recvinfo.userid;
    int status = recvinfo.status;
    // 1 accept, -1 refuse
    Utls sql;
    QSqlQuery query;
    QString condition = "userID="+QString::number(sendid)+" AND friendID="+QString::number(recvid);
    qDebug()<<condition;
    if(status == -1){
        query = sql.researchi("friend_relationship_table",condition);
        if(query.exec()){
            sql.deletei("friend_relationship_table","userID="+condition);
            sendinfo.status = 1;
            sendinfo.function_name = "requestRefuse";
        }
        else{
            sendinfo.status = -1;
            sendinfo.function_name = "no_such_request";
        }
    }
    else{//status = 1
        query = sql.researchi("friend_relationship_table",condition);
        Func func;
        if(query.exec()){
            sql.updatei("friend_relationship_table","passed=1",condition);
            sendinfo.status = 1;
            sendinfo.function_name = "requestAccept";
            //func.insertReadInformation(sendid,recvid,0);

        }
        else{
            sendinfo.status = -1;
            sendinfo.function_name = "no_such_request";
        }


    }


    sendinfo.sendStructData(tcpSocket[0],sendinfo);


}

void myServer::createGroup(QTcpSocket **tcpSocket, Protocol recvinfo)
{
    QList<QString> groupMem = recvinfo.sql;

    Protocol ansInfo;

    if(groupMem.empty()){
        ansInfo.function_name= "createGroup_null";

    }
    else{
        Func func;
        int groupId= func.creategroupchat(recvinfo.userid);
        if(groupId==-1){
            ansInfo.function_name= "createGroup_error";
        }
        else{
            func.addFriendToGroup(groupId,recvinfo.userid);
            ansInfo.function_name= "createGroup_ok";
            for (const QString& member : groupMem){
                QList<QString>parts=member.split("#");
                int memId = parts[1].toInt();
                func.addFriendToGroup(groupId,memId);
        }


        }
    }
    ansInfo.sendStructData(tcpSocket[0],ansInfo);

}

void myServer::chatHistory_Group(QTcpSocket **tcpSocket, Protocol recvinfo)
{
    //int userId = recvinfo.userid;
    int groupId = recvinfo.friendid;

    // 在数据库中查找聊天记录
    Protocol ansInfo;
    Func func;
    QSqlQuery queryRes = func.getChatHistory_Group(groupId);
    //
    ansInfo.userip = "0.0.0";
    if(queryRes.size()){
        ansInfo.function_name="chatHistory_ok";
        //查找的结果不为空,把查找结果存起来

        while (queryRes.next()) {
            QString value1 = queryRes.value(0).toString(); // 获取第一列的值，并转换为 QString 类型 发送者
            //QString value2 = queryRes.value(1).toString(); // 获取第二列的值，并转换为 QString 类型 接收者
            QString value3 = queryRes.value(2).toString(); // 获取第三列的值，并转换为 QString 类型 消息
            QString value4 = queryRes.value(3).toString(); // 获取第四列的值，并转换为 QString 类型 时间戳
            QString msg = QString("%1 \n %2 : %4\n").arg(value4).arg(value1).arg(value3);
            ansInfo.sql.append(msg); // 将值添加到 QList 中

            ansInfo.userip = value4; // 用userid储存最后一条消息的时间戳

        }
    }
    else{
        ansInfo.function_name="chatHistory_empty";
        //查找的结果为空
    }
    qDebug()<<"最后的时间戳:"<<ansInfo.userip;
    ansInfo.sendStructData(tcpSocket[0],ansInfo);

}

void myServer::addPic(QTcpSocket *tcpSocket[], Protocol recvinfo){
    Protocol sendinfo;

    Func sql;

    qDebug()<<recvinfo.ba;

    bool status = sql.insertUserImage(recvinfo.userid, recvinfo.ba);

    if(status){
        sendinfo.status = 1;
    }
    else{
        sendinfo.status = -1;
    }

    sendinfo.sendStructData(tcpSocket[0],sendinfo);
}

void myServer::detect(QTcpSocket *tcpSocket[], Protocol recvinfo){

    qDebug()<<recvinfo.ba;

    Func sql;
    QList<QByteArray> images = sql.findAllUserImages(recvinfo.userid);
    qDebug() << images.size();

    Protocol sendinfo;
    sendinfo.friendIcon = images;

    if(images.size()!=0){
        sendinfo.status = 1;
    }
    else{
        sendinfo.status = -1;
    }
    sendinfo.sendStructData(tcpSocket[0],sendinfo);
}

myServer::~myServer()
{
    tcpServer->close();
    tcpServer->deleteLater();
    delete ui;
}
