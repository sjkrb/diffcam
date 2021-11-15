#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <stdint.h>
#include <QDebug>
#include <QTimer>
#include <QFileDialog>
#include <QClipboard>
#include "scapture.h"

#ifdef _WIN32
#ifdef QT_NO_DEBUG
#pragma comment(lib, "opencv_core2413.lib")
#pragma comment(lib, "opencv_highgui2413.lib")
#else
#pragma comment(lib, "opencv_core2413d.lib")
#pragma comment(lib, "opencv_highgui2413d.lib")
#endif
#endif

// solution 1: use cv::VideoCapture
//class Capture1 {
//private:
//	cv::VideoCapture cap;
//public:
//	void open()
//	{
//		close();
//		cap.open(0);
//	}
//	void close()
//	{
//		cap.release();
//	}
//	bool isOpened() const
//	{
//		return cap.isOpened();
//	}
//	QImage capture()
//	{
//		if (isOpened()) {
//			cv::Mat frame;
//			cap.read(frame);
//			if (frame.dims == 2 && frame.channels() == 3) {
//				int w = frame.cols;
//				int h = frame.rows;
//				QImage image(w, h, QImage::Format_RGB32);
//				for (int i = 0; i < h; i++) {
//					uint8_t const *src = frame.ptr(i);
//					QRgb *dst = (QRgb *)image.scanLine(i);
//					for (int j = 0; j < w; j++) {
//						dst[j] = qRgb(src[2], src[1], src[0]);
//						src += 3;
//					}
//				}
//				return image;
//			}
//		}
//		return QImage();
//	}
//};

// solution 2: use CvCapture


struct MainWindow::Private {
    //	Capture1 cap;
    SCapture cap;
    QTimer timer;
};


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    pv = new Private();
    ui->setupUi(this);

    pv->cap.open();

    connect(&pv->timer, SIGNAL(timeout()), this, SLOT(doCapture()));
    pv->timer.setInterval(0);
    pv->timer.start();
}

MainWindow::~MainWindow()
{
    pv->cap.close();
    delete pv;
    delete ui;
}

QImage MainWindow::putImage(const Mat &mat)
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
void MainWindow::doCapture()
{
    //	QImage image = pv->cap.capture();

    bigImageMat = pv->cap.capture();
    if (ui->btn_pic1->isChecked()==false)
    {
        savedPic1 = bigImageMat.clone();
    }
    if (ui->btn_pic2->isChecked()==false)
    {
        savedPic2 = bigImageMat.clone();
    }
    QImage img1 = putImage(savedPic1);
    img1 = img1.scaled(ui->wdfirstImage->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    img1 = img1.mirrored(true,false);
    ui->wdfirstImage->setImage(img1);

    QImage img2 = putImage(savedPic2);
    img2 = img2.scaled(ui->wdSecondImage->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    img2 = img2.mirrored(true,false);
    ui->wdSecondImage->setImage(img2);
}

void MainWindow::on_action_file_save_as_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save as"), QString(), "JPEG files (*.jpg);;PNG files (*.png)");
    if (path.isEmpty()) return;

    QImage image = ui->wdfirstImage->getImage();
    image.save(path);
}

void MainWindow::on_action_edit_copy_triggered()
{
    QImage image = ui->wdfirstImage->getImage();
    qApp->clipboard()->setImage(image);
}

void MainWindow::on_btn_diff_clicked()
{
    Mat filterAns;
    absdiff(savedPic1 , savedPic2, diffmat);
    QImage img4 = putImage(diffmat);
    img4 = img4.scaled(ui->weThirdImage->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    img4 = img4.mirrored(true,false);
    ui->weThirdImage->setImage(img4);

    cv::Size keysize(11,11);
    GaussianBlur(diffmat,filterAns,keysize,0.5,0.5);
    QImage img5 = putImage(diffmat);
    img5 = img5.scaled(ui->wdFourthImage->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    img5 = img5.mirrored(true,false);
    ui->wdFourthImage->setImage(img5);


}

