#ifndef ACTIVITIES_H
#define ACTIVITIES_H

#include "utils.h"
#include "serialport.h"
#include "constants.h"

class Activities
{
public:
    Activities(std::array <char, BUFFER_SIZE > write_buff, Serialport *serial, Utils *utils);
    void quit_menu();
    std::array <char, BUFFER_SIZE > nextInput();
    std::array <char, BUFFER_SIZE > settingMenu_LDR_maxMinValues();
    std::array <char, BUFFER_SIZE > settingMenu_ledStatus();
    std::array <char, BUFFER_SIZE >  settingMenu_relay();
    std::array <char, BUFFER_SIZE > setting_menu();
    std::array <char, BUFFER_SIZE > reload_menu();
    std::array <char, BUFFER_SIZE > menu();
    std::array <char, BUFFER_SIZE> getWrite_buff();
    void setWrite_buff(std::array <char, BUFFER_SIZE > writeBuff );
private:
    std::array <char, BUFFER_SIZE > m_write_buff;
    Serialport *m_serial; // a pointer to serial port object
    Utils *m_utils;
};

#endif // ACTIVITIES_H
