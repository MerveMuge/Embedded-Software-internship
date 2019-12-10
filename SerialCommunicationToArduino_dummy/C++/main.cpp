//to compile use: g++ serial_success.cpp -o serial -std=c++11

//you might not need every inclusion
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

using namespace std;

#define BAUDRATE B9600


int main(int argc, char** argv)
{

    int fileDescriptor = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);

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

    tcflush(fileDescriptor, TCIFLUSH);
    if (tcsetattr(fileDescriptor, TCSANOW, &newtio)) {
        perror("could not set the serial settings!");
        return -99;
    }

    int i = 0;
    string test = ">123,456,7890;";

    while(i < 10) {

        usleep(100000);

        string res = test + std::to_string(i) + "\n";
        long wrote = write(fileDescriptor, res.c_str(), sizeof(char)*res.size() );

        cout << res << endl;


        i++;
    }


    cout << "ready" << endl;

    return 0;
}
