#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "opencv2/opencv.hpp"

namespace Ui {
class MainWindow;
}

using namespace cv;

class MainWindow : public QMainWindow
{
	Q_OBJECT
private:
	struct Private;
	Private *pv;
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();


    QImage putImage(const Mat &mat);
private slots:

	void doCapture();

	void on_action_file_save_as_triggered();

	void on_action_edit_copy_triggered();

    void on_btn_diff_clicked();

//    void diff(Mat firstMat, Mat secondMat);

    Ui::MainWindow *ui;
//    static void uiPainter(QWidgetItem *item,const Mat paintMat);
private:
    Mat bigImageMat;
    Mat savedPic1;
    Mat savedPic2;
    Mat diffmat;

};

#endif // MAINWINDOW_H
