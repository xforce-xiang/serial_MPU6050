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
    connect(this->mSerialPort,SIGNAL(real_time_data(QByteArray)),this,SLOT(showMessage(QByteArray)));

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

//接收处理串口emit过来的数据
void MainWindow::Bufferdata(QByteArray bytes)
{
    QString str;
    str = str.prepend(bytes);
    int pos1,pos2;
    if(bytes.indexOf('*') != -1 && bytes.indexOf('#') != -1)
    {
       // QString quat,ruler,ypr,areal;

        qDebug() << "analyze Bufferdata ...\r\n";
        pos1 = str.indexOf('*');
        pos2 = str.lastIndexOf('#');
        if(pos1 > pos2)
        {
//            qDebug() << "接收帧出错";
//            ui->label_log->setText("帧格式出错，放弃解析数据");
            ui->label_log->setText("--");
            return;
        }

        countlbl->setText("每帧接收的数据量：" + QString::number(bytes.length()));

        str = str.mid(pos1+1,pos2-pos1-1); //截取字符串

//        qDebug() << "exit cut Bufferdata ...\r\n";

        QStringList valuelist = str.split("\t");
        for(int x = 0;x < valuelist.length();x++)
        {
            qDebug() << QString::number(x) + ':' + valuelist.at(x);
        }
        ui->label_log->setText("接收数据个数： "+QString::number(valuelist.length()));

        for(int i = 0 ;i < valuelist.length()-3; i++)
        {
            if(valuelist.at(i)=="quat")
            {
                //                quater.w = valuelist.at(i+1).toFloat();
                //                quater.x = valuelist.at(i+2).toFloat();
                //                quater.y = valuelist.at(i+3).toFloat();
                //                quater.z = valuelist.at(i+4).toFloat();

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
        return;
    }

}

void MainWindow::calculate()
{

}

//因为使用append 16进制显示会自动换行，使用两位两行代码会是程序死掉
void MainWindow::showMessage(QByteArray string)
{
    QString str;
    Bufferdata(string);

    if(hexStatus == true)  //QByteArray本身就为16进制
    {
        string = string.toHex();
//        ui->txt_browse->moveCursor(QTextCursor::End);
//        ui->txt_browse->insertPlainText(str.prepend(string));
      ui->txt_browse->append(str.prepend(string));
    }
    else
    {
        // 以下两行代码可解决append自动换行问题
        ui->txt_browse->moveCursor(QTextCursor::End);
        ui->txt_browse->insertPlainText(str.prepend(string)); //QByteArray--->QString
//        ui->txt_browse->append(str.prepend(string));
    }
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
