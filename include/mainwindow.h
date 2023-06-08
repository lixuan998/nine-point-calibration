#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QImage>
#include <QFileDialog>
#include <QPixmap>
#include <QLabel>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QProcess>
#include <QThread>
#include "opencv_headers.h"
#include <QFile>

#include "converter.h"
#include "imageprocessthread.h"
#include "detectthread.h"
#include "contour.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();    

private:
    void initUI();

    void initParameters();
    void resizeEvent(QResizeEvent * event);

    void connectMenuSignals();

    void connectStatusBarSignals();
    void connectRealTimeCalcCheckBox();

    void connectMouseTrackingSignals();

    void connectSpinBoxAndSliderSignals();

    void connectImageProcessThreadSignals();

    //void connectTabWidgetSignals();
    void showQtImage(QLabel *showing_label, QImage &qt_img);

    void showResOnTable();

private:
    cv::Mat origin_cv_img, des_cv_img;
    QImage origin_qt_img, des_qt_img;

    ImageProcessThread image_processer;
    DetectThread *detector;

    std::vector<Contour> res_contours;

    QStandardItemModel model;

    cv::Mat trans_matrix;

    double A, B, C, D, E, F;

    Ui::MainWindow *ui;

private slots:
    void on_img_process_button_clicked();

    void choseImage();
    void on_calibration_hand2eye2D_button_clicked();

    void showCoordinateOnStatusBar(double x, double y);

    void showCVImageOnDes(cv::Mat des_mat);

    void realTimeCalc(int thresh);

    void realTimeCalcToggled(bool checked);

    void receiveResContours(std::vector<Contour> res_contours);

    void exportRes();

//    void on_pushButton_clicked();

signals:
    void setImageSize(double width, double height);
    void sendCalibParams(QVector<double>);
};
#endif // MAINWINDOW_H
