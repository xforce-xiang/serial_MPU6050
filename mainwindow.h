#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QThread>
#include <QMainWindow>
#include <QDebug>
#include "mySerialPort.h"
#include <QMessageBox>
#include "mysetup.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_serial_button_clicked();
    void on_serial_send_btn_clicked();
    void on_btn_clear_txt_clicked();
    void on_pushButton_clicked();
    void showMessage(QByteArray);
    void Com_Message(QString);
    void calculate();
    void on_checkBox_Hex_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    mySetup *mySetupUi;
    mySerialPort *serial;
    mySerialPort *mSerialPort;
    QThread *myThread;
    QLabel *cominfolabel; //statusBar xianshi 串口信息.
    QLabel *countlbl;
    QByteArray bytesbuf;
    bool hexStatus = false;

    void init();
    void mylog(const QString log);
    void statusBarInit();
    void Bufferdata(QByteArray bytes);

    struct Quaternion //四元数
    {
        float w;
        float x;
        float y;
        float z;
    } quater;
    struct Euler    //欧拉角
    {
        float a;
        float b;
        float c;
    } euler;

    struct YAWPITCHROLL
    {
        float yaw;
        float pitch;
        float roll;
    } YPR;

    struct REALACCEL //未补偿的加速度
    {
        float x;
        float y;
        float z;
    } RACCEL;

    struct WORLDACCEL //已补偿的加速度
    {
        float x;
        float y;
        float z;
    } WACCEL;


signals:
    void data_str_send(QString);


};

#endif // MAINWINDOW_H
