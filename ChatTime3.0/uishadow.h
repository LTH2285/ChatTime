#ifndef UISHADOW_H
#define UISHADOW_H

#include <QObject>
#include <QGraphicsDropShadowEffect>
#include <QWidget>
class Shadow : public QGraphicsDropShadowEffect {
    Q_OBJECT

public:
    explicit Shadow(QObject *parent = nullptr)
        : QGraphicsDropShadowEffect(parent) {
        setBlurRadius(10);
        setXOffset(5);
        setYOffset(5);
        setColor(Qt::lightGray);
    }

    void applyToWidget(QWidget *widget) {
        widget->setGraphicsEffect(this);
        if (!widget->parentWidget()) {
            widget->setWindowFlags(Qt::FramelessWindowHint | widget->windowFlags());
        }
    }
};



#endif // UISHADOW_H
