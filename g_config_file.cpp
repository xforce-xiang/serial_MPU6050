#include "g_config_file.h"
MySerialportConfig g_serialport_config;

MySerialportConfig::MySerialportConfig()
{
    this->g_com.clear();
    this->g_speed.clear();
    this->g_databit.clear();
    this->g_parity.clear();
    this->g_stopbit.clear();
    this->g_flowcontrol.clear();
}
