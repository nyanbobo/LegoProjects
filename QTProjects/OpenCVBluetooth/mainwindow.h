#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "bluetoothwrapper.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void showCamera();
    void scanBluetoothDevices();
    void connectToDevice();
    
private:
    Ui::MainWindow *ui;
    bluetoothWrapper bt;
    std::vector<std::string> bt_addr;

protected:
    void closeEvent(QCloseEvent * event);
};

#endif // MAINWINDOW_H
