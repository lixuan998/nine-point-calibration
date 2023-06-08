#ifndef MOUSETRACKINGLABEL_H
#define MOUSETRACKINGLABEL_H

#include <QObject>
#include <QLabel>
#include <QMouseEvent>
#include <QDebug>

class MouseTrackingLabel : public QLabel
{
    Q_OBJECT
public:
    MouseTrackingLabel(QWidget *parent = nullptr);

private:
    void mouseMoveEvent(QMouseEvent *event);

private:
    double x, y;
    double img_width, img_height;
public slots:
    void getImageSize(double img_width, double img_height);
private slots:

signals:
    void showCoordinateOnStatusBar(double, double);


};

#endif // MOUSETRACKINGLABEL_H
