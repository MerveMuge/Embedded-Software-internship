#include "serialport.h"

#include <iostream>
#include <unistd.h>
#include <string>

#include <stdio.h> // standard input / output functions
#include <string.h> // string function definitions
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h> // File control definitions
#include <errno.h> // Error number definitions
#include <termios.h> // POSIX terminal control definitionss
#include <time.h>   // time calls
#include <stdlib.h>
#include <curses.h>
#include <cstdlib>

#include <iterator>
#include <algorithm>
#include <array>

#include <bits/stdc++.h> //vector

Serialport::Serialport(int fileDescriptor , bool isPrint){
    m_fileDescriptor = fileDescriptor;
    m_isPrint = isPrint;
}

bool Serialport::getIsPrint(){
    return m_isPrint;
}

void Serialport::setIsPrint(bool isPrint){
    m_isPrint = isPrint;
}

int Serialport::getFileDescriptor(){
    return m_fileDescriptor;
}

void Serialport::setFileDescriptor(int fileDescriptor){
    m_fileDescriptor = fileDescriptor;
}

int Serialport::openPort(){

    struct termios newtio;
    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;

    // set to 8N1
    newtio.c_cflag &= ~PARENB;
    newtio.c_cflag &= ~CSTOPB;
    newtio.c_cflag &= ~CSIZE;
    newtio.c_cflag |= CS8;

    newtio.c_iflag = IGNPAR;

    // output mode to
    //newtio.c_oflag = 0;
    newtio.c_oflag |= OPOST;

    /* set input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;

    newtio.c_cc[VTIME] = 10; /* inter-character timer 1 sec */
    newtio.c_cc[VMIN] = 0; /* blocking read disabled  */

    tcflush(m_fileDescriptor, TCIFLUSH);

    if (tcsetattr(m_fileDescriptor, TCSANOW, &newtio)) {
        perror("could not set the serial settings!");
        return -99;
    }
    return 0;
}

void Serialport::writeToPort(std::array <char, BUFFER_SIZE> writeBuffer){
    std::string byte_configuration = "";
    for (int i = 0; i < BUFFER_SIZE ; i++) {
        byte_configuration = byte_configuration + writeBuffer.at(i);
    }
    write( m_fileDescriptor, byte_configuration.c_str(), sizeof(char)* byte_configuration.size() );
}

std::vector<char> Serialport::run(){
    char c;
    std::vector<char> sensorInfo;
    int cleanerCount = 0;
    while(true){
        bool isData =  read(m_fileDescriptor, &c, 1) > 0;
        if(isData){
            if(c == '&'){
                cleanerCount=1;
                if( ( cleanerCount = 1 ) && ( !sensorInfo.empty() ) ){
                    sensorInfo.clear();
                }
                cleanerCount = 2;
            }
            if(c != '&')
                sensorInfo.push_back(c);
        }
        if(sensorInfo.size() == 99){
            if( (getIsPrint())){
                //std::cout <<sensorInfo.size();
                for(int i = 0 ; i < (signed)sensorInfo.size() ; i++){
                    //std::cout << "-";
                    std::cout << sensorInfo.at(i);
                }

                setIsPrint(false);
                //sensorInfo.clear();
            }
        }
    }

    /*std::vector<char> sensorInfo;
    bool isPrinted = false;
    bool isReaded = false;

    char c;
    int cleanerCount = 0;
    while ( (isPrinted == false) && (isReaded == false) ){
        while( ( read(m_fileDescriptor, &c, 1) > 0  ) ){
            if(c == '&'){
                cleanerCount=1;
                if( ( cleanerCount = 1 ) && ( !sensorInfo.empty() ) ){
                    sensorInfo.clear();
                }
                cleanerCount = 2;
            }
            if(c != '&')
                sensorInfo.push_back(c);
            //fprintf(stdout, "%c", c);
            isReaded = true;
        }
        isPrinted = true;
    }

    for(int i = 0 ; i < (signed)sensorInfo.size() ; i++){
        //std::cout << "-";
        std::cout << sensorInfo.at(i);
    }*/

    return sensorInfo;
}
