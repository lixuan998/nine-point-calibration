#include "contour.h"

Contour::Contour(std::vector<cv::Point> single_contour)
{
    this->single_contour = single_contour;
    rotated_rect = cv::minAreaRect(single_contour);
}

cv::RotatedRect Contour::getRotatedRect()
{
    return rotated_rect;
}

std::vector<cv::Point> Contour::getContour()
{
    return single_contour;
}

cv::Point Contour::getCenterPoint()
{
    return rotated_rect.center;
}

int Contour::cmpX(Contour x, Contour y)
{
    return x.getRotatedRect().center.x < y.getRotatedRect().center.x;
}

int Contour::cmpY(Contour x, Contour y)
{
    return x.getRotatedRect().center.y < y.getRotatedRect().center.y;
}
