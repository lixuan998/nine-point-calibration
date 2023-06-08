#ifndef DETECTTHREAD_H
#define DETECTTHREAD_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QProcess>
#include "opencv_headers.h"
class Ret{
public:
    void addPoint(cv::Point point);
    int getSize();
    cv::Point getPoint(int index);
private:
    std::vector<cv::Point> points;
    QString letter;
};

class DetectThread : public QThread
{
    Q_OBJECT
public:
    DetectThread(QString img_path);
private:
    void run();
    void transDetectRes(QString ret_string);

private:
    std::vector<Ret> detect_res;
    QString img_path;
};

#endif // DETECTTHREAD_H
