#include "converter.h"

Converter::Converter()
{

}

void Converter::MatToQImage(cv::Mat &src_mat, QImage &des_qimg)
{
    switch (src_mat.type())
    {
        // 8位4通道
        case CV_8UC4:
        {
            des_qimg = QImage(src_mat.data, src_mat.cols, src_mat.rows, src_mat.step, QImage::Format_RGB32);

            break;
        }

        // 8位3通道
        case CV_8UC3:
        {
            des_qimg = QImage(src_mat.data, src_mat.cols, src_mat.rows, src_mat.step, QImage::Format_RGB888);
            des_qimg = des_qimg.rgbSwapped();
            break;
        }

        // 8位单通道
        case CV_8UC1:
        {
            qDebug() << "CV_8UC1";
            static QVector<QRgb>  sColorTable;
            // only create our color table once
            if (sColorTable.isEmpty())
            {
                for (int i = 0; i < 256; ++i) sColorTable.push_back(qRgb(i, i, i));
            }
            des_qimg = QImage(src_mat.data, src_mat.cols, src_mat.rows, src_mat.step, QImage::Format_Indexed8);
            des_qimg.setColorTable(sColorTable);
            break;
        }

        default:
            qDebug("Image format is not supported: depth=%d and %d channels\n", src_mat.depth(), src_mat.channels());
            break;
    }
}

void Converter::QImageToMat(QImage &src_qimg, cv::Mat &des_mat)
{
    qDebug() << src_qimg.format();
    switch (src_qimg.format())
    {
        case QImage::Format_ARGB32:
        case QImage::Format_RGB32:
        case QImage::Format_ARGB32_Premultiplied:
            des_mat = cv::Mat(src_qimg.height(), src_qimg.width(), CV_8UC4, (void*)src_qimg.constBits(), src_qimg.bytesPerLine());
            break;
        case QImage::Format_RGB888:
        {
            des_mat = cv::Mat(src_qimg.height(), src_qimg.width(), CV_8UC3, (void*)src_qimg.constBits(), src_qimg.bytesPerLine());
            cv::cvtColor(des_mat, des_mat, cv::COLOR_BGR2RGB);//QImage Format_RGB888是按照R,G,B排布，  Mat按照B,G,R排布， 因而，需要进行互换；
            break;
        }
        case QImage::Format_Indexed8:
            des_mat = cv::Mat(src_qimg.height(), src_qimg.width(), CV_8UC1, (void*)src_qimg.constBits(), src_qimg.bytesPerLine());
            break;
   }
}
