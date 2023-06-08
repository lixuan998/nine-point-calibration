#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    
    ui->setupUi(this);
    initUI();
    initParameters();
    connectMenuSignals();
    connectRealTimeCalcCheckBox();
    connectMouseTrackingSignals();
    connectStatusBarSignals();
    connectImageProcessThreadSignals();
    connectSpinBoxAndSliderSignals();
    //connectTabWidgetSignals();

    

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::initUI()
{
    int addx[] = {0, 20, 40, 0, 20, 40, 0, 20, 40};
    int addy[] = {0, 0, 0, 20, 20, 20, 40, 40, 40};
    int basic_x = 135;
    int basic_y = 130  ;
    ui->statusBar->showMessage("0, 0");
    //ui->image_result_tableview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    model.setHorizontalHeaderLabels({"ID", "Pixmap X-coordinate", "Pixmap Y-coordinate", "Robot X-coordinate", "Robot Y-coordinate"});
    for(int i = 0; i < 9; ++ i)
    {
        model.setItem(i, 0, new QStandardItem(QString::number(i + 1)));
        model.setItem(i, 3, new QStandardItem(QString::number(basic_x + addx[i])));
        model.setItem(i, 4, new QStandardItem(QString::number(basic_y + addy[i])));
    }

    ui->image_result_tableview->setModel(&model);
}
void MainWindow::initParameters()
{
    detector = new DetectThread("");
}
void MainWindow::resizeEvent(QResizeEvent * event)
{
    showQtImage(ui->source_img_label, origin_qt_img);
    showQtImage(ui->destination_img_label, des_qt_img);
    event->accept();
}

void MainWindow::connectRealTimeCalcCheckBox()
{
    connect(ui->real_time_calc_checkbox, &QCheckBox::toggled, this, &MainWindow::realTimeCalcToggled);
}

void MainWindow::connectMenuSignals()
{
    connect(ui->imgfile_action, &QAction::triggered, this, &MainWindow::choseImage);
    connect(ui->actioncfg, &QAction::triggered, this, &MainWindow::exportRes);
}

void MainWindow::connectStatusBarSignals()
{
    connect(ui->source_img_label, &MouseTrackingLabel::showCoordinateOnStatusBar, this, &MainWindow::showCoordinateOnStatusBar);
    connect(ui->destination_img_label, &MouseTrackingLabel::showCoordinateOnStatusBar, this, &MainWindow::showCoordinateOnStatusBar);
}

void MainWindow::connectMouseTrackingSignals()
{
    connect(this, &MainWindow::setImageSize, ui->source_img_label, &MouseTrackingLabel::getImageSize);
    connect(this, &MainWindow::setImageSize, ui->destination_img_label, &MouseTrackingLabel::getImageSize);
}

void MainWindow::connectImageProcessThreadSignals()
{
    connect(&image_processer, &ImageProcessThread::showCVImageOnDes, this, &MainWindow::showCVImageOnDes);
    connect(&image_processer, &ImageProcessThread::sendResContours, this, &MainWindow::receiveResContours);
}
void MainWindow::connectSpinBoxAndSliderSignals()
{
    connect(ui->thresh_spinbox, SIGNAL(valueChanged(int)), ui->thresh_slider, SLOT(setValue(int)));
    connect(ui->thresh_slider, SIGNAL(valueChanged(int)), ui->thresh_spinbox, SLOT(setValue(int)));
    connect(ui->thresh_slider, SIGNAL(valueChanged(int)), &image_processer, SLOT(setThresh(int)));
}
// void MainWindow::connectTabWidgetSignals()
// {
//     connect(ui->tabWidget_process, &QTabWidget::currentChanged, ui->stackedWidget, &QStackedWidget::setCurrentIndex);
// }
void MainWindow::showResOnTable()
{
    if(res_contours.size() != 9) return;
    for(size_t i = 0; i < res_contours.size(); ++i)
    {

        model.setItem(i, 1, new QStandardItem(QString::number(res_contours[i].getCenterPoint().x)));
        model.setItem(i, 2, new QStandardItem(QString::number(res_contours[i].getCenterPoint().y)));
        if(!trans_matrix.empty())
        {
            cv::Point2d pix_point(res_contours[i].getCenterPoint().x, res_contours[i].getCenterPoint().y);
            cv::Point2d robot_point;
            robot_point.x = pix_point.x * A + pix_point.y * B + C;
            robot_point.y = pix_point.x * D + pix_point.y * E + F;
            model.setItem(i, 3, new QStandardItem(QString::number(robot_point.x)));
            model.setItem(i, 4, new QStandardItem(QString::number(robot_point.y)));
        }
        //model.setItem(0, 4, new QStandardItem(QString::number(robot_point.y)));
        //model.setItem(0, 5, new QStandardItem(QString::number(robot_point.y)));
    }
}
void MainWindow::choseImage()
{
    QString img_file = QFileDialog::getOpenFileName(this, QString("选择文件"), QString("/home/climatex/calibration_hand_eye-master/ImgDB/hand_eye_calibration/hand2eye/"), QString(("Images (*.png *.jpeg *.bmp *.jpg)")));
    if(img_file == "") return;
    else
    {
        origin_qt_img.load(img_file);
        Converter::QImageToMat(origin_qt_img, origin_cv_img);
        emit setImageSize((double) origin_qt_img.width(), (double) origin_qt_img.height());
        showQtImage(ui->source_img_label, origin_qt_img);
    }
}

