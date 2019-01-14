#ifndef MYSETUP_H
#define MYSETUP_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>
#include "g_config_file.h"

namespace Ui {
class mySetup;
}

class mySetup : public QWidget
{
    Q_OBJECT

public:
    explicit mySetup(QWidget *parent = nullptr);
    ~mySetup();

protected:
    void init();
    void get_serialport_config();
    void set_config_ui(void);

private slots:
    void on_serial_button_clicked();

    void on_serial_Btn_update_Com_clicked();

private:
    Ui::mySetup *ui;


    QStringList const Baudlist = {
        "115200",
        "76800",
        "57600",
        "43000",
        "38400",
        "19200",
        "14400",
        "9600",
        "4800",
        "2400",
        "1200",
    };

    QStringList const Stopbit = {"1","1.5","2"};
    QStringList const Databit = {"8","7","6","5"};
    QStringList const Paritybit = {"None","Odd","Even","Space","Mark"};
    QStringList const FlowControl = {"NoFlowControl","HardWareControl","SoftWareControl"};

signals:
    void serial_config_Ok(bool);
    void serial_message(QString);

};

#endif // MYSETUP_H
