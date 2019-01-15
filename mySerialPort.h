#ifndef mySerialPort_H
#define mySerialPort_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include "g_config_file.h"

#define READ_BUF_SIZE 10

class mySerialPort :public QObject
{
    Q_OBJECT

public:
    mySerialPort();


public slots:
    void read_Data();
    void OpenCom(bool);
    void sendstring(QString string);

private:
    QByteArray myReadBuf;
    QByteArray frame;
    QSerialPort *m_SerialPort;
    bool combination = false;

    qint32 getBaud(QString baud);
    QSerialPort::StopBits getstopbit(QString Stopbit);
    QSerialPort::DataBits getdatabit(QString Databit);
    QSerialPort::Parity getparity(QString Paritybit);
    QSerialPort::FlowControl getFlowControl(QString FlowControl);

signals:
    void dataRev(QByteArray);
    void real_time_data(QByteArray);
};

#endif // mySerialPort_H
