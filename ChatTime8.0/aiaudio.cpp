#include "aiaudio.h"
#include "audio.h"
#include "ui_aiaudio.h"
#include "speech.h"

Aiaudio::Aiaudio(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Aiaudio)
{
    ui->setupUi(this);
    ui->pushButton->setText("press to speak");

}

Aiaudio::~Aiaudio()
{
    delete ui;
}

void Aiaudio::startAudio(QString fileName)
{

}

void Aiaudio::on_pushButton_pressed()
{


    audio = new Audio;
    audio->startAudio("1.pcm");
}

void Aiaudio::on_pushButton_released()
{
    //停止录音
    audio->stopAudio();
    //修改按钮文字

    //开始识别
    speech m_speech;
    QString text = m_speech.speechIdentify("1.pcm");

    ui->textEdit->setText(text);


}

void Aiaudio::on_clearBtn_clicked()
{
    ui->textEdit->clear();
}

void Aiaudio::on_pushButton_clicked()
{

}
