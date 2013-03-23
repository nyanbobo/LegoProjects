#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(showCamera()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(close()));
    showCamera();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showCamera()
{
    VideoCapture cap(0); // open the default camera
    //if(!cap.isOpened())  // check if we succeeded


    Mat edges;
    namedWindow("edges",1);
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        //cvtColor(frame, edges, CV_BGR2GRAY);
        //GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        //Canny(edges, edges, 0, 30, 3);
        imshow("edges", frame);
        if(waitKey(50) >= 0)
            break;
    }
    cap.release();
    destroyWindow("edges");
}
