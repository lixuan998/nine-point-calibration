#include "imageprocessthread.h"

ImageProcessThread::ImageProcessThread()
{

}
void ImageProcessThread::setCvMat(cv::Mat color_mat)
{
    this->color_mat = color_mat;
}
void ImageProcessThread::setThresh(int thresh)
{
    this->thresh = (double) thresh;
}
void ImageProcessThread::colorToBinary()
{
    switch (color_mat.channels())
    {
        case 1 :
        {
            grey_mat = color_mat;
            break;
        }
        case 3 :
        {
            cv::cvtColor(color_mat, grey_mat, cv::COLOR_BGR2GRAY);
            break;
        }
        case 4 :
        {
            cv::cvtColor(color_mat, grey_mat, cv::COLOR_BGRA2GRAY);
            break;
        }
        default: qDebug() << "这个图片的通道数闻所未闻 debug from imageprocessthread.cpp line 34";
    }
    cv::medianBlur(grey_mat, grey_mat, 3);
    cv::threshold(grey_mat, binary_mat, thresh, 255, cv::THRESH_BINARY_INV);
}

void ImageProcessThread::convertContourFromVector()
{
    contours_in_class_before_filterate.clear();
    for(size_t i = 0; i < contours_in_vector.size(); ++ i)
    {
        contours_in_class_before_filterate.push_back(Contour(contours_in_vector[i]));
    }
}

void ImageProcessThread::contourFilterate()
{
    contours_in_class_after_filterate.clear();
    contours_in_vector.clear();
    for(size_t i = 0; i < contours_in_class_before_filterate.size(); ++ i)
    {
        if(Filter(contours_in_class_before_filterate[i]))
        {
            contours_in_class_after_filterate.push_back(contours_in_class_before_filterate[i]);
            contours_in_vector.push_back(contours_in_class_before_filterate[i].getContour());
        }
    }
    contours_in_class_before_filterate.clear();
    qDebug() << "contours_in_class_after_filterate : " << contours_in_class_after_filterate.size();
}

bool ImageProcessThread::Filter(Contour single_contour)
{
    cv::RotatedRect tmp_rotated_rect = single_contour.getRotatedRect();
    int width = std::min(tmp_rotated_rect.size.height, tmp_rotated_rect.size.width);
    int height = std::max(tmp_rotated_rect.size.height, tmp_rotated_rect.size.width);

    double alpha = (double) width / (double) height;
    if(alpha > 0.8)
    {
        if(tmp_rotated_rect.size.area() > 10000) return true;
        else return false;
    }
    else return false;
}

void ImageProcessThread::contourSort()
{
    std::sort(contours_in_class_after_filterate.begin(), contours_in_class_after_filterate.end(), Contour::cmpY);
    if(contours_in_class_after_filterate.size() != 9) return;
    for(size_t i = 0; i < contours_in_class_after_filterate.size(); i += 3)
    {
        std::sort(contours_in_class_after_filterate.begin() + i, contours_in_class_after_filterate.begin() + i + 3, Contour::cmpX);
    }
}

void ImageProcessThread::drawResult(cv::Mat draw_mat)
{
    cv::drawContours(draw_mat, contours_in_vector, -1, cv::Scalar(0, 0, 255), 5);
    drawRect(draw_mat);
}

void ImageProcessThread::drawRect(cv::Mat draw_mat)
{
    cv::Point2f points[4];
    for(size_t i = 0; i < contours_in_class_after_filterate.size(); ++ i)
    {
        cv::circle(draw_mat, contours_in_class_after_filterate[i].getCenterPoint(), 10, cv::Scalar(0, 255, 0), 16);
        contours_in_class_after_filterate[i].getRotatedRect().points(points);
        for(size_t j = 0; j < 4; ++ j)
        {
            cv::line(draw_mat, points[j], points[(j + 1) % 4], cv::Scalar(255, 0, 0), 16, cv::FILLED);
        }

        cv::putText(draw_mat, std::to_string(i + 1), contours_in_class_after_filterate[i].getCenterPoint(), cv::FONT_HERSHEY_COMPLEX, 4, cv::Scalar(0, 0, 255), 10);
    }
}

void ImageProcessThread::run()
{
    colorToBinary();
    cv::findContours(binary_mat, contours_in_vector, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    convertContourFromVector();
    contourFilterate();
    contourSort();
    cv::cvtColor(binary_mat, color_mat, cv::COLOR_GRAY2BGR);
    drawResult(color_mat);
//    for(size_t i = 0; i < contours_in_class_after_filterate.size(); ++ i)
//    {
//        qDebug() << i << "th circle center point: " << contours_in_class_after_filterate[i].getCenterPoint().x << " " << contours_in_class_after_filterate[i].getCenterPoint().y;
//    }
    emit showCVImageOnDes(color_mat);
    emit sendResContours(contours_in_class_after_filterate);
}
