/**
* @projectName   serial_MPU
* @brief         MPU6500接收显示
* @author        梁作祥
* @date          2018-12-03
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qDebug() << "it's setting Ui...";
    //setWindowFlags(Qt::FramelessWindowHint | windowFlags());  //设置窗体无边框，窗体不能移动
    ui->setupUi(this);
    qDebug() << "Ui setting done...";
    init();
    statusBarInit();
}

void MainWindow::init()
{
    qDebug() << "init start ...";

    qDebug() << "set GYRO default is null ...";

    //    ui->serial_send_btn->setDisabled(true);
    bytesbuf.clear();

    qDebug() << "initial 90% ...";

    mySetupUi = new mySetup();
    this->mSerialPort = new mySerialPort;
    qDebug() << "initial 90% ...";

    this->myThread = new QThread;
    qDebug() << "declare myThread finished....";

    this->mSerialPort->moveToThread(this->myThread);
    qDebug() << "add serial to myThread ...";

    this->myThread->start();
    qDebug() << "Thread start ...";

    connect(this->mySetupUi,SIGNAL(serial_config_Ok(bool)),this->mSerialPort,SLOT(OpenCom(bool)));
    connect(this->mSerialPort,SIGNAL(dataRev(QByteArray)),this,SLOT(showMessage(QByteArray)));
    connect(this,SIGNAL(data_str_send(QString)),this->mSerialPort,SLOT(sendstring(QString)));
    connect(this->mySetupUi,SIGNAL(serial_message(QString)),this,SLOT(Com_Message(QString)));

    qDebug() << "init finished ...";
}

void MainWindow::statusBarInit()
{
    cominfolabel = new QLabel("串口没有打开...");
    countlbl = new QLabel("0000");
    statusBar()->addWidget(cominfolabel);
    statusBar()->addWidget(countlbl);
    statusBar()->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_serial_button_clicked()    //打开或关闭串口
{

}

void MainWindow::Bufferdata(QByteArray bytes)
{
    qDebug() << "enter buffer function...";
    QString str;
    str = str.prepend(bytes);
    int pos1,pos2;
    if(bytes.contains("*")&&bytes.contains('#'))
    {
        QString quat,ruler,ypr,areal;

        qDebug() << "cut Bufferdata ...\r\n";
        pos1 = str.indexOf('*');
        pos2 = str.lastIndexOf('#');

        str = str.mid(pos1+1,pos2-pos1-1); //截取字符串

        qDebug() << "exit cut Bufferdata ...\r\n";

        QStringList valuelist = str.split("\t");
        ui->label_log->setText("接收数据个数： "+QString::number(valuelist.length()));
        if(valuelist.length() <18)
        {
            qDebug() << "valuelist < 18 , exit...\r\n";
            return;
        }

        for(int i = 0 ;i < valuelist.length()-3; i++)
        {
            if(valuelist.at(i)=="quat")
            {
                ui->quat_w->setText(valuelist.at(i+1));
                ui->quat_x->setText(valuelist.at(i+2));
                ui->quat_y->setText(valuelist.at(i+3));
                ui->quat_z->setText(valuelist.at(i+4));
            }
            else if(valuelist.at(i) == "euler")
            {
                ui->euler_x->setText(valuelist.at(i+1));
                ui->euler_y->setText(valuelist.at(i+2));
                ui->euler_z->setText(valuelist.at(i+3));
            }
            else if(valuelist.at(i) == "ypr")
            {
                ui->ypr_x->setText(valuelist.at(i+1));
                ui->ypr_y->setText(valuelist.at(i+2));
                ui->ypr_z->setText(valuelist.at(i+3));
            }
            else if(valuelist.at(i) == "areal")
            {
                ui->areal_x->setText(valuelist.at(i+1));
                ui->areal_y->setText(valuelist.at(i+2));
                ui->areal_z->setText(valuelist.at(i+3));
            }
            else if(valuelist.at(i) == "aworld")
            {
                ui->aworld_x->setText(valuelist.at(i+1));
                ui->aworld_y->setText(valuelist.at(i+2));
                ui->aworld_z->setText(valuelist.at(i+3));
            }
            else
            {
                ;
            }
        }

        valuelist.clear();
    }
    else
    {
        qDebug() << "not include # * \r\n";
        return;
    }

}

void MainWindow::showMessage(QByteArray string)
{
    QString str;
    Bufferdata(string);

    if(hexStatus == true)
    {
        string = string.toHex();
        ui->txt_browse->append(str.prepend(string));
    }
    else
    {
        ui->txt_browse->append(str.prepend(string)); //QByteArray--->QString
    }

    str = QString::number(string.length());
    countlbl->setText("每帧接收的数据量：" + str);

}

void MainWindow::Com_Message(QString string)
{
    cominfolabel->setText(string);
}

void MainWindow::mylog(QString log)
{
    ui->txt_browse->append(log);
}

void MainWindow::on_serial_send_btn_clicked()
{
    QString str = ui->serial_send_lineEdit->text();
    emit data_str_send(str);
}

void MainWindow::on_btn_clear_txt_clicked()
{
    ui->txt_browse->clear();
}

void MainWindow::on_pushButton_clicked()
{
    mySetupUi->show();

}

void MainWindow::on_checkBox_Hex_stateChanged(int arg1)
{
    if(hexStatus == true)
    {
        hexStatus = false;
    }
    else
    {
        hexStatus = true;
    }
}
