#include "screenwidget.h"
#include <QDebug>

Screen::Screen(QSize size)
{
    maxWidth = size.width();
    maxHeight = size.height();

    startPos = QPoint(-1, -1);
    endPos = startPos;
    leftUpPos = startPos;
    rightDownPos = startPos;
    status = SELECT;
}

void Screen::setEnd(QPoint p)
{
    endPos = p;
    leftUpPos = startPos;
    rightDownPos = endPos;
    cmpPoint(leftUpPos, rightDownPos);
}

void Screen::move(QPoint pos)
{
    int lx = leftUpPos.x() + pos.x();
    int ly = leftUpPos.y() + pos.y();
    int rx = rightDownPos.x() + pos.x();
    int ry = rightDownPos.y() + pos.y();

    if(lx < 0)
    {
        lx = 0;
        rx -= pos.x();
    }

    if(ly < 0)
    {
        ly = 0;
        ly -= pos.y();
    }

    if(rx > maxWidth)
    {
        rx = maxWidth;
        lx -= pos.x();
    }

    if(ry > maxHeight)
    {
        ry = maxHeight;
        ly -= pos.y();
    }

    leftUpPos = QPoint(lx, ly);
    rightDownPos = QPoint(rx, ry);
    startPos = leftUpPos;
    endPos = rightDownPos;
}

void Screen::cmpPoint(QPoint &leftTop, QPoint &rightDown)
{
    QPoint l = leftTop;
    QPoint r = rightDown;

    if(l.x() <= r.x())
    {
        if(l.y() > r.y())
        {
            leftTop.setY(r.y());
            rightDown.setY(l.y());
        }
    }
    else
    {
        if(l.y() < r.y())
        {
            leftTop.setX(r.x());
            rightDown.setX(l.x());
        }
        else
        {
            QPoint tmp;
            tmp = leftTop;
            leftTop = rightDown;
            rightDown = tmp;
        }
    }
}

bool Screen::isInArea(QPoint pos)
{
    if(pos.x() > leftUpPos.x() && pos.x() < rightDownPos.x() && pos.y()>leftUpPos.y() && pos.y()<rightDownPos.y())
        return true;

    return false;
}

ScreenWidget::ScreenWidget(bool toShow, QWidget *parent) : toShow(toShow), QWidget(parent)
{
    menu = new QMenu(this);
    menu->addAction("保存当前截图", this, SLOT(saveScreen()));
    menu->addAction("保存全屏截图", this, SLOT(saveFullScreen()));
    menu->addAction("截图另存为", this, SLOT(saveScreenOther()));
    menu->addAction("全屏另存为", this, SLOT(saveFullScreenOther()));
    menu->addAction("退出截图", this, SLOT(closeWidget()));

    screen = new Screen(qApp->primaryScreen()->geometry().size());

    fullScreen = new QPixmap();
    setAttribute(Qt::WA_DeleteOnClose);
}

void ScreenWidget::paintEvent(QPaintEvent *)
{
    int x = screen->leftUpPos.x();
    int y = screen->leftUpPos.y();
    int w = screen->rightDownPos.x() - x;
    int h = screen->rightDownPos.y() - y;

    QPainter painter(this);

    QPen pen;
    pen.setColor(Qt::green);
    pen.setWidth(2);
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    painter.drawPixmap(0, 0, *bgScreen);

    if(w!=0 && h!=0)
        painter.drawPixmap(x, y, fullScreen->copy(x, y, w, h));

    painter.drawRect(x, y, w, h);

    pen.setColor(Qt::yellow);
    painter.setPen(pen);
    painter.drawText(x+2, y-8, tr("截图范围: ( %1 x %2 ) - ( %3 x %4 ) 图片大小: (%5 x %6)").arg(x).arg(y).arg(x+w).arg(y+h).arg(w).arg(h));
//    qDebug() << "paintEvent";
}

