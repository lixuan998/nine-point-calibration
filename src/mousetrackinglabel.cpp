#include "mousetrackinglabel.h"

MouseTrackingLabel::MouseTrackingLabel(QWidget *parent) : QLabel(parent)
{
    this->setMouseTracking(true);  //鼠标跟踪生效 即就算按钮不被按下，它也会一直接受鼠标移动的事件
    setFocusPolicy(Qt::StrongFocus);  //获取焦点
}

void MouseTrackingLabel::mouseMoveEvent(QMouseEvent *event)
{
    x = (double) event->x();
    y = (double) event->y();

    const QPixmap *temp = this->pixmap();
    if(temp == nullptr)
    {
        //qDebug() << "nullptr debug from mousetrackinglabel.cpp line 18";
        return;
    }
    else if(temp->isNull())
    {
        //qDebug() << "null pixmap debug from mousetrackinglabel.cpp line 23";
    }
    else
    {
        if(x <= temp->width() && y <= temp->height())
        {
            x = x / temp->width() * img_width;
            y = y / temp->height() * img_height;
            emit showCoordinateOnStatusBar(x, y);
        }
    }
}

void MouseTrackingLabel::getImageSize(double img_width, double img_height)
{
    this->img_height = img_height;
    this->img_width = img_width;
}
