#ifndef AIAUDIO_H
#define AIAUDIO_H

#include <QWidget>
#include <QWidget>
#include "audio.h"

namespace Ui {
class Aiaudio;
}

class Aiaudio : public QWidget
{
    Q_OBJECT

public:
    explicit Aiaudio(QWidget *parent = nullptr);
    ~Aiaudio();


    void startAudio(QString fileName);
private slots:
    void on_pushButton_pressed();

    void on_pushButton_released();

    void on_clearBtn_clicked();

    void on_pushButton_clicked();

private:
    Ui::Aiaudio *ui;
    Audio *audio;
};

#endif // AIAUDIO_H
