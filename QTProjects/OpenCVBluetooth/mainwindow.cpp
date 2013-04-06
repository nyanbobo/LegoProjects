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
    connect(ui->btScanButton, SIGNAL(clicked()), this, SLOT(scanBluetoothDevices()));
    connect(ui->btConnectButton, SIGNAL(clicked()), this, SLOT(connectToDevice()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::scanBluetoothDevices()
{
    std::vector<std::string> bt_name;
    QMessageBox msgbox;
    ui->btScanButton->setEnabled(false);
    bt_addr.clear();
    bt_name.clear();
    bt.bluetoothScan(bt_addr, bt_name);
    ui->btDeviceList->clear();
    if(bt_addr.size() == 0)
    {
        msgbox.warning(this, "No Device", "No bluetooth device found!!!");
    }
    for(int i = 0; i < bt_addr.size(); i++)
    {
        std::string tmp = bt_name[i];
        QListWidgetItem * newItem = new QListWidgetItem(tmp.c_str());
        newItem->setData(Qt::UserRole, bt_addr[i].c_str());
        ui->btDeviceList->addItem(newItem);
    }
    ui->btScanButton->setEnabled(true);
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

void MainWindow::connectToDevice()
{
    int batteryLevel;
    QMessageBox msgbox;
    std::string device_address;
    device_address = ui->btDeviceList->currentItem()->data(Qt::UserRole).toString().toStdString();
    if(bt.bluetoothConnect(device_address))
    {
        batteryLevel = bt.getBatteryLevel();
        ui->batteryBar->setValue(batteryLevel/90);
    }
    else
    {
        msgbox.warning(this, "Cannot Connect", "Cannot connect to device!!!");
    }
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    bt.bluetoothClose();
}
