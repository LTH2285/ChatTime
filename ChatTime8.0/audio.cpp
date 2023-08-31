#include "audio.h"

#include <QAudioInput>
#include <QFile>
#include <QIODevice>
#include <QDebug>

Audio::Audio()
{

}

void Audio::startAudio(QString fileName)
{


    QAudioDeviceInfo device = QAudioDeviceInfo::defaultInputDevice();
    if (device.isNull())    //录音设备不存在
    {
        QMessageBox::warning(NULL, "QAudioDeviceInfo", "录音设备不存在");
    }
    else
    {
        //音频编码要求
        QAudioFormat m_format;
        //设置采样频率
        m_format.setSampleRate(16000);
        //设置通道数
        m_format.setChannelCount(1);
        //设置位深
        m_format.setSampleSize(16);
        //设置编码
        m_format.setCodec("audio/pcm");

        //判断设备是否支持该格式
        if (!device.isFormatSupported(m_format))
        {
            m_format = device.nearestFormat(m_format);
        }

        //打开文件
        m_file = new QFile;
        m_file->setFileName(fileName);
        m_file->open(QIODevice::WriteOnly);

        //创建录音对象
        m_audio = new QAudioInput(m_format, this);
        m_audio->start(m_file);
    }


}

void Audio::stopAudio()
{
    //停止录音
    m_audio->stop();

    //关闭文件
    m_file->close();
    //删除文件对象
    delete m_file;
    m_file = NULL;
}
