#include <QDebug>
#include <QMetaType>
#include <QString>
#include <QApplication>

#include "opencv_headers.h"
#include "contour.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<std::vector<Contour>>("std::vector<Contour>");
    MainWindow w;
    w.show();

    return a.exec();
}
