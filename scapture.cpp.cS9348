#include "scapture.h"
#include <QDebug>

SCapture::SCapture()
{

}

void SCapture::open()
{
    close();
    cap = cvCreateCameraCapture(0);
}

void SCapture::close()
{
    if (isOpened()) {
        cvReleaseCapture(&cap);
        cap = nullptr;
    }
}

bool SCapture::isOpened() const
{
    return cap != nullptr;
}

QImage SCapture::putImage(const Mat &mat)
{
    // 8-bits unsigned, NO. OF CHANNELS=1
    if(mat.type()==CV_8UC1)
    {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    if(mat.type()==CV_8UC3)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

cv::Mat SCapture::capture()
{
    if (isOpened()) {
        IplImage *iplimage = cvQueryFrame(cap);

        cv::Mat ma = cvarrToMat(iplimage,false);
        return ma;
    }
    ;
}

//Mat SCapture::diff(Mat firstMat, Mat secondMat)
//{
//    Mat diffmat;
//    absdiff(firstMat , secondMat, diffmat);
//    return diffmat;
//}

//void SCapture::uiPainter(QMainWindow* window, QWidget* widget,Mat paintMat)
//{
//    QImage img2 = putImage(paintMat);
//    img2 = img2.scaled(window->QWidget::->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
//    img2 = img2.mirrored(true,false);
//    ui->wdSecondImage->setImage(img2);
//}
