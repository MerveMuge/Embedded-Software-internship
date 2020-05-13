#include <iostream>
#include <unistd.h>
#include <string>
#include <thread>

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
#include <chrono>

#include <iterator>
#include <algorithm>
#include <array>

#include <functional>

#include "serialport.h"
#include "utils.h"
#include "activities.h"
#include "constants.h"

#include <bits/stdc++.h> //vector

int main() {
    std::array <char, BUFFER_SIZE> write_buff;
    write_buff.fill('0');

    int fileDescriptor = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);
    bool isPrint = false;

    Serialport * serial = new Serialport(fileDescriptor, isPrint);
    Utils * utils = new Utils(write_buff);
    Activities * activity = new Activities( write_buff, serial, utils );

    std::thread runThread(&Serialport::run, serial);
    runThread.detach();

    if(serial->openPort() == 0 ){
        write_buff.at(0) = '*';
        write_buff.at(1) = 'd'; //for get sensors info in default.
        write_buff.at(13) = '&';

        serial->setIsPrint(true);
        serial->writeToPort(write_buff);
        //serial->setIsPrint(false);
        write_buff.at(13) = '0';

        std::thread mainThread(&Activities::menu, activity);
        mainThread.join();

    }
    return 0;
}
