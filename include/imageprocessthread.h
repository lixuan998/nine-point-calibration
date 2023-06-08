#ifndef IMAGEPROCESSTHREAD_H
#define IMAGEPROCESSTHREAD_H

#include <QObject>
#include <QWidget>
#include <vector>
#include <QThread>
#include <QDebug>
#include "opencv_headers.h"

#include "contour.h"


class ImageProcessThread : public QThread
{
    Q_OBJECT
public:
    ImageProcessThread();


private:
    void run();

    void colorToBinary();

    void convertContourFromVector();

    bool Filter(Contour single_contour);

    void contourFilterate();

    void contourSort();

    void drawResult(cv::Mat draw_mat);

    void drawRect(cv::Mat draw_mat);

private:
    cv::Mat color_mat, binary_mat, grey_mat;

    //contours中的每一个元素存储了图像中的轮廓
    std::vector< std::vector<cv::Point> > contours_in_vector;

    std::vector<Contour> contours_in_class_before_filterate, contours_in_class_after_filterate;
    double thresh;
public slots:
    void setCvMat(cv::Mat color_mat);
    void setThresh(int thresh);

signals:
    void showCVImageOnDes(cv::Mat);

    void sendResContours(std::vector<Contour>);
};

#endif // IMAGEPROCESSTHREAD_H
