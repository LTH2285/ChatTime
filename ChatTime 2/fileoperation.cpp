#include "fileoperation.h"
#include "ui_fileoperation.h"
#include <QFileDialog>
#include <QThread>
#include <QMessageBox>
#include "sendfile.h"
fileOperation::fileOperation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fileOperation)
{
    ui->setupUi(this);
    ui->ip->setText("127.0.0.1");
    ui->port->setText("8989");
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);


    //创建线程对象
    QThread* t =new QThread;
    //创建任务对象
    SendFile* worker = new SendFile;

    worker->moveToThread(t);

    connect(this,&fileOperation::sendFile,worker,&SendFile::sendFile);
    connect(this,&fileOperation::startConnect,worker,&SendFile::connectServer);
   //处理主线程发送的信号

    connect(worker,&SendFile::connectedOK,this,[=](){
        QMessageBox::information(this,"连接服务器","已经成功连接了服务器，恭喜！");
    });

    connect(worker,&SendFile::gameover,this,[=](){
        //资源释放
        t->quit();
        t->wait();
        worker->deleteLater();
        t->deleteLater();
    });
    connect(worker,&SendFile::curPercent,ui->progressBar,&QProgressBar::setValue);
    t->start();
}

fileOperation:: ~fileOperation()
{
    delete ui;
}



void fileOperation::on_selectFileBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("All Files (*);;Text Files (*.txt)"));
    if(fileName.isEmpty())
    {
        QMessageBox::warning(this,"打开文件","选择的文件路径不能为空!");
        return;
    }
    ui->fileNameInput->insert(fileName);
}

void fileOperation::on_sendBtn_clicked()
{
    if(ui->progressBar->value()!=100){

    }
    else{
        //send
    }
    emit sendFile(ui->fileNameInput->text());
}

void fileOperation::on_cancelBtn_clicked()
{
    this->close();
}

void fileOperation::on_connectServer_clicked()
{
    QString ip=ui->ip->text();
    unsigned short port =ui->port->text().toUShort();
    emit startConnect(port,ip);
}
