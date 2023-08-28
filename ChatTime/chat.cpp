#include "chat.h"
#include "ui_chat.h"
#include "home.h"
#include <QApplication>
#include <QListWidget>
#include "fileoperation.h"
#include "facecharacter.h"


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


}

Chat::~Chat()
{
    delete ui;
}

void Chat::on_pushButton_clicked()
{
    this->close();
    Home *home = new Home(m_sendid);
    home->show();
}

void Chat::on_chatSendBtn_clicked()
{
    QString message = ui->chatInput->text();
    if(!message.trimmed().isEmpty()){
        ui->chatHistory->append( "I chat to " + m_recvusername + " :" + message);
    }

}

void Chat::on_chatFileBtn_clicked()
{
    fileOperation *file = new fileOperation();
    file->show();
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