void ScreenWidget::showEvent(QShowEvent *)
{
    QPoint point(-1, -1);
    screen->startPos = point;
    screen->setEnd(point);

    QScreen *pscreen = qApp->primaryScreen();
    *fullScreen = pscreen->grabWindow(0, 0, 0, screen->maxWidth, screen->maxHeight);

    QPixmap pixmap(screen->maxWidth, screen->maxHeight);
    pixmap.fill(QColor(160, 160, 160, 200));
    bgScreen = new QPixmap(*fullScreen);
    QPainter painter(bgScreen);
    painter.drawPixmap(0, 0, pixmap);
//    qDebug() << "showEvent";
}

#define STRDATETIME qPrintable (QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss"))
void ScreenWidget::saveScreen()
{
    int x = screen->leftUpPos.x();
    int y = screen->leftUpPos.y();
    int w = screen->rightDownPos.x() - x;
    int h = screen->rightDownPos.y() - y;

    qDebug() << "x = " << x;
    qDebug() << "y = " << y;
    qDebug() << "w = " << w;
    qDebug() << "h = " << h;

    QString fileName = QString("%1/screen_%2.png").arg(qApp->applicationDirPath()).arg(STRDATETIME);
    fullScreen->copy(x, y, w, h).save(fileName, "png");

    close();
}

void ScreenWidget::saveFullScreen()
{
    QString fileName = QString("%1/full_%2.png").arg(qApp->applicationDirPath()).arg(STRDATETIME);
    fullScreen->save(fileName, "png");
    close();
}

void ScreenWidget::saveScreenOther()
{
    QString name = QString("%1.png").arg(STRDATETIME);
    QString fileName = QFileDialog::getSaveFileName(this, "保存图片", name, "png Files (*.png)");
/*    if(!fileName.endsWith(".png"))
    {
        fileName += ".png";
    }*/

    if(fileName.length() > 0)
    {
        int x = screen->leftUpPos.x();
        int y = screen->leftUpPos.y();
        int w = screen->rightDownPos.x() - x;
        int h = screen->rightDownPos.y() - y;

        fullScreen->copy(x, y, w, h).save(fileName, "png");
    }
    this->close();
}

void ScreenWidget::saveFullScreenOther()
{
    QString name = QString("%1.png").arg(STRDATETIME);
    QString fileName = QFileDialog::getSaveFileName(this, "保存图片", name, "png Files (*.png)");
    if (!fileName.endsWith(".png")) {
        fileName += ".png";
    }

    if (fileName.length() > 0) {
        fullScreen->save(fileName, "png");
    }
    this->close();
}

void ScreenWidget::closeWidget()
{
    this->close();
}

void ScreenWidget::mousePressEvent(QMouseEvent *e)
{
    int status = screen->status;

    if(status == Screen::SELECT)
    {
        screen->startPos = e->pos();
    }
    else if(status == Screen::MOV)
    {
        if(screen->isInArea(e->pos()) == false)
        {
            screen->startPos = e->pos();
            screen->status = Screen::SELECT;
        }
        else
        {
            movPos = e->pos();
            setCursor(Qt::SizeAllCursor);
            qDebug() << "sizeAllCursor";
        }
    }
    this->update();
}

void ScreenWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(screen->status == Screen::SELECT)
    {
        screen->setEnd(e->pos());
    }
    else if(screen->status == Screen::MOV)
    {
        QPoint p(e->x()-movPos.x(), e->y()-movPos.y());
        screen->move(p);
        movPos = e->pos();
    }
    this->update();
}

void ScreenWidget::mouseReleaseEvent(QMouseEvent *)
{
    if(screen->status == Screen::SELECT)
    {
        screen->status = Screen::MOV;
    }
    else if(screen->status == Screen::MOV)
    {
        this->setCursor(Qt::ArrowCursor);
    }
}

void ScreenWidget::contextMenuEvent(QContextMenuEvent *)
{
    setCursor(Qt::ArrowCursor);
    menu->exec(cursor().pos());
}







