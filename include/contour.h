#ifndef CONTOUR_H
#define CONTOUR_H

#include <QObject>
#include <QDebug>
#include <vector>
#include "opencv_headers.h"

class Contour
{

public:
    Contour(std::vector<cv::Point> single_contour);

    cv::RotatedRect getRotatedRect();

    std::vector<cv::Point> getContour();

    cv::Point getCenterPoint();

    static int cmpX(Contour x, Contour y);

    static int cmpY(Contour x, Contour y);


private:
    std::vector<cv::Point> single_contour;
    cv::RotatedRect rotated_rect;
};

#endif // CONTOUR_H
