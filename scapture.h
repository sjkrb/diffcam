#ifndef SCAPTURE_H
#define SCAPTURE_H

#include <QImage>
#include "opencv2/opencv.hpp"
#include "MainWindow.h"


using namespace cv;


class SCapture
{

private:
    CvCapture *cap = nullptr;
public:
    SCapture();
    void open();
    void close();
    bool isOpened() const;
    QImage putImage(const Mat& mat);
    Mat capture();

};

#endif // SCAPTURE_H
