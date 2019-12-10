#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <constants.h>
#include <array>
#include <bits/stdc++.h> //vector

class Serialport
{
public:
    Serialport(int fileDescriptor, bool isPrint);
    int openPort();
    void writeToPort(std::array <char, BUFFER_SIZE> writeBuffer);
    std::vector<char> run();
    int getFileDescriptor();
    void setFileDescriptor(int fileDescriptor);
    bool getIsPrint();
    void setIsPrint(bool isPrint);
    std::vector<char> getSensorInfo();
    void setSensorInfo(std::vector<char> sensorInfo);
private:
    int m_fileDescriptor;
    bool m_isPrint;
};

#endif // SERIALPORT_H
