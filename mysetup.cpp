#include "mysetup.h"
#include "ui_mysetup.h"

mySetup::mySetup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mySetup)
{
    ui->setupUi(this);
    init();
}

mySetup::~mySetup()
{
    delete ui;

}

void mySetup::init()
{
    ui->serial_baud->addItems(Baudlist);
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        ui->serial_Com->addItem(QString("%1") .arg(info.portName()));
    }
    ui->serial_databit->addItems(Databit);
    ui->serial_stopbit->addItems(Stopbit);
    ui->serial_parity->addItems(Paritybit);
    ui->serial_flowcontrol->addItems(FlowControl);

    qDebug() << "UI initual finished ...";

    ui->serial_baud->setCurrentText("115200");
    ui->serial_databit->setCurrentText("8");
    ui->serial_stopbit->setCurrentText("1");
    ui->serial_parity->setCurrentText("None");

    ui->serial_button->setText("Open");
}

void mySetup::get_serialport_config()
{
    g_serialport_config.g_com = ui->serial_Com->currentText();
    g_serialport_config.g_databit=ui->serial_databit->currentText();
    g_serialport_config.g_parity = ui->serial_parity->currentText();
    g_serialport_config.g_speed = ui->serial_baud->currentText();
    g_serialport_config.g_stopbit = ui->serial_stopbit->currentText();
    g_serialport_config.g_flowcontrol = ui->serial_flowcontrol->currentText();
}

void mySetup::set_config_ui()
{
    if(ui->serial_button->text() == "Open")  //com处于关闭状态
    {
        ui->serial_Com->setEnabled(true);
        ui->serial_baud->setEnabled(true);
        ui->serial_databit->setEnabled(true);
        ui->serial_flowcontrol->setEnabled(true);
        ui->serial_parity->setEnabled(true);
        ui->serial_stopbit->setEnabled(true);
        ui->serial_Btn_update_Com->setEnabled(true);
    }
    else if(ui->serial_button->text() == "Close") //Com处于开启状态
    {
        ui->serial_Com->setDisabled(true);
        ui->serial_baud->setDisabled(true);
        ui->serial_databit->setDisabled(true);
        ui->serial_flowcontrol->setDisabled(true);
        ui->serial_parity->setDisabled(true);
        ui->serial_stopbit->setDisabled(true);
        ui->serial_Btn_update_Com->setDisabled(true);
    }
}

void mySetup::on_serial_button_clicked()
{
    get_serialport_config();

    if(ui->serial_button->text() == "Open") //当前close
    {
        int ret = QMessageBox::information(this,"提示","配置完成",QMessageBox::Ok,QMessageBox::Cancel);
        if(QMessageBox::Ok == ret)
        {
            emit serial_config_Ok(true);

            QString sym = " , ";
            QString str = g_serialport_config.g_com + sym + g_serialport_config.g_speed + \
                     sym + g_serialport_config.g_databit + sym + g_serialport_config.g_stopbit +\
                     sym + g_serialport_config.g_parity + sym + g_serialport_config.g_flowcontrol;
            emit serial_message(str);
            qDebug() << "-------------------已发送打开Com命令------------";
            ui->serial_button->setText("Close");

            this->close();
        }
    }
    else if(ui->serial_button->text() == "Close")
    {
        int ret = QMessageBox::information(this,"提示","确定关闭串口？",QMessageBox::Ok,QMessageBox::Cancel);
        if(QMessageBox::Ok == ret)
        {
            emit serial_config_Ok(false);
            QString str = "串口已关闭！";
            emit serial_message(str);
            ui->serial_button->setText("Open");
            qDebug() << "-------------------已关闭Com------------";

            this->close();


        }
    }
    set_config_ui();
}


void mySetup::on_serial_Btn_update_Com_clicked()
{
    ui->serial_Com->clear();
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        ui->serial_Com->addItem(QString("%1") .arg(info.portName()));
    }
}
