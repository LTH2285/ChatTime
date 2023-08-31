#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H

#include <QWidget>
#include <QMenu>
#include <QApplication>
#include <QScreen>
#include <QPaintEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QDateTime>
#include <QFileDialog>

class Screen
{
public:
    enum STATUS
    {
        SELECT,
        MOV,
        SET_W_H,
    };
    Screen(QSize size);


    int maxWidth;
    int maxHeight;

    QPoint startPos;
    QPoint endPos;
    QPoint leftUpPos;
    QPoint rightDownPos;
    STATUS status;

    void setEnd(QPoint );
    void cmpPoint(QPoint &leftTop, QPoint &rightDown);
    bool isInArea(QPoint pos);
    void move(QPoint pos);
};



class ScreenWidget : public QWidget
{
    Q_OBJECT
public:
    ScreenWidget(bool b=true,QWidget *parent = 0);

    bool toShow;
    QMenu *menu;
    Screen *screen;
    QPixmap *fullScreen;
    QPixmap *bgScreen;

    QPoint movPos;

    void paintEvent(QPaintEvent *);
    void showEvent(QShowEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

    void contextMenuEvent(QContextMenuEvent *event);
public slots:
    void saveScreen();
    void saveFullScreen();
    void saveScreenOther();
    void saveFullScreenOther();
    void closeWidget();
};

#endif // SCREENWIDGET_H
