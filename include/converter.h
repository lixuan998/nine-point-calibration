#ifndef CONVERTER_H
#define CONVERTER_H

#include <QObject>
#include <QImage>
#include <QDebug>
#include "opencv_headers.h"

class Converter : public QObject
{
    Q_OBJECT
public:
    Converter();

    static void MatToQImage(cv::Mat &src_mat, QImage &des_qimg);

    static void QImageToMat(QImage &src_qimg, cv::Mat &des_mat);
};

#endif // CONVERTER_H
