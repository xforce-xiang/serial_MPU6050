#include "mySerialPort.h"

mySerialPort::mySerialPort()
{
    this->m_SerialPort = new QSerialPort(this);
    connect(this->m_SerialPort,SIGNAL(readyRead()),this,SLOT(read_Data()));

}

void mySerialPort::OpenCom(bool status)
{
    if(status)
    {
        m_SerialPort->setPortName(g_serialport_config.g_com);
        m_SerialPort->setBaudRate(getBaud(g_serialport_config.g_speed));
        m_SerialPort->setStopBits(getstopbit(g_serialport_config.g_stopbit));
        m_SerialPort->setDataBits(getdatabit(g_serialport_config.g_databit));
        m_SerialPort->setParity(getparity(g_serialport_config.g_parity));
        m_SerialPort->setFlowControl(getFlowControl(g_serialport_config.g_flowcontrol));
        m_SerialPort->setReadBufferSize(READ_BUF_SIZE);
        m_SerialPort->open(QIODevice::ReadWrite);


        qDebug() << g_serialport_config.g_com + " Open Success...";
    }
    else
    {
        m_SerialPort->close();
        qDebug() << g_serialport_config.g_com + " has been closed...";
    }

}


void mySerialPort::sendstring(QString string)
{
    if(m_SerialPort->isOpen())
    {
        QByteArray sendByte = string.toLatin1();
        m_SerialPort->write(sendByte.data(),sendByte.size());
    }

}

qint32 mySerialPort::getBaud(QString baud)
{
    return baud.toInt();
}

QSerialPort::StopBits mySerialPort::getstopbit(QString Stopbit)
{
    if(Stopbit == "1") return QSerialPort::OneStop;
    else if(Stopbit == "1.5") return QSerialPort::OneAndHalfStop;
    else if(Stopbit == "2") return  QSerialPort::TwoStop;
    else return QSerialPort::UnknownStopBits;
}

QSerialPort::DataBits mySerialPort::getdatabit(QString Databit)
{
    if(Databit == "5") return QSerialPort::Data5;
    else if(Databit == "6") return QSerialPort::Data6;
    else if(Databit == "7") return QSerialPort::Data7;
    else if(Databit == "8") return QSerialPort::Data8;
    else return QSerialPort::UnknownDataBits;
}
//"None","Odd","Even","Space","Mark"
QSerialPort::Parity mySerialPort::getparity(QString Paritybit)
{
    if(Paritybit == "None") return QSerialPort::NoParity;
    else if(Paritybit == "Odd") return QSerialPort::OddParity;
    else if(Paritybit == "Even") return QSerialPort::EvenParity;
    else if(Paritybit == "Space") return QSerialPort::SpaceParity;
    else if(Paritybit == "Mark") return QSerialPort::MarkParity;
    else return QSerialPort::UnknownParity;
}

QSerialPort::FlowControl mySerialPort::getFlowControl(QString FlowControl)
{
    if(FlowControl == "NoFlowControl") return QSerialPort::NoFlowControl;
    else if(FlowControl == "HardWareControl") return QSerialPort::HardwareControl;
    else if(FlowControl == "SoftWareControl") return QSerialPort::SoftwareControl;
    else return QSerialPort::UnknownFlowControl;
}


//串口接收到数据后组合成一帧，emit出去（帧内容是四元数）
void mySerialPort::read_Data()
{
    this->myReadBuf.append(this->m_SerialPort->readAll());

    emit real_time_data(myReadBuf);
//    QString string;
//    qDebug() << "real time data = " + string.prepend(myReadBuf) + ' ' + QString::number( myReadBuf.length());//QByteArray-->QString.


    if(myReadBuf.indexOf('*') != -1 && combination == false && myReadBuf.indexOf('#') == -1)  //检测到 * 号，且 字符串组合标志关闭
    {
        frame =  myReadBuf.mid(myReadBuf.indexOf('*')); //截取字符串
        combination = true;
    }

    if(myReadBuf.indexOf('*') == -1 && myReadBuf.indexOf('#') == -1 && combination == true)
    {
        frame += myReadBuf;
    }

    if(myReadBuf.indexOf('#') != -1 && combination == true )
    {
        frame += myReadBuf.mid(0,myReadBuf.indexOf('#') + 1);
        combination = false;
        emit dataRev(frame);
        QString string;
        qDebug() << "frame = " + string.prepend(frame) + ' ' + QString::number( frame.length());//QByteArray-->QString.
        frame.clear();

        if(myReadBuf.indexOf('*') != -1 && combination == false)
        {
            frame =  myReadBuf.mid(myReadBuf.indexOf('*')); //截取字符串
            combination = true;
        }
    }


    myReadBuf.clear();

}



















