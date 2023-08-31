#ifndef FACECHARACTER_H
#define FACECHARACTER_H

#include <QWidget>

namespace Ui {
class FaceCharacter;
}

class FaceCharacter : public QWidget
{
    Q_OBJECT

public:
    explicit FaceCharacter(QWidget *parent = nullptr);
    ~FaceCharacter();

private slots:
    void on_insertBtn_clicked();

    void on_cancelBtn_clicked();

signals:
     void emojiSelected(const QString &emoji);
private:
    Ui::FaceCharacter *ui;
};

#endif // FACECHARACTER_H
