#ifndef GROUPCHAT_H
#define GROUPCHAT_H

#include <QWidget>

namespace Ui {
class GroupChat;
}

class GroupChat : public QWidget
{
    Q_OBJECT

public:
    explicit GroupChat(QWidget *parent = nullptr);
    ~GroupChat();

private:
    Ui::GroupChat *ui;
};

#endif // GROUPCHAT_H
