#include "detectthread.h"

void Ret::addPoint(cv::Point point)
{
    this->points.push_back(point);
}
int Ret::getSize()
{
    return points.size();
}

cv::Point Ret::getPoint(int index)
{
    return points[index];
}
DetectThread::DetectThread(QString img_path)
{
    this->img_path = img_path;
}

void DetectThread::run()
{
    QProcess *proc = new QProcess;
    QString proc_path = "python3";
    QStringList args;
    args << "/home/climatex/vscode_workspace/OCR/ocr_demo.py" << "/home/climatex/MVS_Data/1.bmp";
    proc->setProgram(proc_path);
    proc->setArguments(args);
    proc->start();
    proc->waitForFinished();
    qDebug() << "done";
    QString ret_string = proc->readAllStandardOutput();
    transDetectRes(ret_string);
}

void DetectThread::transDetectRes(QString ret_string)
{
    detect_res.clear();
    QStringList tmp_list = ret_string.split('\n');
    for(int i = tmp_list.size() - 1; i >= 0; -- i)
    {
        if(tmp_list[i].isEmpty()) tmp_list.removeAt(i);
        else
        {
            tmp_list[i].remove(0, 2);
            tmp_list[i].remove(tmp_list[i].size() - 1, 1);
        }
    }

    for(int i = 0; i < tmp_list.size(); ++ i)
    {
        detect_res.push_back(Ret());
        int left_brace = -1;
        for(int j = 0; j < tmp_list[i].size(); ++ j)
        {
            if(tmp_list[i][j] == '[') left_brace = j;
            if(tmp_list[i][j] == ']')
            {
                if(left_brace == -1) continue;
                QString tmp = tmp_list[i].mid(left_brace + 1, j - left_brace - 1);
                QStringList tmp_point = tmp.split(',');
                detect_res.back().addPoint(cv::Point(tmp_point[0].toInt(), tmp_point[1].toInt()));
                left_brace = -1;
            }
        }
    }

    for(size_t i = 0;i < detect_res.size(); ++ i)
    {
        for(int j = 0; j < detect_res[i].getSize(); ++ j)
        {
            qDebug() << "x: " << detect_res[i].getPoint(j).x << " y: " << detect_res[i].getPoint(j).y;
        }
    }
}
