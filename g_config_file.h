#ifndef G_CONFIG_FILE_H
#define G_CONFIG_FILE_H

#include <QString>

class MySerialportConfig
{
public:
    QString g_com;
    QString g_speed;
    QString g_databit;
    QString g_parity;
    QString g_stopbit;
    QString g_flowcontrol;

public:
    MySerialportConfig();
    ~MySerialportConfig(){}
};

extern MySerialportConfig g_serialport_config;

#endif // G_CONFIG_FILE_H