void MainWindow::on_calibration_hand2eye2D_button_clicked()
{
    if(res_contours.empty()) return;
    std::vector<cv::Point2d> pix_points;
    std::vector<cv::Point2d> robot_points;
    for(size_t i = 0; i < res_contours.size(); ++i)
    {
        pix_points.push_back(cv::Point2d(res_contours[i].getCenterPoint().x, res_contours[i].getCenterPoint().y));
    }
    for(int i = 0; i < model.rowCount(); ++ i)
    {
        robot_points.push_back(cv::Point2d((model.item(i, 3)->text().toDouble()), (model.item(i, 4)->text().toDouble())));
    }
    for(size_t i = 0; i < pix_points.size(); ++ i)
    {
        qDebug() << "pix_points[" << i << "]: x:" << pix_points[i].x << " ,y:" << pix_points[i].y;
    }
    for(size_t i = 0; i < robot_points.size(); ++ i)
    {
        qDebug() << "robot_points[" << i << "]: x:" << robot_points[i].x << " ,y:" << robot_points[i].y;
    }

    trans_matrix = cv::estimateRigidTransform(pix_points, robot_points, true);

    qDebug() << "A";
    if(trans_matrix.isContinuous()) trans_matrix.reshape(1, trans_matrix.channels());
qDebug() << "B";

    A = trans_matrix.at<double>(0, 0);
    B = trans_matrix.at<double>(0, 1);
    C = trans_matrix.at<double>(0, 2);
    D = trans_matrix.at<double>(1, 0);
    E = trans_matrix.at<double>(1, 1);
    F = trans_matrix.at<double>(1, 2);

    double p_x = 360;
    double p_y = 250;

    double r_x = p_x * A + p_y * B + C;
    double r_y = p_x * D + p_y * E + F;
    qDebug() << "r_x: " << r_x << " r_y: " << r_y;
}

void MainWindow::on_img_process_button_clicked()
{
    if(origin_cv_img.empty())
    {
        QMessageBox::information(nullptr, "提示", "请先选择处理图片", QMessageBox::Ok);
        return;
    }
    else
    {
        image_processer.setCvMat(origin_cv_img.clone());
        image_processer.setThresh(ui->thresh_spinbox->value());
        image_processer.start();
    }
}

void MainWindow::showQtImage(QLabel *showing_label, QImage &qt_img)
{
    if(qt_img.isNull()) return;
    QPixmap temp = QPixmap::fromImage(qt_img);
    temp = temp.scaled(ui->source_img_label->size(), Qt::KeepAspectRatio);
    showing_label->setPixmap(temp);
}

void MainWindow::showCoordinateOnStatusBar(double x, double y)
{
    qDebug() <<"A: " << A;
    qDebug() <<"B: " << B;
    qDebug() <<"C: " << C;
    qDebug() <<"D: " << D;
    qDebug() <<"E: " << E;
    qDebug() <<"F: " << F;
    QString coordinate_info = "Pixmap coordinate: (" + QString::number(x) + ", " + QString::number(y) + ")";
    if(!trans_matrix.empty())
    {
        double r_x = x * A + y * B + C;
        double r_y = x * D + y * E + F;
        coordinate_info += " Robot coordinate: (" + QString::number(r_x) + ", " + QString::number(r_y) + ")";
    }
    ui->statusBar->showMessage(coordinate_info);
}

void MainWindow::showCVImageOnDes(cv::Mat des_mat)
{
    this->des_cv_img = des_mat;
    Converter::MatToQImage(des_cv_img, des_qt_img);
    showQtImage(ui->destination_img_label, des_qt_img);
}

void MainWindow::realTimeCalcToggled(bool checked)
{
    if(origin_cv_img.empty()) return;
    if(checked)
    {
        if(connect(ui->thresh_slider, SIGNAL(valueChanged(int)), &image_processer, SLOT(setThresh(int))))
            disconnect(ui->thresh_slider, SIGNAL(valueChanged(int)), &image_processer, SLOT(setThresh(int)));
        connect(ui->thresh_slider, SIGNAL(valueChanged(int)), this, SLOT(realTimeCalc(int)));
    }
    else
    {
        if(connect(ui->thresh_slider, SIGNAL(valueChanged(int)), this, SLOT(realTimeCalc(int))))
            disconnect(ui->thresh_slider, SIGNAL(valueChanged(int)), this, SLOT(realTimeCalc(int)));
        connect(ui->thresh_slider, SIGNAL(valueChanged(int)), &image_processer, SLOT(setThresh(int)));
    }
}

void MainWindow::realTimeCalc(int threshold)
{
    if(image_processer.isRunning())
    {
        image_processer.quit();
        image_processer.wait();
    }
    image_processer.setThresh(threshold);
    image_processer.setCvMat(origin_cv_img.clone());
    image_processer.start();
}

void MainWindow::receiveResContours(std::vector<Contour> res_contours)
{
    this->res_contours = res_contours;
    showResOnTable();
}

void MainWindow::exportRes()
{
    QFile file("./calib_info.calib");
    file.open(QIODevice::WriteOnly);
    QString info;
    info += QString::number(ui->thresh_spinbox->value()) + "\n";
    info += QString::number((A)) + "\n";
    info += QString::number((B)) + "\n";
    info += QString::number((C)) + "\n";
    info += QString::number((D)) + "\n";
    info += QString::number((E)) + "\n";
    info += QString::number((F)) + "\n";

    file.write(info.toUtf8());

    QMessageBox::information(nullptr, "提示", "保存成功", QMessageBox::Ok);
}

//void MainWindow::on_pushButton_clicked()
//{
//    detector->start();
//}

